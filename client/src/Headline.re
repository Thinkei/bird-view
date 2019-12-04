[@react.component]
let make = (~children) => {
  <h2
    className=TW.(
      [TextAlign(TextCenter), Margin(My2), TextTransform(Uppercase)]
      |> make
    )>
    children
  </h2>;
};
