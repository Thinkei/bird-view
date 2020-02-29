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
  let onClose = () => setter(_ => false);
  let onOpen = () => setter(_ => true);
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
            {"Send new bird view" |> ReasonReact.string}
          </Button>
          <Modal
            closable=false
            visible=isOpenModal
            okType={isCreatedSuvey ? `danger : `primary}
            onOk={_ => {
              onClosedModal();
              onStartSending();

              let sendings =
                squadIds
                |> List.mapi((index, squadId) => {
                     let variables =
                       CreateSurveyConfig.make(~squadId, ())##variables;

                     mutate(
                       ~variables,
                       ~refetchQueries=
                         index + 1 == List.length(squadIds)
                           ? [|"allSquads"|] : [||],
                       (),
                     )
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
              |> Js.Promise.then_(_ => {
                   onCreatedSurvey();
                   onEndSending();
                   Notification.success(
                     Notification.makeConfigProps(
                       ~message="Create new bird view successfully!",
                       (),
                     ),
                   )
                   |> ignore;
                   Js.Promise.resolve();
                 })
              |> ignore;
              ();
            }}
            onCancel={_ => onClosedModal()}>
            {(
               isCreatedSuvey
                 ? "You just created one. Are you sure you want to create another?"
                 : "Create new bird view and send email to all squads?"
             )
             |> React.string}
          </Modal>
        </>
      }}
    </CreateSurveyMutation>
  </div>;
};
