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

module SignUpConfig = [%graphql
  {|
  mutation signUp($email: String!, $password: String!) {
    createUser(authProvider: {email: {email: $email, password: $password }}) {
      id
    }
  }
|}
];

module SignUpMutation = ReasonApollo.CreateMutation(SignUpConfig);

type state = {
  email: string,
  password: string,
};

type action =
  | UpdateEmail(string)
  | UpdatePassword(string);

[@react.component]
let make = (~onSignUpSuccess) => {
  let (state, dispatch) =
    React.useReducer(
      state =>
        fun
        | UpdateEmail(email) => {...state, email}
        | UpdatePassword(password) => {...state, password},
      {email: "", password: ""},
    );

  <SignUpMutation>
    ...{(mutate, {result}) => {
      <div style=layoutWrapper>
        <Card
          title="Sign Up" style={ReactDOMRe.Style.make(~width="300px", ())}>
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
                  SignUpConfig.make(
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
                       switch (data##createUser) {
                       | None =>
                         Notification.error(
                           Notification.makeConfigProps(
                             ~message="Something went wrong!",
                             (),
                           ),
                         )
                         |> ignore
                       | Some(user) =>
                         onSignUpSuccess(
                           state.email,
                           state.password,
                           user##id,
                         )
                       }
                     };
                     Js.Promise.resolve();
                   })
                |> ignore;
              }}
              loading={Button.LoadingProp.Bool(result == Loading)}
              htmlType="submit"
              _type=`primary>
              {str("Next")}
            </Button>
          </form>
        </Card>
      </div>
    }}
  </SignUpMutation>;
};
