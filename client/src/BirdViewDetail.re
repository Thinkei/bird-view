module QueryConfig = [%graphql
  {|
  query MemberBirdViewTemplate($id: ID) {
    memberBirdViewTemplate: MemberBirdViewTemplate(id: $id) {
      birdViewTemplate {
        name
        questions {
          id
          content
          goodExample
          badExample
        }
      }
      member {
        id
        name
        email
      }
    }
  }
|}
];

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

[@react.component]
let make = (~id) => {
  let variables = QueryConfig.make(~id, ())##variables;
  let (queryState, _full) = Query.use(~variables, ());

  switch (queryState) {
  | Loading => <Spinner />
  | Data(data) =>
    let template = data##memberBirdViewTemplate;
    switch (template) {
    | None => <EmptyData />
    | Some(data) =>
      <div>
        <MemberInfo
          name={
            data##member
            ->Belt.Option.mapWithDefault("Unknown", member => member##name)
          }
        />
        {switch (data##birdViewTemplate) {
         | None => <FriendlyError message="Invalid birdview template" />

         | Some(data) => <BirdViewTemplate name=data##name />
         }}
      </div>
    };
  | NoData => <EmptyData />
  | Error(e) => <FriendlyError message=e##message />
  };
};
