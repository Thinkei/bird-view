[%bs.raw {|require("tailwindcss/dist/tailwind.min.css")|}];
[%bs.raw {|require("@ehrocks/eh-ant-design/styles/eh-ant-design.css")|}];
[%bs.raw {|require("antd/dist/antd.css")|}];

open Ehd;
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
                    ~top="10px",
                    ~right="10px",
                    (),
                  )}>
                  <Button
                    ghost=true
                    _type=`danger
                    onClick={_ => {
                      Session.clearSession();
                      Utils.refreshPage();
                    }}>
                    {str("Sign out")}
                  </Button>
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
