[@react.component]
let make = (~value, ~options, ~onChange) => {
  <Chakra.Select value onChange size=`sm>
    {options
     |> List.map(((value, label)) =>
          <option key=value value> {label |> ReasonReact.string} </option>
        )
     |> Array.of_list
     |> ReasonReact.array}
  </Chakra.Select>;
};
