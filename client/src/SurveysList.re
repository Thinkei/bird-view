open Chakra;
let str = ReasonReact.string;

let optionStr =
  fun
  | None => ""
  | Some(v) => v;

module SurveysList = {
  [@react.component]
  let make = (~data, ~updatable, ~answerable, ~reportViewable) => {
    <div>
      <Table>
        <thead>
          <TableRow>
            <TableHeader> {"Sent At" |> str} </TableHeader>
            <TableHeader> {"Squad Name" |> str} </TableHeader>
            {updatable
               ? <TableHeader> {"Enabled?" |> str} </TableHeader> : React.null}
            <TableHeader> {"Actions" |> str} </TableHeader>
          </TableRow>
        </thead>
        <tbody>
          {data
           |> List.map(rowData => {
                <TableRow key=rowData##id>
                  <TableData>
                    {rowData##createdAt
                     |> Js.Json.decodeString
                     |> optionStr
                     |> Utils.formatDate
                     |> str}
                  </TableData>
                  <TableData> {rowData##squad##name |> str} </TableData>
                  {updatable
                     ? <TableData>
                         <SurveyStatusSwitch
                           surveyId=rowData##id
                           enabled=rowData##enabled
                         />
                       </TableData>
                     : ReasonReact.null}
                  <TableData>
                    <ButtonGroup spacing=4>
                      {answerable
                         ? <Route.Link
                             route=Route.Config.(SurveyDetail(rowData##id))>
                             <Button
                               variantColor=`blue
                               size=`sm
                               isDisabled={!rowData##enabled}>
                               {"Answer" |> str}
                             </Button>
                           </Route.Link>
                         : React.null}
                      {reportViewable
                         ? <Route.Link
                             style={ReactDOMRe.Style.make(
                               ~marginLeft="5px",
                               (),
                             )}
                             route=Route.Config.(SurveyReport(rowData##id))>
                             <Button variantColor=`cyan size=`sm>
                               {"View report" |> str}
                             </Button>
                           </Route.Link>
                         : ReasonReact.null}
                    </ButtonGroup>
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
        enabled
        squad {
          name
        }
      }
    }
  |}
];

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

[@react.component]
let make = (~squadId, ~role) => {
  switch (squadId) {
  | None =>
    <FriendlyError
      message="You don't belong to any squad. Contact Hieu Pham to assign you into a squad!"
    />
  | Some(squadId) =>
    let variables = QueryConfig.make(~squadId, ())##variables;
    let (queryState, _full) = Query.use(~variables, ());

    let updatable =
      switch (role) {
      | Some(`Leader)
      | Some(`Admin) => true
      | _ => false
      };

    let answerable =
      switch (role) {
      | Some(`Leader)
      | Some(`Member) => true
      | _ => false
      };

    let reportViewable =
      switch (role) {
      | Some(`Leader) => true
      | _ => false
      };

    <div>
      <Headline> {str("BIRD VIEW SURVEYS")} </Headline>
      {switch (queryState) {
       | Loading => <AppSpinner />
       | Data(data) =>
         <div>
           {switch (data##allSurveys |> Belt.Array.size) {
            | 0 => <Empty />
            | _ =>
              <SurveysList
                data={data##allSurveys |> Array.to_list}
                updatable
                answerable
                reportViewable
              />
            }}
         </div>
       | NoData => <EmptyData />
       | Error(e) => <FriendlyError message=e##message />
       }}
    </div>;
  };
};
