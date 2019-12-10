[@react.component]
let make = (~children, ~style=?) => {
  <td
    ?style
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
