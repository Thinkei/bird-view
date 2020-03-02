open Chakra;

[@react.component]
let make = (~title, ~children) => {
  <div
    style={ReactDOMRe.Style.make(
      ~boxShadow=
        "rgba(0, 0, 0, 0.1) 0px 4px 6px -1px, rgba(0, 0, 0, 0.06) 0px 2px 4px -1px",
      ~marginTop="2rem",
      ~padding="1.25rem",
      ~borderWidth="1px",
      ~flex="1 1 0%",
      ~borderRadius="0.25rem",
      (),
    )}>
    <Heading fontSize="xl"> {title |> React.string} </Heading>
    <br />
    children
  </div>;
};
