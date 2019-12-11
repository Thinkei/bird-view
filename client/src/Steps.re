Utils.require("antd/lib/steps/style");

[@bs.module "antd/lib/steps"] [@react.component]
external make:
  (
    ~direction: [@bs.string] [ | `horizontal | `vertical]=?,
    ~progressDot: bool=?,
    ~size: [@bs.string] [ | `default | `small]=?,
    ~status: [@bs.string] [ | `wait | `process | `finish | `error]=?,
    ~current: int=?,
    ~style: ReactDOMRe.Style.t=?,
    ~children: React.element=?
  ) =>
  React.element =
  "default";

module Step = {
  [@bs.scope "default"] [@bs.module "antd/lib/steps"] [@react.component]
  external make:
    (~title: string=?, ~description: string=?, ~children: React.element=?) =>
    React.element =
    "Step";
};
