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
        message="You don't belong to any squad. Try to re-signin or contact Hieu Pham for support"
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
            message="You don't have any survey to answer. Contact Phat Le for support"
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
