open ReasonApolloTypes;
exception GraphQLErrors(array(graphqlError));
exception EmptyResponse;

let str = ReasonReact.string;

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

  let toast = Chakra.Toast.useToast();

  <SignUpMutation>
    ...{(mutate, {result}) => {
      <Group title="Sign Up">
        <Chakra.Text fontSize="md">
          "Register with Employment Hero email"->React.string
        </Chakra.Text>
        <br />
        <form>
          <Chakra.Input
            _type=`email
            placeholder="Email"
            onChange={e =>
              e->ReactEvent.Form.target##value->UpdateEmail->dispatch
            }
          />
          <br />
          <br />
          <Chakra.Input
            _type=`password
            placeholder="Password"
            onChange={e =>
              e->ReactEvent.Form.target##value->UpdatePassword->dispatch
            }
          />
          {switch (result) {
           | Error(e) => <ErrorAlert message=e##message />
           | _ => React.null
           }}
          <br />
          <br />
          <Chakra.Button
            onClick={e => {
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
                |> ignore
              | _ =>
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
                       toast(
                         ~title="Oops",
                         ~description="Something went wrong!",
                         ~position=`topRight,
                         ~status=`danger,
                         (),
                       )
                     | Data(data) =>
                       switch (data##createUser) {
                       | None =>
                         toast(
                           ~title="Oops",
                           ~description="Something went wrong!",
                           ~position=`topRight,
                           ~status=`danger,
                           (),
                         )
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
              };
            }}
            variantColor=`blue
            isLoading={result == Loading}>
            {str("Next")}
          </Chakra.Button>
        </form>
      </Group>
    }}
  </SignUpMutation>;
};
