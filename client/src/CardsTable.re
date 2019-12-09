let str = ReasonReact.string;

module QueryConfig = [%graphql
  {|
    query allAnswerRecords($surveyId: ID, $userId: ID) {
      allAnswerRecords(filter: { survey: { id: $surveyId}, user: { id: $userId }}) {
        id
        answer
        card { id }
      }
    }
  |}
];

let rec findAnswerRecord = cardId =>
  fun
  | [] => None
  | [h, ...tail] =>
    h##card##id == cardId ? Some(h) : findAnswerRecord(cardId, tail);

module RenderByTable = {
  [@react.component]
  let make = (~data, ~answerRecords, ~surveyId, ~userId) => {
    <div>
      <Headline>
        <span> {str("ANSWER SHEET: ")} </span>
        <span> {List.length(answerRecords) |> string_of_int |> str} </span>
        <span> {"/" |> str} </span>
        <span> {List.length(data) |> string_of_int |> str} </span>
      </Headline>
      <Table>
        <thead>
          <TableRow>
            <TableHeader> {"Description" |> str} </TableHeader>
            <TableHeader> {"Example of Awesome" |> str} </TableHeader>
            <TableHeader> {"Example of Crappy" |> str} </TableHeader>
            <TableHeader> {"Your answer" |> str} </TableHeader>
          </TableRow>
        </thead>
        <tbody>
          {data
           |> List.map(rowData => {
                let answerRecord =
                  findAnswerRecord(rowData##id, answerRecords);
                let style =
                  switch (answerRecord) {
                  | Some(_) => ReactDOMRe.Style.make()
                  | None =>
                    ReactDOMRe.Style.make(~backgroundColor="#fef1f1", ())
                  };
                <TableRow style>
                  <TableData> {rowData##description |> str} </TableData>
                  <TableData> {rowData##goodExample |> str} </TableData>
                  <TableData> {rowData##badExample |> str} </TableData>
                  <TableData>
                    <AnswerSelect surveyId answerRecord cardId=rowData##id userId/>
                  </TableData>
                </TableRow>;
              })
           |> Array.of_list
           |> ReasonReact.array}
        </tbody>
      </Table>
    </div>;
  };
};

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

[@react.component]
let make = (~data, ~surveyId, ~userId) => {
  let variables =
    QueryConfig.make(~surveyId, ~userId, ())##variables;
  let (queryState, _full) = Query.use(~variables, ());

  switch (queryState) {
  | Loading => <Spinner />
  | Data(res) =>
    <RenderByTable
      surveyId
      data
      userId
      answerRecords={res##allAnswerRecords |> Array.to_list}
    />
  | NoData => <RenderByTable surveyId data userId answerRecords=[] />
  | Error(e) => <FriendlyError message=e##message />
  };
};
