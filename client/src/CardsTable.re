let str = ReasonReact.string;

module QueryConfig = [%graphql
  {|
    query allAnswerRecords($surveyId: ID, $memberId: ID) {
      allAnswerRecords(filter: { survey: { id: $surveyId}, member: { id: $memberId }}) {
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
  let make = (~data, ~answerRecords) => {
    Js.log(answerRecords);
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
         |> List.map(rowData =>
              <TableRow>
                <TableData> {rowData##description |> str} </TableData>
                <TableData> {rowData##goodExample |> str} </TableData>
                <TableData> {rowData##badExample |> str} </TableData>
                <TableData>
                  <AnswerSelect
                    answerRecord={findAnswerRecord(
                      rowData##id,
                      answerRecords,
                    )}
                    cardId=rowData##id
                  />
                </TableData>
              </TableRow>
            )
         |> Array.of_list
         |> ReasonReact.array}
      </tbody>
    </Table>;
  };
};

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

[@react.component]
let make = (~data, ~surveyId) => {
  let variables =
    QueryConfig.make(~surveyId, ~memberId=Session.memberId, ())##variables;
  let (queryState, _full) = Query.use(~variables, ());

  switch (queryState) {
  | Loading => <Spinner />
  | Data(res) =>
    <RenderByTable
      data
      answerRecords={res##allAnswerRecords |> Array.to_list}
    />
  | NoData => <RenderByTable data answerRecords=[] />
  | Error(e) => <FriendlyError message=e##message />
  };
};
