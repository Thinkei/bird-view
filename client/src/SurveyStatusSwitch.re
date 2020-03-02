let str = React.string;

module Config = [%graphql
  {|
  mutation updateSurvey($id: ID!, $enabled: Boolean!) {
    updateSurvey(id: $id, enabled: $enabled) {
      id
    }
  }
|}
];

module Mutation = ReasonApollo.CreateMutation(Config);

[@react.component]
let make = (~surveyId, ~enabled) =>
  <Mutation>
    ...{(mutate, _) => {
      <Chakra.Switch
        defaultIsChecked=enabled
        onChange={e => {
          let variables =
            Config.make(
              ~id=surveyId,
              ~enabled=e->ReactEvent.Form.target##checked,
              (),
            )##variables;
          mutate(~variables, ~refetchQueries=[|"allSurveys"|], ()) |> ignore;
        }}
      />
    }}
  </Mutation>;
