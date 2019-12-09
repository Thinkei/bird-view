[@react.component]
let make = (~children) => {
  open Session;
  let (token, setToken) = React.useState(() => getTokenFromStorage());
  Js.log(token);

  switch (token) {
  | Invalid =>
    <SignIn
      setToken={newToken => {
        setToken(_ => Valid(newToken));
        setTokenToStorage(newToken);
      }}
    />
  | Valid(_) => children
  };
};
