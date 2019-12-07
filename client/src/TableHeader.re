[@react.component]
let make = (~children) => {
  <th
    className=TW.(
      [
        Padding(Py4),
        Padding(Px6),
        BackgroundColor(BgGray300),
        FontWeight(FontBold),
        TextTransform(Uppercase),
        FontSize(TextSm),
        TextColor(TextGray900),
        BorderWidth(BorderB),
        BorderColor(BorderGray300),
      ]
      |> make
    )>
    children
  </th>;
};
