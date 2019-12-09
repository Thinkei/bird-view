[%bs.raw {|require("tailwindcss/dist/tailwind.min.css")|}];
[%bs.raw {|require("@ehrocks/eh-ant-design/styles/eh-ant-design.css")|}];

module AppRouter = Router.Create(Route.Config);

let str = ReasonReact.string;

[@react.component]
let make = () =>
  <AppRouter>
    {currentRoute =>
       <AuthenticationProvider>
         {Route.Config.(
            switch (currentRoute) {
            | Home => <Home />
            | SurveyDetail(id) => <SurveyDetail id />
            | NotFound => <div> {ReasonReact.string("Not Found")} </div>
            }
          )}
       </AuthenticationProvider>}
  </AppRouter>;
