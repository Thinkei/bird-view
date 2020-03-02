let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

let httpLink =
  ApolloLinks.createHttpLink(
    ~uri=[%bs.raw {| process.env.GRAPHQL_SERVER_URI |}],
    (),
  );

let headerContextLink =
  ApolloLinks.createContextLink(() => {
    Session.(
      switch (getSessionFromStorage()) {
      | Invalid => {
          "headers": {
            "authorization": "",
          },
        }
      | Valid({token}) => {
          "headers": {
            "authorization": {j|Bearer $token|j},
          },
        }
      }
    )
  });

let instance =
  ReasonApollo.createApolloClient(
    ~link=ApolloLinks.from([|headerContextLink, httpLink|]),
    ~cache=inMemoryCache,
    (),
  );
