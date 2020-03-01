open Chakra;

[@react.component]
let make = (~children) => {
  <Flex alignItems=`center justifyContent=`center>
    <div style={ReactDOMRe.Style.make(~margin="10px 0", ())}>
      <Text> children </Text>
    </div>
  </Flex>;
};
