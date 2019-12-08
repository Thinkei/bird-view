[@react.component]
let make = (~value, ~options, ~onChange) => {
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

  <select value className onChange>
    {options
     |> List.map(((value, label)) =>
          <option key=value value> {label |> ReasonReact.string} </option>
        )
     |> Array.of_list
     |> ReasonReact.array}
  </select>;
};
