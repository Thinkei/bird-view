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
      <Ehd.Switch
        checkedChildren={str("Yes")}
        unCheckedChildren={str("No")}
        defaultChecked=enabled
        onChange={(value, _) => {
          let variables =
            Config.make(~id=surveyId, ~enabled=value, ())##variables;
          mutate(~variables, ~refetchQueries=[|"allSurveys"|], ()) |> ignore;
        }}
      />
    }}
  </Mutation>;
