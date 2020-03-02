open ReasonApolloTypes;
exception GraphQLErrors(array(graphqlError));
exception EmptyResponse;

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
    let toast = Chakra.Toast.useToast();

    <Mutation>
      ...{(mutate, {result}) => {
        <div>
          <div>
            <Chakra.Text fontSize="sm">
              {"Your squad:" |> React.string}
            </Chakra.Text>
            <Chakra.Select
              onChange={e => {
                let selectedSquad = e->ReactEvent.Form.target##value->Some;
                setSelectedSquad(_ => selectedSquad);
              }}>
              <option value="" />
              {squads
               |> Array.map(squad => {
                    <option key={squad.id} value={squad.id}>
                      {squad.name |> str}
                    </option>
                  })
               |> React.array}
            </Chakra.Select>
          </div>
          <br />
          <div>
            <Chakra.Text fontSize="sm">
              {"Your role:" |> React.string}
            </Chakra.Text>
            <Chakra.Select
              defaultValue={roleToJs(`Member)}
              onChange={e => {
                let selectedRole =
                  e->ReactEvent.Form.target##value->roleFromJs |? `Member;
                setRole(_ => selectedRole);
              }}>
              <option key={roleToJs(`Member)} value={roleToJs(`Member)}>
                {roleToJs(`Member) |> str}
              </option>
              <option key={roleToJs(`Leader)} value={roleToJs(`Leader)}>
                {roleToJs(`Leader) |> str}
              </option>
            </Chakra.Select>
          </div>
          <br />
          <Chakra.Button
            isLoading={result == Loading}
            isDisabled={Belt.Option.isNone(selectedSquad)}
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
                         toast(
                           ~title="Oops",
                           ~description="Something went wrong!",
                           ~position=`topRight,
                           ~status=`danger,
                           (),
                         )
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
            variantColor=`blue>
            {str("Finish")}
          </Chakra.Button>
        </div>
      }}
    </Mutation>;
  };
};

[@react.component]
let make = (~onSquadSelectSuccess, ~userId) => {
  let (queryState, _full) = Query.use();

  switch (queryState) {
  | Loading => <AppSpinner />
  | Data(res) =>
    <Selection squads=res##allSquads userId onSquadSelectSuccess />
  | NoData => <Empty />
  | Error(e) => <FriendlyError message=e##message />
  };
};
