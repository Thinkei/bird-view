let str = ReasonReact.string;

let (|?) = (x, y) =>
  switch (x) {
  | None => y
  | Some(x) => x
  };

type user = {
  id: string,
  email: option(string),
  name: option(string),
};

[@bs.deriving jsConverter]
type answer = [
  | [@bs.as "Green"] `Green
  | [@bs.as "Yellow"] `Yellow
  | [@bs.as "Red"] `Red
];

type answerRecord = {
  answer,
  user,
};

type card = {
  id: string,
  description: string,
  answerRecords: option(array(answerRecord)),
  goodExample: string,
  badExample: string,
};

module QueryConfig = [%graphql
  {|
  query cardsAndUsers($surveyId: ID!, $squadId: ID!) {
    allCards @bsRecord {
      id
      description
      goodExample
      badExample
      answerRecords(filter: { survey: { id: $surveyId }}) @bsRecord {
        answer
        user @bsRecord {
          id
          email
          name
        }
      }
    }
    allUsers(filter: { squad: { id: $squadId }}) @bsRecord {
      id
      email
      name
    }
  }

|}
];

module Query = ReasonApolloHooks.Query.Make(QueryConfig);

let extractEmailRegex = [%bs.re "/^[^@]+/"];

let matchFirst = (str, regex) => {
  let matches = str |? "" |> Js.String.match(regex);
  (matches |? [||])->Array.get(0);
};

let rec findAnswer = (answerRecords, userId) => {
  switch (answerRecords) {
  | [] => None
  | [head, ...tail] =>
    head.user.id == userId ? Some(head.answer) : findAnswer(tail, userId)
  };
};

module ReportTable = {
  [@react.component]
  let make = (~cards, ~users) => {
    let emailHandlers =
      users |> Array.map(user => user.email->matchFirst(extractEmailRegex));
    <div>
      <Route.Link
        style={ReactDOMRe.Style.make(
          ~position="absolute",
          ~top="10px",
          ~left="10px",
          (),
        )}
        route=Route.Config.(Home)>
        <Ehd.Button> {"View all surveys" |> React.string} </Ehd.Button>
      </Route.Link>
      <Headline> {str("SURVEY REPORT")} </Headline>
      <Table>
        <thead>
          <TableRow>
            <TableHeader> {"Card" |> str} </TableHeader>
            {emailHandlers
             |> Array.map(handler => {
                  <TableHeader> {handler |> str} </TableHeader>
                })
             |> ReasonReact.array}
          </TableRow>
        </thead>
        <tbody>
          {cards
           |> Array.map(card => {
                <TableRow>
                  <TableData>
                    <Ehd.Tooltip
                      title={
                        <div>
                          <p>
                            <strong> {"Example of Awesome: " |> str} </strong>
                            <span> {card.goodExample |> str} </span>
                          </p>
                          <p>
                            <strong> {"Example of Crappy: " |> str} </strong>
                            <span> {card.badExample |> str} </span>
                          </p>
                        </div>
                      }>
                      <strong> {str(card.description)} </strong>
                    </Ehd.Tooltip>
                  </TableData>
                  {users
                   |> Array.map((user: user) => {
                        let answer =
                          findAnswer(
                            card.answerRecords |? [||] |> Array.to_list,
                            user.id,
                          );
                        let style =
                          switch (answer) {
                          | Some(`Red) =>
                            ReactDOMRe.Style.make(
                              ~backgroundColor="#ffc4c4",
                              (),
                            )
                          | Some(`Green) =>
                            ReactDOMRe.Style.make(
                              ~backgroundColor="#98e998",
                              (),
                            )
                          | Some(`Yellow) =>
                            ReactDOMRe.Style.make(
                              ~backgroundColor="#efef9b",
                              (),
                            )
                          | None => ReactDOMRe.Style.make()
                          };
                        <TableData style>
                          {answer->Belt.Option.mapWithDefault(
                             "Not yet answer",
                             answerToJs,
                           )
                           |> str}
                        </TableData>;
                      })
                   |> ReasonReact.array}
                </TableRow>
              })
           |> ReasonReact.array}
        </tbody>
      </Table>
    </div>;
  };
};

[@react.component]
let make = (~id, ~session) => {
  Session.(
    switch (session.squadId) {
    | Some(squadId) =>
      let variables = QueryConfig.make(~squadId, ~surveyId=id, ())##variables;
      let (queryState, _full) = Query.use(~variables, ());

      switch (queryState) {
      | Loading => <Spinner />
      | Data(data) =>
        <ReportTable users=data##allUsers cards=data##allCards />
      | NoData => <EmptyData />
      | Error(e) => <FriendlyError message=e##message />
      };
    | None => <FriendlyError message="You doesn't belong to any squad" />
    }
  );
};
