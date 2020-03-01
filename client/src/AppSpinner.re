[@react.component]
let make = () => {
  <div
    className=TW.(
      [Margin(My2), Margin(Mx2), TextAlign(TextCenter)] |> make
    )>
    <Chakra.Spinner
      thickness="4px"
      speed="0.65s"
      emptyColor=`gray200
      color=`blue500
      size=`xl
    />
  </div>;
};
