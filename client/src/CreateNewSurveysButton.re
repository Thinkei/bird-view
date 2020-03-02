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
  let toast = Chakra.Toast.useToast();
  <div>
    <CreateSurveyMutation>
      ...{(mutate, {result}) => {
        <>
          <Chakra.Button
            variant=`ghost
            variantColor=`blue
            size=`sm
            isLoading={result == Loading || isSending}
            rightIcon=`email
            onClick={_ => onOpenedModal()}>
            {"Send new bird view" |> React.string}
          </Chakra.Button>
          <Chakra.Modal closeOnEsc=false isOpen=isOpenModal>
            <Chakra.ModalOverlay />
            <Chakra.ModalContent>
              <Chakra.ModalHeader>
                {(
                   isCreatedSuvey
                     ? "You just created one. Are you sure you want to create another?"
                     : "Create new bird view and send email to all squads?"
                 )
                 |> React.string}
              </Chakra.ModalHeader>
              <Chakra.ModalFooter>
                <Chakra.Button
                  size=`sm variant=`outline onClick={_ => {onClosedModal()}}>
                  "Close"->React.string
                </Chakra.Button>
                <div style={ReactDOMRe.Style.make(~marginLeft="10px", ())}>
                  <Chakra.Button
                    variantColor={isCreatedSuvey ? `red : `blue}
                    size=`sm
                    onClick={_ => {
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
                                    Js.Result.Error("error")
                                    |> Js.Promise.resolve
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

                           toast(
                             ~title="Kudos!",
                             ~description="Create new bird view successfully!",
                             ~position=`topRight,
                             ~status=`success,
                             (),
                           );
                           Js.Promise.resolve();
                         })
                      |> ignore;
                      ();
                    }}>
                    "OK"->React.string
                  </Chakra.Button>
                </div>
              </Chakra.ModalFooter>
            </Chakra.ModalContent>
          </Chakra.Modal>
        </>
      }}
    </CreateSurveyMutation>
  </div>;
};
