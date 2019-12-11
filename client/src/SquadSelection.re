open Ehd;
open ReasonApolloTypes;
exception GraphQLErrors(array(graphqlError));
exception EmptyResponse;

module Option = Select.Option;

let str = React.string;

let (|?) = (x, y) =>
  switch (x) {
  | None => y
  | Some(x) => x
  };

type squad = {
  id: string,
  name: string,
};

module QueryConfig = [%graphql
  {|
    query allSquads {
      allSquads(orderBy: name_ASC) @bsRecord {
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
  mutation updateUser($id: ID!, $squadId: ID!, $role: Role!) {
    updateUser(id: $id, squadId: $squadId, role: $role) {
      id
    }
  }
  |}
  ];

  module Mutation = ReasonApollo.CreateMutation(Config);

  [@react.component]
  let make = (~squads, ~userId, ~onSquadSelectSuccess) => {
    open Session;
    let (selectedSquad, setSelectedSquad) = React.useState(() => None);
    let (role, setRole) = React.useState(() => `Member);

    <Mutation>
      ...{(mutate, _) => {
        <div>
          <div
            style={ReactDOMRe.Style.make(
              ~display="flex",
              ~padding="20px",
              ~justifyContent="center",
              (),
            )}>
            <div>
              <p> {"Your squad:" |> React.string} </p>
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
            </div>
            <div>
              <p> {"Your role:" |> React.string} </p>
              <Select
                style={ReactDOMRe.Style.make(
                  ~width="120px",
                  (),
                )}
                defaultValue={roleToJs(`Member)}
                onChange={role => setRole(_ => roleFromJs(role) |? `Member)}>
                <Option key={roleToJs(`Member)}>
                  {roleToJs(`Member) |> str}
                </Option>
                <Option key={roleToJs(`Leader)}>
                  {roleToJs(`Leader) |> str}
                </Option>
              </Select>
            </div>
          </div>
          <div
            style={ReactDOMRe.Style.make(
              ~display="flex",
              ~padding="10px",
              ~justifyContent="center",
              (),
            )}>
            <Button
              disabled={Belt.Option.isNone(selectedSquad)}
              onClick={e => {
                ReactEvent.Synthetic.preventDefault(e);
                Belt.Option.map(
                  selectedSquad,
                  squadId => {
                    let variables =
                      Config.make(~id=userId, ~squadId, ~role, ())##variables;

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
                )
                |> ignore;
                ();
              }}
              htmlType="submit"
              _type=`primary>
              {str("Finish")}
            </Button>
          </div>
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
