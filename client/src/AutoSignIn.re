open ReasonApolloTypes;
exception GraphQLErrors(array(graphqlError));
exception EmptyResponse;

module SignInConfig = [%graphql
  {|
  mutation signinUser($email: String!, $password: String!) {
    signinUser(email: { email: $email, password: $password }) {
      token
      user {
        id
        role
        name
        squad { id }
      }
    }
  }
|}
];

module SignInMutation = ReasonApollo.CreateMutation(SignInConfig);
[@react.component]
let make = (~email, ~password, ~setSession) => {
  let toast = Chakra.Toast.useToast();
  <SignInMutation>
    ...{(mutate, _) => {
      React.useEffect0(() => {
        let signinVariables =
          SignInConfig.make(~email, ~password, ())##variables;

        mutate(~variables=signinVariables, ())
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
             | Data(data) =>
               open Session;
               switch (data##signinUser##token, data##signinUser##user) {
               | (Some(tk), Some(user)) =>
                 setSession({
                   token: tk,
                   role: user##role,
                   userId: user##id,
                   squadId: Belt.Option.map(user##squad, squad => squad##id),
                 })
               | (_, _) =>
                 toast(
                   ~title="Oops",
                   ~description="Something went wrong!",
                   ~position=`topRight,
                   ~status=`danger,
                   (),
                 )
               };
               toast(
                 ~title="Welcome back!",
                 ~description="You've signed in successfully",
                 ~position=`topRight,
                 ~status=`success,
                 (),
               );
             };
             Js.Promise.resolve();
           })
        |> ignore;
        None;
      });
      <div> {"Logging in..." |> React.string} </div>;
    }}
  </SignInMutation>;
};
