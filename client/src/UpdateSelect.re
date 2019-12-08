open GraphTypes;
let str = ReasonReact.string;

module UpdateConfig = [%graphql
  {|
  mutation updateAnswerRecord($id: ID!, $answer: AnswerOption!) {
    updateAnswerRecord(id: $id, answer: $answer) {
      id
      answer
    }
  }
|}
];

module UpdateMutation = ReasonApollo.CreateMutation(UpdateConfig);

[@react.component]
let make = (~answerRecordId, ~answerRecordAnswer) => {
  let (answerColor, setAnswerColor) =
    React.useState(() => answerRecordAnswer);

  <UpdateMutation>
    ...{(mutate, _) => {
      <Select
        value={answerToJs(answerColor)}
        options=[
          (answerToJs(`Green), answerToJs(`Green)),
          (answerToJs(`Yellow), answerToJs(`Yellow)),
          (answerToJs(`Red), answerToJs(`Red)),
        ]
        onChange={e => {
          let answer =
            switch (e->ReactEvent.Form.target##value) {
            | "Green" => `Green
            | "Yellow" => `Yellow
            | "Red" => `Red
            | _ => `Green
            };
          setAnswerColor(_ => answer);

          let updateVariables =
            UpdateConfig.make(~id=answerRecordId, ~answer, ())##variables;

          mutate(~variables=updateVariables, ())
          |> Js.Promise.then_(_ => {Js.Promise.resolve()})
          |> ignore;
          ();
        }}
      />
    }}
  </UpdateMutation>;
};
