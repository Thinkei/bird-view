let str = ReasonReact.string;

let optionStr =
  fun
  | None => str("")
  | Some(v) => str(v);

module TableConfig = {
  type rowData = {
    .
    "id": string,
    "content": string,
    "goodExample": string,
    "badExample": string,
  };

  type column =
    | Content
    | GoodExample
    | BadExample
    | Answer;

  let getColumnHeader =
    fun
    | Content => "Description"
    | GoodExample => "Good Example"
    | BadExample => "Bad Example"
    | Answer => "Answer";

  let render = data =>
    fun
    | Content => data##content |> str
    | GoodExample => data##goodExample |> str
    | BadExample => data##badExample |> str
    | Answer =>
      <Select>
        <SelectOption> {"Green" |> str} </SelectOption>
        <SelectOption> {"Yellow" |> str} </SelectOption>
        <SelectOption> {"Red" |> str} </SelectOption>
      </Select>;

  let columns = [Content, GoodExample, BadExample, Answer];
};

module QuestionsTable = Table.Make(TableConfig);

[@react.component]
let make = (~name, ~questions) => {
  Js.log(questions);
  <div>
    <Headline>
      <p>
        <span> {ReasonReact.string("Answer for birdview: ")} </span>
        <strong> {ReasonReact.string(name)} </strong>
      </p>
    </Headline>
    <QuestionsTable
      data={
        switch (questions) {
        | None => []
        | Some(qs) => qs |> Array.to_list
        }
      }
    />
  </div>;
};
