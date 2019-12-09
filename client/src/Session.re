let squadId = "ck3sxea970hi20112bpqx6xp0";
let userId = "ck3wiv08f08zq0170zt011iup";

let tokenKey = "__TOKEN__";

type token =
  | Invalid
  | Valid(string);

let setTokenToStorage = newToken => {
  LocalStorage.setItem(tokenKey, newToken);
};

let getTokenFromStorage = () => {
  let token = LocalStorage.getItem(tokenKey) |> Js.Nullable.toOption;
  switch (token) {
  | None => Invalid
  | Some(v) => Valid(v)
  };
};
