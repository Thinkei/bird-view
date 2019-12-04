[@react.component]
let make = () => {
  <div
    className=TW.(
      [Margin(My2), Margin(Mx2), TextAlign(TextCenter)] |> make
    )>
    <img
      className=TW.([Margin(MxAuto)] |> make)
      src={Utils.requireURI("./loading.svg")}
    />
  </div>;
};
