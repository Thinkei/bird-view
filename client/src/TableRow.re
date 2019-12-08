[@react.component]
let make = (~style=?, ~children) => {
  <tr ?style className=TW.([BackgroundColor(HoverBgGreen100)] |> make)>
    children
  </tr>;
};
