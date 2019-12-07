let str = ReasonReact.string;

[@react.component]
let make = (~children) => {
  <table
    className=TW.(
      [TextAlign(TextLeft), Width(WFull), BorderCollapse(BorderCollapse)]
      |> make
    )>
    children
  </table>;
};
