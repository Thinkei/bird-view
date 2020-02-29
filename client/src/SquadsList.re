let str = React.string;

module QueryConfig = [%graphql
  {|
    query allSquads {
      allSquads(orderBy: name_ASC) {
        id
        name
        users {
          id
          role
          name
        }
      }
    }
  |}
];

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

module DisplayInTable = {
  [@react.component]
  let make = (~data) => {
    <div>
      <Table>
        <thead>
          <TableRow>
            <TableHeader> {"Id" |> str} </TableHeader>
            <TableHeader> {"Squad Name" |> str} </TableHeader>
            <TableHeader> {"Action" |> str} </TableHeader>
          </TableRow>
        </thead>
        <tbody>
          {data
           |> List.map(rowData => {
                <TableRow key=rowData##id>
                  <TableData> {rowData##id |> str} </TableData>
                  <TableData> {rowData##name |> str} </TableData>
                  <TableData> React.null </TableData>
                </TableRow>
              })
           |> Array.of_list
           |> ReasonReact.array}
        </tbody>
      </Table>
    </div>;
  };
};

[@react.component]
let make = () => {
  let (queryState, _full) = Query.use();

  switch (queryState) {
  | Loading => <Spinner />
  | Data(data) =>
    <div>
      {switch (data##allSquads |> Belt.Array.size) {
       | 0 => <Ehd.Empty />
       | _ => <DisplayInTable data={data##allSquads |> Array.to_list} />
       }}
    </div>
  | NoData => <EmptyData />
  | Error(e) => <FriendlyError message=e##message />
  };
};
