let str = ReasonReact.string;

let optionStr =
  fun
  | None => str("")
  | Some(v) => str(v);

module TableConfig = {
  type rowData = {
    .
    "id": string,
    "createdAt": Js.Json.t,
  };

  type column =
    | Id
    | CreatedAt;

  let getColumnHeader =
    fun
    | Id => "Id"
    | CreatedAt => "Created At";

  let render = data =>
    fun
    | Id => data##id |> str
    | CreatedAt => data##createdAt |> Js.Json.decodeString |> optionStr;

  let columns = [Id, CreatedAt];
};

module RenderByTable = Table.Make(TableConfig);

/* Fixed member id temporarily */
module QueryConfig = [%graphql
  {|
  query allMemberBirdViewTemplates {
    allMemberBirdViewTemplates(filter: { member: { id: "ck22zb1e5050g0165vovwu8ct"}}) {
      id
      createdAt
    }
  }
|}
];

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

[@react.component]
let make = () => {
  let (queryState, _full) = Query.use();

  switch (queryState) {
  | Loading => <p> {str("Loading...")} </p>
  | Data(data) =>
    <RenderByTable data={data##allMemberBirdViewTemplates |> Array.to_list} />
  | NoData
  | Error(_) => <p> {React.string("You are lucky! Ping us")} </p>
  };
};
