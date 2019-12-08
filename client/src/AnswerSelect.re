[@react.component]
let make = (~surveyId, ~cardId, ~answerRecord) => {
  switch (answerRecord) {
  | None =>
    <CreateSelect
      surveyId
      cardId
    />
  | Some(record) =>
    <UpdateSelect
      answerRecordId={record##id}
      answerRecordAnswer={record##answer}
    />
  };
};
