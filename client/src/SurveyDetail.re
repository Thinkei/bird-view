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
let make = (~id as _) => {
  let (queryState, _full) = Query.use();

  switch (queryState) {
  | Loading => <Spinner />
  | Data(data) => <CardsTable data={data##allCards |> Array.to_list} />
  | NoData => <EmptyData />
  | Error(e) => <FriendlyError message=e##message />
  };
};
