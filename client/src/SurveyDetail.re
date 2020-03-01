module CardQueryConfig = [%graphql
  {|
  query allCards($surveyId: ID!) {
    allCards {
      id
      description
      badExample
      goodExample
    }
    survey: Survey(id: $surveyId) {
      id
      enabled
    }
  }
|}
];

module Query = ReasonApolloHooks.Query.Make(CardQueryConfig);

[@react.component]
let make = (~id, ~session) => {
  open Session;
  let variables = CardQueryConfig.makeVariables(~surveyId=id, ());
  let (queryState, _full) = Query.use(~variables, ());

  switch (queryState) {
  | Loading => <AppSpinner />
  | Data(data) =>
    switch (data##survey) {
    | None => <FriendlyError message="Survey is not found" />
    | Some(s) =>
      switch (s##enabled) {
      | false => <FriendlyError message="Survey is diabled" />
      | true =>
        switch (data##allCards |> Belt.Array.size) {
        | 0 => <Empty />
        | _ =>
          <div>
            <Route.Link
              style={ReactDOMRe.Style.make(
                ~position="absolute",
                ~top="5px",
                ~left="10px",
                (),
              )}
              route=Route.Config.(Home)>
              <Chakra.Button
                variant=`ghost variantColor=`blue size=`sm leftIcon=`arrowLeft>
                {"All surveys" |> React.string}
              </Chakra.Button>
            </Route.Link>
            <CardsTable
              surveyId=id
              data={data##allCards |> Array.to_list}
              userId={session.userId}
            />
          </div>
        }
      }
    }
  | NoData => <EmptyData />
  | Error(e) => <FriendlyError message=e##message />
  };
};
