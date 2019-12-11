module CardQueryConfig = [%graphql
  {|
  query allCards {
    allCards {
      id
      description
      badExample
      goodExample
    }
  }
|}
];

module Query = ReasonApolloHooks.Query.Make(CardQueryConfig);

[@react.component]
let make = (~id, ~session) => {
  open Session;
  let (queryState, _full) = Query.use();

  switch (queryState) {
  | Loading => <Spinner />
  | Data(data) =>
    switch (data##allCards |> Belt.Array.size) {
    | 0 => <Ehd.Empty />
    | _ =>
      <div>
        <Route.Link
          style={ReactDOMRe.Style.make(
            ~position="absolute",
            ~top="10px",
            ~left="10px",
            (),
          )}
          route=Route.Config.(Home)>
          <Ehd.Button> {"View all surveys" |> React.string} </Ehd.Button>
        </Route.Link>
        <CardsTable
          surveyId=id
          data={data##allCards |> Array.to_list}
          userId={session.userId}
        />
      </div>
    }
  | NoData => <EmptyData />
  | Error(e) => <FriendlyError message=e##message />
  };
};
