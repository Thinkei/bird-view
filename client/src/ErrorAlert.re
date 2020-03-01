[@react.component]
let make = (~message) =>
  <div style={ReactDOMRe.Style.make(~width="100%", ~marginTop="20px", ())}>
    <Chakra.Alert status=`error>
      <Chakra.AlertIcon />
      message->React.string
    </Chakra.Alert>
  </div>;
