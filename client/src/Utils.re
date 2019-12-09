[@bs.val] external requireURI: string => string = "require";
[@bs.val] external require: string => unit = "require";

let formatDate = str => str |> Js.Date.fromString |> Js.Date.toLocaleString;
