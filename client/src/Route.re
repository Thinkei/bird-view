module type Config = {
  type route;

  let toUrl: route => string;
  let toRoute: ReasonReact.Router.url => route;
};

module Config = {
  type route =
    | Home
    | Squad(string)
    | LatestSurveyDetail
    | SurveyDetail(string)
    | SurveyReport(string)
    | NotFound;

  let toRoute = (url: ReasonReact.Router.url) => {
    let hashes = url.hash |> Js.String.split("/") |> Array.to_list;
    switch (url.path) {
    | [""]
    | []
    | ["/"] =>
      switch (hashes) {
      | [""] => Home
      | ["", "squads", squadId] => Squad(squadId)
      | ["", "surveys", surveyId] => SurveyDetail(surveyId)
      | ["", "latest"] => LatestSurveyDetail
      | ["", "reports", surveyId] => SurveyReport(surveyId)
      | _ => NotFound
      }
    | _ => NotFound
    };
  };

  let toUrl =
    fun
    | Home => "#"
    | Squad(id) => "#/squads/" ++ id
    | SurveyDetail(id) => "#/surveys/" ++ id
    | LatestSurveyDetail => "#/latest"
    | SurveyReport(id) => "#/reports/" ++ id
    | NotFound => "#/404";
};

module Link = {
  [@react.component]
  let make = (~route, ~style=?, ~className=?, ~children) => {
    let href = Config.toUrl(route);
    let onClick = e => {
      ReactEvent.Mouse.preventDefault(e);
      ReasonReact.Router.push(href);
    };
    <a href onClick ?style ?className> children </a>;
  };
};
