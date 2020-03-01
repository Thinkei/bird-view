open Ehd;
open ReasonApolloTypes;
exception GraphQLErrors(array(graphqlError));
exception EmptyResponse;

let str = ReasonReact.string;

module Step = Steps.Step;

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

[@bs.deriving jsConverter]
type currentStep =
  | [@bs.as 1] ProvidingEmailPwd
  | [@bs.as 2] SelectingSquad;

type state = {
  email: string,
  password: string,
  userId: string,
  currentStep,
};

type action =
  | UpdateSignUpInfo(string, string, string)
  | UpdateCurrentStep(currentStep);

[@react.component]
let make = (~setSession) => {
  let (state, dispatch) =
    React.useReducer(
      state =>
        fun
        | UpdateSignUpInfo(email, password, userId) => {
            ...state,
            email,
            password,
            userId,
          }
        | UpdateCurrentStep(currentStep) => {...state, currentStep},
      {email: "", password: "", userId: "", currentStep: ProvidingEmailPwd},
    );
  <div>
    {switch (state.currentStep) {
     | ProvidingEmailPwd =>
       <SignUp
         onSignUpSuccess={(email, password, userId) => {
           dispatch(UpdateSignUpInfo(email, password, userId));
           dispatch(UpdateCurrentStep(SelectingSquad));
         }}
       />
     | SelectingSquad =>
       <SignInMutation>
         ...{(mutate, _) => {
           <SquadSelection
             onSquadSelectSuccess={() => {
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
                      switch (data##signinUser##token, data##signinUser##user) {
                      | (Some(tk), Some(user)) =>
                        setSession({
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
             userId={state.userId}
           />
         }}
       </SignInMutation>
     }}
  </div>;
};
