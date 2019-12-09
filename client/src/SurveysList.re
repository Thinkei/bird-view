open Ehd;
let str = ReasonReact.string;

let optionStr =
  fun
  | None => ""
  | Some(v) => v;

module SurveysList = {
  [@react.component]
  let make = (~data) => {
    <div>
      <Table>
        <thead>
          <TableRow>
            <TableHeader> {"Created At" |> str} </TableHeader>
            <TableHeader> {"Squad Name" |> str} </TableHeader>
            <TableHeader> {"Action" |> str} </TableHeader>
          </TableRow>
        </thead>
        <tbody>
          {data
           |> List.map(rowData => {
                <TableRow>
                  <TableData>
                    {rowData##createdAt
                     |> Js.Json.decodeString
                     |> optionStr
                     |> Utils.formatDate
                     |> str}
                  </TableData>
                  <TableData> {rowData##squad##name |> str} </TableData>
                  <TableData>
                    <Route.Link
                      route=Route.Config.(SurveyDetail(rowData##id))>
                      <Button> {"Detail" |> str} </Button>
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

module QueryConfig = [%graphql
  {|
    query allSurveys($squadId: ID) {
      allSurveys(filter: { squad: { id: $squadId}}, orderBy: createdAt_DESC) {
        id
        createdAt
        squad {
          name
        }
      }
    }
  |}
];

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

module NotAMemberOfAnySquad = {
  [@react.component]
  let make = () =>
    <div> {"You doesn't belong to any squad" |> ReasonReact.string} </div>;
};

[@react.component]
let make = (~session) => {
  Session.(
    switch (session.squadId) {
    | None => <NotAMemberOfAnySquad />
    | Some(squadId) =>
      let variables = QueryConfig.make(~squadId, ())##variables;
      let (queryState, _full) = Query.use(~variables, ());

      <div>
        <Headline> {str("YOUR BIRDVIEW SURVEYS")} </Headline>
        {switch (queryState) {
         | Loading => <Spinner />
         | Data(data) =>
           <div>
             {switch (session.squadId, session.role) {
              | (Some(squadId), Some(`Leader)) =>
                <CreateNewSurveyButton squadId />
              | (_, _) => ReasonReact.null
              }}
             <SurveysList data={data##allSurveys |> Array.to_list} />
           </div>
         | NoData => <EmptyData />
         | Error(e) => <FriendlyError message=e##message />
         }}
      </div>;
    }
  );
};
