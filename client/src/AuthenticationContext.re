let context = React.createContext("");

let getToken = () => React.useContext(context);

let make = React.Context.provider(context);

let makeProps = (~value, ~children, ()) => {
  "value": value,
  "children": children,
};
