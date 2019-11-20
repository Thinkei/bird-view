[@react.component]
let make = () => {
  <div
    className=TW.(
      [Margin(My2), Margin(Mx2), TextAlign(TextCenter)] |> make
    )>
    {ReasonReact.string("Loading...")}
  </div>;
};
