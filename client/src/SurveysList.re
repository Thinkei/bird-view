open Ehd;
let str = ReasonReact.string;

let optionStr =
  fun
  | None => ""
  | Some(v) => v;

module SurveysList = {
  [@react.component]
  let make = (~data, ~isLeader) => {
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
                      <Button _type=`primary> {"Answer" |> str} </Button>
                    </Route.Link>
                    {isLeader
                       ? <Route.Link
                           style={ReactDOMRe.Style.make(
                             ~marginLeft="5px",
                             (),
                           )}
                           route=Route.Config.(SurveyReport(rowData##id))>
                           <Button> {"View report" |> str} </Button>
                         </Route.Link>
                       : ReasonReact.null}
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

[@react.component]
let make = (~session) => {
  Session.(
    switch (session.squadId) {
    | None =>
      <FriendlyError
        message="You doesn't belong to any squad. Contact Hieu Pham to assign you into a squad!"
      />
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
             {switch (data##allSurveys |> Belt.Array.size) {
              | 0 => <Ehd.Empty />
              | _ =>
                <SurveysList
                  data={data##allSurveys |> Array.to_list}
                  isLeader={
                    switch (session.role) {
                    | Some(`Leader) => true
                    | _ => false
                    }
                  }
                />
              }}
           </div>
         | NoData => <EmptyData />
         | Error(e) => <FriendlyError message=e##message />
         }}
      </div>;
    }
  );
};
