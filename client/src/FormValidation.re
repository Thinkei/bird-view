let nonEmptyString = value => value != "";

let validEHEmail = str =>
  Js.Re.fromString("^[^@\\s\\:]+@employmenthero\\.com$")
  |> Js.Re.exec(str)
  |> (
    fun
    | Some(_) => true
    | None => false
  );
