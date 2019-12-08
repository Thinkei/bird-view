open GraphTypes;
let str = ReasonReact.string;

module CreateConfig = [%graphql
  {|
  mutation createAnswerRecord($cardId: ID!, $memberId: ID!, $surveyId: ID!, $answer: AnswerOption!) {
    createAnswerRecord(cardId: $cardId, memberId: $memberId, surveyId: $surveyId, answer: $answer) {
      id
      answer
      card { id }
    }
  }
|}
];

module CreateMutation = ReasonApollo.CreateMutation(CreateConfig);

[@react.component]
let make = (~surveyId, ~cardId) => {
  <CreateMutation>
    ...{(mutate, _) => {
      <Select
        value=""
        options=[
          ("", ""),
          (answerToJs(`Green), answerToJs(`Green)),
          (answerToJs(`Yellow), answerToJs(`Yellow)),
          (answerToJs(`Red), answerToJs(`Red)),
        ]
        onChange={e => {
          let answer =
            switch (e->ReactEvent.Form.target##value) {
            | "Green" => Some(`Green)
            | "Yellow" => Some(`Yellow)
            | "Red" => Some(`Red)
            | _ => None
            };
          ();

          switch (answer) {
          | None => ()
          | Some(answer) =>
            let createVariables =
              CreateConfig.make(
                ~cardId,
                ~surveyId,
                ~memberId=Session.memberId,
                ~answer,
                (),
              )##variables;

            mutate(
              ~variables=createVariables,
              ~refetchQueries=[|"allAnswerRecords"|],
              (),
            )
            |> Js.Promise.then_(_ => {Js.Promise.resolve()})
            |> ignore;
          };
        }}
      />
    }}
  </CreateMutation>;
};
