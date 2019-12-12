[@bs.module "antd/lib/popconfirm"] [@react.component]
external make:
  (
    ~title: React.element=?,
    ~onConfirm: ReactEvent.Mouse.t => unit=?,
    ~onCancel: ReactEvent.Mouse.t => unit=?,
    ~okText: React.element=?,
    ~okType: [@bs.string] [ | `primary | `ghost | `dashed | `danger]=?,
    ~cancelText: React.element=?,
    ~onVisibleChange: bool => unit=?,
    ~children: React.element,
  ) =>
  React.element =
  "default";
