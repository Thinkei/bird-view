let tokenKey = "__TOKEN__";
let roleKey = "__ROLE__";
let userIdKey = "__USER_ID__";
let squadIdKey = "__SQUAD_ID__";

[@bs.deriving jsConverter]
type role = [
  | [@bs.as "Member"] `Member
  | [@bs.as "Leader"] `Leader
  | [@bs.as "Admin"] `Admin
];

type session = {
  token: string,
  role: option(role),
  userId: string,
  squadId: option(string),
};

type token =
  | Invalid
  | Valid(session);

let setSessionToStorage = session => {
  LocalStorage.setItem(tokenKey, session.token->Some);

  switch (session.role) {
  | Some(role) => LocalStorage.setItem(roleKey, Some(role |> roleToJs))
  | None => ()
  };
  LocalStorage.setItem(userIdKey, session.userId->Some);
  switch (session.squadId) {
  | Some(squadId) => LocalStorage.setItem(squadIdKey, squadId->Some)
  | None => ()
  };
};

let getSessionFromStorage = () => {
  let token = LocalStorage.getItem(tokenKey) |> Js.Nullable.toOption;
  let role = LocalStorage.getItem(roleKey) |> Js.Nullable.toOption;
  let userId = LocalStorage.getItem(userIdKey) |> Js.Nullable.toOption;
  let squadId = LocalStorage.getItem(squadIdKey) |> Js.Nullable.toOption;

  switch (token, role, userId, squadId) {
  | (Some(token), roleAsString, Some(userId), squadId) =>
    Valid({
      token,
      role:
        switch (roleAsString) {
        | Some("Leader") => Some(`Leader)
        | Some("Admin") => Some(`Admin)
        | _ => Some(`Member)
        },
      userId,
      squadId,
    })
  | _ => Invalid
  };
};

let clearSession = () => {
  [tokenKey, roleKey, userIdKey, squadIdKey]
  |> List.map(LocalStorage.removeItem)
  |> ignore;
};
