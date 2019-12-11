[@react.component]
let make = (~children) => {
  open Session;
  let (session, setSession) = React.useState(() => getSessionFromStorage());

  switch (session) {
  | Invalid =>
    <SignInOrSignUp
      setSession={session => {
        setSession(_ => Valid(session));
        setSessionToStorage(session);
        Utils.refreshPage();
      }}
    />
  | Valid(session) => children(session)
  };
};
