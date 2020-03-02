[@react.component]
let make = (~surveyId, ~cardId, ~userId, ~answerRecord) => {
  switch (answerRecord) {
  | None => <CreateSelect surveyId cardId userId />
  | Some(record) =>
    <UpdateSelect
      answerRecordId={record##id}
      answerRecordAnswer={record##answer}
    />
  };
};
