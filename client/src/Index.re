let client = Client.instance;

ReactDOMRe.renderToElementWithId(
  <Chakra.ThemeProvider>
    <Chakra.CSSReset />
    <ReasonApollo.Provider client>
      <ReasonApolloHooks.ApolloProvider client>
        <App />
      </ReasonApolloHooks.ApolloProvider>
    </ReasonApollo.Provider>
  </Chakra.ThemeProvider>,
  "root",
);
