[%bs.raw {|require("tailwindcss/dist/tailwind.min.css")|}];
[%bs.raw {|require("@ehrocks/eh-ant-design/styles/eh-ant-design.css")|}];

module AppRouter = Router.Create(Route.Config);

let str = ReasonReact.string;

[@react.component]
let make = () =>
  <div className=TW.([Padding(Px2)] |> make)>
    <AppRouter>
      {currentRoute =>
         <AuthenticationProvider>
           {session => {
              Route.Config.(
                switch (currentRoute) {
                | Home => <SurveysList session />
                | SurveyDetail(id) => <SurveyDetail id session />
                | NotFound => <div> {ReasonReact.string("Not Found")} </div>
                }
              );
            }}
         </AuthenticationProvider>}
    </AppRouter>
  </div>;
