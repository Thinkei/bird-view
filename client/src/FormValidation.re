
let nonEmptyString = value => value == "";

let validEmail = str =>
  Js.Re.fromString("^[^@\\s]+@([^@\\s]+\\.)+[^@\\W]+$")
  |> Js.Re.exec(str)
  |> (
    fun
    | Some(_) => true
    | None => false
  );
