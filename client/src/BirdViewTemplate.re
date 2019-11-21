[@react.component]
let make = (~name) => {
  <div> {ReasonReact.string("Bird view name: " ++ name)} </div>;
};
