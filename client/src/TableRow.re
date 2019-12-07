[@react.component]
let make = (~children) => {
  <tr className=TW.([BackgroundColor(HoverBgGreen100)] |> make)>
    children
  </tr>;
};
