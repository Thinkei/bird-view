[@bs.val] [@bs.scope "localStorage"]
external setItem: (string, string) => unit = "setItem";
[@bs.val] [@bs.scope "localStorage"]
external getItem: string => Js.Nullable.t(string) = "getItem";
