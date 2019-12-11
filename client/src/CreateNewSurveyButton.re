open Ehd;

open ReasonApolloTypes;
exception GraphQLErrors(array(graphqlError));
exception EmptyResponse;

module CreateSurveyConfig = [%graphql
  {|
  mutation createSurvey($squadId: ID!) {
    createSurvey(squadId: $squadId) {
      id
    }
  }
|}
];

module CreateSurveyMutation = ReasonApollo.CreateMutation(CreateSurveyConfig);

[@react.component]
let make = (~squadId) => {
  <div className=TW.([Padding(Py2)] |> make)>
    <CreateSurveyMutation>
      ...{(mutate, {result}) => {
        <Button
          onClick={_ => {
            let variables = CreateSurveyConfig.make(~squadId, ())##variables;

            mutate(~variables, ~refetchQueries=[|"allSurveys"|], ())
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
                 | Data(_) =>
                   Notification.success(
                     Notification.makeConfigProps(
                       ~message="Create new survey successfully!",
                       (),
                     ),
                   )
                   |> ignore
                 };
                 Js.Promise.resolve();
               })
            |> ignore;
          }}
          loading={Button.LoadingProp.Bool(result == Loading)}
          _type=`primary>
          {"Create new survey" |> ReasonReact.string}
        </Button>
      }}
    </CreateSurveyMutation>
  </div>;
};
