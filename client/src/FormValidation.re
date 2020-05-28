let nonEmptyString = value => value != "";

let validEHEmail = str =>
  Js.Re.fromString("^[^@\\s\\:]+@employmenthero\\.com$")
  |> Js.Re.exec_(_,str)
  |> (
    fun
    | Some(_) => true
    | None => false
  );
