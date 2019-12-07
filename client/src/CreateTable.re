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
    <Table>
      <thead>
        <TableRow>
          {TC.columns
           |> List.map(column =>
                <TableHeader>
                  {TC.getColumnHeader(column) |> str}
                </TableHeader>
              )
           |> Array.of_list
           |> ReasonReact.array}
        </TableRow>
      </thead>
      <tbody>
        {data
         |> List.map(rowData =>
              <TableRow>
                {TC.columns
                 |> List.map(column =>
                      <TableData> {TC.render(rowData, column)} </TableData>
                    )
                 |> Array.of_list
                 |> ReasonReact.array}
              </TableRow>
            )
         |> Array.of_list
         |> ReasonReact.array}
      </tbody>
    </Table>;
  };
};
