open Ehd;

[@react.component]
let make = (~children) => {
  <Typography.Title
    level=1
    style={ReactDOMRe.Style.make(~marginTop="20px", ~textAlign="center", ())}>
    children
  </Typography.Title>;
};
