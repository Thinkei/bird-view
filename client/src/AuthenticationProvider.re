[@react.component]
let make = (~children) => {
  open Session;
  let (session, setSession) = React.useState(() => getSessionFromStorage());

  switch (session) {
  | Invalid =>
    <SignIn
      setSesion={session => {
        setSession(_ => Valid(session));
        setSessionToStorage(session);
      }}
    />
  | Valid(session) => children(session)
  };
};
