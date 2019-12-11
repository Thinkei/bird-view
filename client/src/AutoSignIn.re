open Ehd;
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
let make = (~email, ~password, ~setSession) =>
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
               Notification.error(
                 Notification.makeConfigProps(
                   ~message="Something went wrong!",
                   (),
                 ),
               )
               |> ignore
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
                 Notification.error(
                   Notification.makeConfigProps(
                     ~message="Something went wrong!",
                     (),
                   ),
                 )
                 |> ignore
               };
               Notification.success(
                 Notification.makeConfigProps(
                   ~message="Sign in successfully",
                   (),
                 ),
               )
               |> ignore;
             };
             Js.Promise.resolve();
           })
        |> ignore;
        None;
      });
      <div> {"Logging in..." |> React.string} </div>;
    }}
  </SignInMutation>;
