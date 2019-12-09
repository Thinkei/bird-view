let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

let httpLink =
  ApolloLinks.createHttpLink(
    ~uri="https://api.graph.cool/simple/v1/ck3swt8le9sal0119fx4dpb74",
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
