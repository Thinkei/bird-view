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

/* Fixed member id temporarily */
module QueryConfig = [%graphql
  {|
  query allMemberBirdViewTemplates {
    allMemberBirdViewTemplates(filter: { member: { id: "ck22zb1e5050g0165vovwu8ct"}}) {
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
  let (queryState, _full) = Query.use();

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
     | NoData
     | Error(_) => <p> {React.string("You are lucky! Ping us")} </p>
     }}
  </div>;
};
