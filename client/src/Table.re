module type TableConfig = {
  type column;
  type rowData;

  let getColumnHeader: column => string;
  let render: (rowData, column) => React.element;

  let columns: list(column);
};

module Make = (TC: TableConfig) => {
  let str = ReasonReact.string;

  [@react.component]
  let make = (~data) => {
    <table
      className=TW.(
        [TextAlign(TextLeft), Width(WFull), BorderCollapse(BorderCollapse)]
        |> make
      )>
      <thead>
        <tr>
          {TC.columns
           |> List.map(column =>
                <th
                  className=TW.(
                    [
                      Padding(Py4),
                      Padding(Px6),
                      BackgroundColor(BgGray300),
                      FontWeight(FontBold),
                      TextTransform(Uppercase),
                      FontSize(TextSm),
                      TextColor(TextGray900),
                      BorderWidth(BorderB),
                      BorderColor(BorderGray300),
                    ]
                    |> make
                  )>
                  {TC.getColumnHeader(column) |> str}
                </th>
              )
           |> Array.of_list
           |> ReasonReact.array}
        </tr>
      </thead>
      <tbody>
        {data
         |> List.map(rowData =>
              <tr className=TW.([BackgroundColor(HoverBgGreen100)] |> make)>
                {TC.columns
                 |> List.map(column =>
                      <td
                        className=TW.(
                          [
                            Padding(Py4),
                            Padding(Px6),
                            BorderWidth(BorderB),
                            BorderColor(BorderGray300),
                          ]
                          |> make
                        )>
                        {TC.render(rowData, column)}
                      </td>
                    )
                 |> Array.of_list
                 |> ReasonReact.array}
              </tr>
            )
         |> Array.of_list
         |> ReasonReact.array}
      </tbody>
    </table>;
  };
};
