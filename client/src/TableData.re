[@react.component]
let make = (~children) => {
  <td
    className=TW.(
      [
        Padding(Py4),
        Padding(Px6),
        BorderWidth(BorderB),
        BorderColor(BorderGray300),
      ]
      |> make
    )>
    children
  </td>;
};
