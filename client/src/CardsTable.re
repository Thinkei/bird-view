let str = ReasonReact.string;

let optionStr =
  fun
  | None => str("")
  | Some(v) => str(v);

module TableConfig = {
  type rowData = {
    .
    "id": string,
    "description": string,
    "goodExample": string,
    "badExample": string,
  };

  type column =
    | Description
    | GoodExample
    | BadExample
    | Answer;

  let getColumnHeader =
    fun
    | Description => "Description"
    | GoodExample => "Good Example"
    | BadExample => "Bad Example"
    | Answer => "Answer";

  let render = data =>
    fun
    | Description => data##description |> str
    | GoodExample => data##goodExample |> str
    | BadExample => data##badExample |> str
    | Answer =>
      <Select>
        <SelectOption> {"Green" |> str} </SelectOption>
        <SelectOption> {"Yellow" |> str} </SelectOption>
        <SelectOption> {"Red" |> str} </SelectOption>
      </Select>;

  let columns = [Description, GoodExample, BadExample, Answer];
};

module CardsTable = Table.Make(TableConfig);

[@react.component]
let make = (~data) => <CardsTable data />;
