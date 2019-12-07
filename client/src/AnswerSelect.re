let str = ReasonReact.string;

[@react.component]
let make = (~cardId, ~answerRecord) => {
  let onChange = _ => ();
  let className =
    TW.(
      [
        Display(Block),
        AppearanceNone,
        Width(WFull),
        BackgroundColor(BgGray200),
        BorderWidth(Border),
        BorderColor(BorderGray200),
        TextColor(TextGray700),
        Padding(Py3),
        Padding(Px4),
        Padding(Pr8),
        BorderRadius(Rounded),
        LineHeight(LeadingTight),
        BackgroundColor(FocusBgWhite),
        BorderColor(FocusBorderGray500),
      ]
      |> make
    );
  <select
    className
    onChange={e =>
      (
        switch (e->ReactEvent.Form.target##value) {
        | "Green" => `Green
        | "Yellow" => `Yellow
        | "Red" => `Red
        | _ => `NotYetSpecified
        }
      )
      |> onChange
    }>
    <option> {"" |> str} </option>
    <option> {"Green" |> str} </option>
    <option> {"Yellow" |> str} </option>
    <option> {"Red" |> str} </option>
  </select>;
};
