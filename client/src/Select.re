[@react.component]
let make = (~children) => {
  <select
    className=TW.(
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
    )>
    children
  </select>;
};
