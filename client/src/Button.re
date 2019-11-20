[@react.component]
let make = (~onClick, ~children) => {
  <button
    onClick
    className=TW.(
      [
        BackgroundColor(BgBlue500),
        BackgroundColor(HoverBgBlue700),
        TextColor(TextWhite),
        Padding(Py2),
        Padding(Px4),
        BorderRadius(Rounded),
      ]
      |> make
    )>
    children
  </button>;
};
