let str = ReasonReact.string;

let optionStr =
  fun
  | None => str("")
  | Some(v) => str(v);

module TableConfig = {
  type member = {
    .
    "email": string,
    "name": string,
  };

  type rowData = {
    .
    "id": string,
    "createdAt": Js.Json.t,
    "member": option(member),
  };

  type column =
    | CreatedAt
    | Name
    | Action;

  let getColumnHeader =
    fun
    | CreatedAt => "Created At"
    | Name => "Name"
    | Action => "Action";

  let render = data =>
    fun
    | CreatedAt => data##createdAt |> Js.Json.decodeString |> optionStr
    | Name =>
      data##member->Belt.Option.mapWithDefault("", member => member##name)
      |> str
    | Action =>
      <Button onClick={_ => ()}>
        <Route.Link route=Route.Config.(BirdViewDetail(data##id))>
          {str("Detail")}
        </Route.Link>
      </Button>;

  let columns = [CreatedAt, Name, Action];
};

module RenderByTable = Table.Make(TableConfig);

module QueryConfig = [%graphql
  {|
    query allMemberBirdViewTemplates($memberId: ID) {
      allMemberBirdViewTemplates(filter: { member: { id: $memberId}}) {
        id
        createdAt
        member {
          email
          name
        }
      }
    }
  |}
];

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

[@react.component]
let make = () => {
  let memberId = Session.memberId;
  let variables = QueryConfig.make(~memberId, ())##variables;
  let (queryState, _full) = Query.use(~variables, ());

  <div>
    <h2
      className=TW.(
        [TextAlign(TextCenter), Margin(My2), TextTransform(Uppercase)]
        |> make
      )>
      {str("Bird view answer history")}
    </h2>
    {switch (queryState) {
     | Loading => <Spinner />
     | Data(data) =>
       <RenderByTable
         data={data##allMemberBirdViewTemplates |> Array.to_list}
       />
     | NoData => <EmptyData />
     | Error(e) => <FriendlyError message=e##message />
     }}
  </div>;
};
