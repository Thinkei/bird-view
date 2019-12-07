let str = ReasonReact.string;

let optionStr =
  fun
  | None => str("")
  | Some(v) => str(v);

module TableConfig = {
  type squad = {. "name": string};

  type rowData = {
    .
    "id": string,
    "createdAt": Js.Json.t,
    "name": string,
    "squad": squad,
  };

  type column =
    | CreatedAt
    | Name
    | SquadName
    | Action;

  let getColumnHeader =
    fun
    | CreatedAt => "Created At"
    | Name => "Survey Name"
    | SquadName => "Squad Name"
    | Action => "Action";

  let render = (data, column) =>
    switch (column) {
    | CreatedAt => data##createdAt |> Js.Json.decodeString |> optionStr
    | Name => data##name |> str
    | SquadName => data##squad##name |> str
    | Action =>
      <Route.Link
        className=TW.(
          [
            BackgroundColor(BgBlue500),
            BackgroundColor(HoverBgBlue700),
            TextColor(TextWhite),
            Padding(Py2),
            Padding(Px4),
            BorderRadius(Rounded),
          ]
          |> make
        )
        route=Route.Config.(SurveyDetail(data##id))>
        {str("Detail")}
      </Route.Link>
    };

  let columns = [CreatedAt, Name, SquadName, Action];
};

module SurveysList = CreateTable.Make(TableConfig);

module QueryConfig = [%graphql
  {|
    query allSurveys($squadId: ID) {
      allSurveys(filter: { squad: { id: $squadId}}) {
        id
        createdAt
        name
        squad {
          name
        }
      }
    }
  |}
];

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

[@react.component]
let make = () => {
  let squadId = Session.squadId;
  let variables = QueryConfig.make(~squadId, ())##variables;
  let (queryState, _full) = Query.use(~variables, ());

  <div>
    <Headline> {str("YOUR BIRDVIEW SURVEYS")} </Headline>
    {switch (queryState) {
     | Loading => <Spinner />
     | Data(data) => <SurveysList data={data##allSurveys |> Array.to_list} />
     | NoData => <EmptyData />
     | Error(e) => <FriendlyError message=e##message />
     }}
  </div>;
};
