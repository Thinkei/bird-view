module QueryConfig = [%graphql
  {|
  query surveys($squadId: ID!) {
    allSurveys(last: 1, orderBy: createdAt_ASC, filter: { enabled: true, squad: { id: $squadId }}) {
      id
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
        message="You don't belong to any squad. Contact Hieu Pham to assign you into a squad!"
      />
    | Some(squadId) =>
      let variables = QueryConfig.makeVariables(~squadId, ());
      let (queryState, _full) = Query.use(~variables, ());

      switch (queryState) {
      | Loading => <AppSpinner />
      | Data(data) =>
        switch (data##allSurveys |> Array.to_list) {
        | [] =>
          <FriendlyError
            message="You don't have any survey to answer. Contact Phat Le to support"
          />
        | [survey, ..._] =>
          Js.log(survey##id);
          <SurveyDetail session id={survey##id} />;
        }
      | NoData => <EmptyData />
      | Error(e) => <FriendlyError message=e##message />
      };
    }
  );
};
