[@react.component]
let make = (~name, ~questions) => {
  Js.log(questions);
  <div>
    <Headline>
      <p>
        <span> {ReasonReact.string("Answer for birdview: ")} </span>
        <strong> {ReasonReact.string(name)} </strong>
      </p>
    </Headline>
  </div>;
};
