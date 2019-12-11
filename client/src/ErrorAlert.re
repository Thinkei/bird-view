open Ehd;

[@react.component]
let make = (~message) =>
  <Alert
    style={ReactDOMRe.Style.make(~width="100%", ~marginTop="20px", ())}
    message={message |> React.string}
    _type=`error
    closable=true
  />;
