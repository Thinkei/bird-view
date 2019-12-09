open Ehd;
open ReasonApolloTypes;
exception GraphQLErrors(array(graphqlError));
exception EmptyResponse;

let str = ReasonReact.string;

let layoutWrapper =
  ReactDOMRe.Style.make(~display="flex", ~justifyContent="center", ());
let sider = ReactDOMRe.Style.make(~lineHeight="120px", ());
let content =
  ReactDOMRe.Style.make(~minHeight="120px", ~lineHeight="120px", ());

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

type state = {
  email: string,
  password: string,
};
type action =
  | UpdateEmail(string)
  | UpdatePassword(string);

[@react.component]
let make = (~setSesion) => {
  React.useEffect0(() => {
    Notification.warning(
      Notification.makeConfigProps(
        ~message="You need to sign in to continue!",
        (),
      ),
    );
    None;
  });

  let (state, dispatch) =
    React.useReducer(
      state =>
        fun
        | UpdateEmail(email) => {...state, email}
        | UpdatePassword(password) => {...state, password},
      {email: "", password: ""},
    );

  <SignInMutation>
    ...{(mutate, {result}) => {
      <div style=layoutWrapper>
        <Card
          title="Sign In" style={ReactDOMRe.Style.make(~width="300px", ())}>
          <form>
            <Input
              _type="email"
              style={ReactDOMRe.Style.make(~height="28px", ())}
              placeholder="Email"
              onChange={e =>
                e->ReactEvent.Form.target##value->UpdateEmail->dispatch
              }
            />
            <br />
            <br />
            <Input
              _type="password"
              style={ReactDOMRe.Style.make(~height="28px", ())}
              placeholder="Password"
              onChange={e =>
                e->ReactEvent.Form.target##value->UpdatePassword->dispatch
              }
            />
            <br />
            <br />
            <Button
              onClick={e => {
                ReactEvent.Synthetic.preventDefault(e);
                let signinVariables =
                  SignInConfig.make(
                    ~email=state.email,
                    ~password=state.password,
                    (),
                  )##variables;

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
                       switch (
                         data##signinUser##token,
                         data##signinUser##user,
                       ) {
                       | (Some(tk), Some(user)) =>
                         setSesion({
                           token: tk,
                           role: user##role,
                           userId: user##id,
                           squadId:
                             Belt.Option.map(user##squad, squad => squad##id),
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
              }}
              loading={Button.LoadingProp.Bool(result == Loading)}
              htmlType="submit"
              _type=`primary>
              {str("Sign in")}
            </Button>
          </form>
        </Card>
      </div>
    }}
  </SignInMutation>;
};
