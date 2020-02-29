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

let useDisclosure = () => {
  let (value, setter) = React.useState(() => false);
  let onClose = () => setter(_ => true);
  let onOpen = () => setter(_ => false);
  (value, onOpen, onClose);
};

[@react.component]
let make = (~squadIds) => {
  let (isCreatedSuvey, onCreatedSurvey, _) = useDisclosure();
  let (isOpenModal, onOpenedModal, onClosedModal) = useDisclosure();
  let (isSending, onStartSending, onEndSending) = useDisclosure();
  <div className=TW.([Padding(Py2)] |> make)>
    <CreateSurveyMutation>
      ...{(mutate, {result}) => {
        <>
          <Button
            loading={Button.LoadingProp.Bool(result == Loading || isSending)}
            onClick={_ => onOpenedModal()}
            _type=`primary>
            {"Create new survey" |> ReasonReact.string}
          </Button>
          <Modal
            closable=true
            visible=isOpenModal
            onOk={_ => {
              onClosedModal();
              onStartSending();

              let sendings =
                squadIds
                |> List.map(squadId => {
                     let variables =
                       CreateSurveyConfig.make(~squadId, ())##variables;

                     mutate(~variables, ~refetchQueries=[|"allSurveys"|], ())
                     |> Js.Promise.then_(res => {
                          switch (res) {
                          | Errors(_)
                          | EmptyResponse =>
                            Js.Result.Error("error") |> Js.Promise.resolve
                          | Data(_) =>
                            Js.Result.Ok("ok") |> Js.Promise.resolve
                          }
                        });
                   });
              ();
              Js.Promise.all(sendings |> Array.of_list)
              |> Js.Promise.then_(res => {
                   Notification.success(
                     Notification.makeConfigProps(
                       ~message="Create new surveys successfully!",
                       (),
                     ),
                   )
                   |> ignore;

                   Js.Promise.resolve();
                 });
              ();
            }}
            onCancel={_ => onClosedModal()}>
            {(
               isCreatedSuvey
                 ? "Are you sure you want to create more surveys?"
                 : "Create survey and send email to all squads?"
             )
             |> React.string}
          </Modal>
        </>
      }}
    </CreateSurveyMutation>
  </div>;
};
