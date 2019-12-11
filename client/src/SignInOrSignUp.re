open Ehd;

module TabPane = Tabs.TabPane;

let str = ReasonReact.string;

[@react.component]
let make = (~setSession) => {
  <div style={ReactDOMRe.Style.make(~maxWidth="600px", ~margin="auto", ())}>
    <Tabs tabPosition=`top>
      <TabPane tab="Sign In" key="1"> <SignIn setSession /> </TabPane>
      <TabPane tab="Sign Up" key="2"> <SignUpFlow setSession /> </TabPane>
    </Tabs>
  </div>;
};
