open ReasonApolloTypes;
exception GraphQLErrors(array(graphqlError));
exception EmptyResponse;

let str = ReasonReact.string;

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
let make = (~setSession) => {
  let toast = Chakra.Toast.useToast();
  let (touched, setTouched) = React.useState(() => false);
  React.useEffect0(() => {
    toast(
      ~title="Hi there",
      ~description="You need to sign in to continue!",
      ~position=`topRight,
      ~status=`warning,
      (),
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
      <Group title="Sign In">
        <Chakra.Text fontSize="md">
          "Sign in with Employment Hero email"->React.string
        </Chakra.Text>
        <br />
        <form>
          <Chakra.Input
            _type=`email
            placeholder="Email"
            onChange={e =>
              e->ReactEvent.Form.target##value->UpdateEmail->dispatch
            }
            isRequired=true
            isInvalid={touched && !FormValidation.validEHEmail(state.email)}
          />
          <br />
          <br />
          <Chakra.Input
            _type=`password
            placeholder="Password"
            isRequired=true
            onChange={e =>
              e->ReactEvent.Form.target##value->UpdatePassword->dispatch
            }
            isInvalid={
              touched && !FormValidation.nonEmptyString(state.password)
            }
          />
          {switch (result) {
           | Error(e) => <ErrorAlert message=e##message />
           | _ => React.null
           }}
          <br />
          <br />
          <Chakra.Button
            _type=`submit
            onClick={e => {
              setTouched(_ => true);
              ReactEvent.Synthetic.preventDefault(e);

              switch (
                FormValidation.validEHEmail(state.email),
                FormValidation.nonEmptyString(state.password),
              ) {
              | (false, _) =>
                toast(
                  ~title="Oops",
                  ~description="Invalid email",
                  ~position=`topRight,
                  ~status=`warning,
                  (),
                )
              | (_, false) =>
                toast(
                  ~title="Oops",
                  ~description="Require non-empty password",
                  ~position=`topRight,
                  ~status=`warning,
                  (),
                )
              | _ =>
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
                       toast(
                         ~title="Oops",
                         ~description="Something went wrong!",
                         ~position=`topRight,
                         ~status=`danger,
                         (),
                       )
                     | Data(data) =>
                       open Session;
                       switch (
                         data##signinUser##token,
                         data##signinUser##user,
                       ) {
                       | (Some(tk), Some(user)) =>
                         setSession({
                           token: tk,
                           role: user##role,
                           userId: user##id,
                           squadId:
                             Belt.Option.map(user##squad, squad => squad##id),
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
              };
            }}
            variantColor=`blue
            isLoading={result == Loading}>
            {str("Sign in")}
          </Chakra.Button>
        </form>
      </Group>
    }}
  </SignInMutation>;
};
