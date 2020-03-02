module Styles = {
  open Css;
  let wrapper = [20 |> px |> marginBottom] |> style;
  let h3 = [blue |> color] |> style;
};

[@react.component]
let make = (~title, ~children) =>
  <section className=Styles.wrapper>
    <h3 className=Styles.h3> {ReasonReact.string(title)} </h3>
    children
  </section>;
