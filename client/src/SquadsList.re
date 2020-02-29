open Ehd;

let str = React.string;

module QueryConfig = [%graphql
  {|
    query allSquads {
      allSquads(orderBy: name_ASC) {
        id
        name
        leaders: users(last: 1, filter: { role: Leader }) {
          id
          name
          email
        }
        lastSurveys: surveys(last: 1) {
          id
          createdAt
        }
      }
    }
  |}
];

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

let (|?) = (x, y) =>
  switch (x) {
  | Some(x) => x
  | None => y
  };

let toDefaultList =
  fun
  | None => []
  | Some(arr) => Array.to_list(arr);

module DisplayInTable = {
  [@react.component]
  let make = (~data) => {
    <div>
      <CreateNewSurveysButton squadIds={data |> List.map(r => r##id)} />
      <Table>
        <thead>
          <TableRow>
            <TableHeader> {"Squad Name" |> str} </TableHeader>
            <TableHeader> {"Squad Lead" |> str} </TableHeader>
            <TableHeader> {"Last Sent" |> str} </TableHeader>
            <TableHeader> {"Actions" |> str} </TableHeader>
          </TableRow>
        </thead>
        <tbody>
          {data
           |> List.map(rowData => {
                <TableRow key=rowData##id>
                  <TableData> {rowData##name |> str} </TableData>
                  <TableData>
                    {(
                       switch (rowData##leaders |> toDefaultList) {
                       | [] => "N/A"
                       | [leader, ..._] =>
                         leader##name |? (leader##email |? "Unknown")
                       }
                     )
                     |> str}
                  </TableData>
                  <TableData>
                    {(
                       switch (rowData##lastSurveys |> toDefaultList) {
                       | [] => "N/A"
                       | [lastSurvey, ..._] =>
                         lastSurvey##createdAt
                         |> Js.Json.decodeString
                         |> (s => s |? "" |> Utils.formatDate)
                       }
                     )
                     |> str}
                  </TableData>
                  <TableData>
                    <Route.Link route=Route.Config.(Squad(rowData##id))>
                      <Button _type=`primary> {"Detail" |> str} </Button>
                    </Route.Link>
                  </TableData>
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
