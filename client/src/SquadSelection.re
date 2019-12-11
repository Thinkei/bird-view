open Ehd;
open ReasonApolloTypes;
exception GraphQLErrors(array(graphqlError));
exception EmptyResponse;

module Option = Select.Option;

let str = React.string;

type squad = {
  id: string,
  name: string,
};

module QueryConfig = [%graphql
  {|
    query allSquads {
      allSquads @bsRecord {
        id
        name
      }
    }
  |}
];

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

module Selection = {
  module Config = [%graphql
    {|
  mutation updateUser($id: ID!, $squadId: ID!) {
    updateUser(id: $id, squadId: $squadId) {
      id
    }
  }
  |}
  ];

  module Mutation = ReasonApollo.CreateMutation(Config);

  [@react.component]
  let make = (~squads, ~userId, ~onSquadSelectSuccess) => {
    let (selectedSquad, setSelectedSquad) = React.useState(() => None);

    <Mutation>
      ...{(mutate, _) => {
        <div
          style={ReactDOMRe.Style.make(
            ~display="flex",
            ~padding="20px",
            ~justifyContent="center",
            (),
          )}>
          <Select
            style={ReactDOMRe.Style.make(
              ~width="200px",
              ~marginRight="10px",
              (),
            )}
            onChange={selected => setSelectedSquad(_ => Some(selected))}>
            {squads
             |> Array.map(squad => {
                  <Option key={squad.id}> {squad.name |> str} </Option>
                })
             |> React.array}
          </Select>
          <Button
            disabled={Belt.Option.isNone(selectedSquad)}
            onClick={e => {
              ReactEvent.Synthetic.preventDefault(e);
              Belt.Option.map(
                selectedSquad,
                squadId => {
                  let variables =
                    Config.make(~id=userId, ~squadId, ())##variables;

                  mutate(~variables, ())
                  |> Js.Promise.then_(res => {
                       switch (res) {
                       | Errors(_)
                       | EmptyResponse =>
                         Notification.error(
                           Notification.makeConfigProps(
                             ~message="Something went wrong!",
                             (),
                           ),
                         )
                         |> ignore
                       | Data(_) => onSquadSelectSuccess()
                       };
                       Js.Promise.resolve();
                     })
                  |> ignore;
                },
              ) |> ignore;
              ();
            }}
            htmlType="submit"
            _type=`primary>
            {str("Finish")}
          </Button>
        </div>
      }}
    </Mutation>;
  };
};

[@react.component]
let make = (~onSquadSelectSuccess, ~userId) => {
  let (queryState, _full) = Query.use();

  switch (queryState) {
  | Loading => <Spinner />
  | Data(res) =>
    <Selection squads=res##allSquads userId onSquadSelectSuccess />
  | NoData => <Empty />
  | Error(e) => <FriendlyError message=e##message />
  };
};
