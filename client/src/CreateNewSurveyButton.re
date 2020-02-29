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
let make = () => {
  let squadId = "1123";
  let (createdSurvey, setCreatedSurvey) = React.useState(() => false);
  let (openModal, setModalVisibility) = React.useState(() => false);
  <div className=TW.([Padding(Py2)] |> make)>
    <CreateSurveyMutation>
      ...{(mutate, {result}) => {
        <>
          <Button
            loading={Button.LoadingProp.Bool(result == Loading)}
            onClick={_ => setModalVisibility(_ => true)}
            _type=`primary>
            {"Create new survey" |> ReasonReact.string}
          </Button>
          <Modal
            closable=true
            visible=openModal
            onOk={_ => {
              setModalVisibility(_ => false);

              let variables =
                CreateSurveyConfig.make(~squadId, ())##variables;

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
                     setCreatedSurvey(_ => true);
                     Notification.success(
                       Notification.makeConfigProps(
                         ~message="Create new survey successfully!",
                         (),
                       ),
                     )
                     |> ignore;
                   };
                   Js.Promise.resolve();
                 })
              |> ignore;
            }}
            onCancel={_ => setModalVisibility(_ => false)}>
            {(
               createdSurvey
                 ? "Are you sure you want to create more survey?"
                 : "Create survey and send email to your squad members?"
             )
             |> React.string}
          </Modal>
        </>
      }}
    </CreateSurveyMutation>
  </div>;
};
