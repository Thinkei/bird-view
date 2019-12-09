[@bs.val] external requireURI: string => string = "require";
[@bs.val] external require: string => unit = "require";

[@bs.val] [@bs.scope "location"] external refreshPage: unit => unit = "reload";

let formatDate = str => str |> Js.Date.fromString |> Js.Date.toLocaleString;
