[%bs.raw {|require("tailwindcss/dist/tailwind.min.css")|}];
[%bs.raw {|require("@ehrocks/eh-ant-design/styles/eh-ant-design.css")|}];
[%bs.raw {|require("antd/dist/antd.css")|}];

open Ehd;

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
                   | Home => <SurveysList session />
                   | SurveyDetail(id) => <SurveyDetail id session />
                   | SurveyReport(id) =>
                     switch (session.role) {
                     | Some(`Leader) => <SurveyReport id session />
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
