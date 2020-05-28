[%bs.raw {|require("tailwindcss/dist/tailwind.min.css")|}];
open Session;

module AppRouter = Router.Create(Route.Config);

let str = ReasonReact.string;

[@react.component]
let make = () =>
  <div className=TW.([Padding(Px2)] |> make)>
    <AppRouter>
      {currentRoute =>
         <AuthenticationProvider>
           {session => {
              <div>
                <div
                  style={ReactDOMRe.Style.make(
                    ~position="absolute",
                    ~top="5px",
                    ~right="10px",
                    (),
                  )}>
                  <Chakra.Button
                    size=`sm
                    variant=`outline
                    variantColor=`red
                    onClick={_ => {
                      Session.clearSession();
                      Utils.refreshPage();
                    }}>
                    {str("Sign out")}
                  </Chakra.Button>
                </div>
                {Route.Config.(
                   switch (currentRoute) {
                   | Home =>
                     switch (session.role) {
                     | Some(`Admin) => <SquadsList />
                     | _ =>
                       <SurveysList
                         squadId={session.squadId}
                         role={session.role}
                       />
                     }
                   | Squad(squadId) =>
                     switch (session.role) {
                     | Some(`Leader)
                     | Some(`Admin) =>
                       <SurveysList
                         squadId={squadId->Some}
                         role={session.role}
                       />

                     | _ => <FriendlyError message="Unauthorized" />
                     }

                   | SurveyDetail(id) => <SurveyDetail id session />
                   | LatestSurveyDetail => <LatestSurveyDetail session />
                   | SurveyReport(id) =>
                     switch (session.role) {
                     | Some(`Leader)
                     | Some(`Admin) => <SurveyReport id session />
                     | _ => <FriendlyError message="Unauthorized" />
                     }
                   | NotFound =>
                     <div> {ReasonReact.string("Not Found")} </div>
                   }
                 )}
              </div>;
            }}
         </AuthenticationProvider>}
    </AppRouter>
  </div>;
