open Chakra;
let str = ReasonReact.string;

[@react.component]
let make = (~setSession) => {
  <div style={ReactDOMRe.Style.make(~maxWidth="600px", ~margin="auto", ())}>
    <Tabs>
      <div className=TW.([Height(H12)] |> make)>
        <TabList>
          <Tab> {"Sign In" |> React.string} </Tab>
          <Tab> {"Sign Up" |> React.string} </Tab>
        </TabList>
      </div>
      <TabPanels>
        <TabPanel> <SignIn setSession /> </TabPanel>
        <TabPanel> <SignUpFlow setSession /> </TabPanel>
      </TabPanels>
    </Tabs>
  </div>;
};
