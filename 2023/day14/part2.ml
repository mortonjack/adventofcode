open! Core
open Stdio

type rock = Rounded | Cube

type input = {
  height: int;
  rocks: (int * rock) list list
}

let char_to_rock = function
| '#' -> Some Cube
| 'O' -> Some Rounded
| _ -> None

let rows_to_cols rows = 
  List.fold rows ~init:(String.to_list (List.hd_exn rows) |> List.map ~f:(fun _ -> ""))
  ~f:(fun acc row ->
    List.mapi acc ~f:(fun i acc -> acc ^ Char.to_string (String.get row i)))

let format_input lines = 
  let height = List.length lines in
  let lines = rows_to_cols lines in
  let rocks = List.map lines ~f:(fun line -> String.to_list line
  |> List.mapi ~f:(fun h c -> (h, char_to_rock c)))
  |> List.map ~f:(List.filter_map ~f:(fun (h, rock_type) -> 
    match rock_type with
    | Some rock_type -> Some (h, rock_type)
    | None -> None))
  in { height; rocks }

let calc_load ~height rocks =
  let _, load = List.fold rocks ~init:(0, 0) ~f:(fun (next, load) (h, rock) -> 
    match rock with
    | Cube -> (h+1, load)
    | Rounded -> (next+1, load + (height - next)) )
  in load

let sum_load input =
  List.map input.rocks ~f:(calc_load ~height:(input.height))
  |> List.sum (module Int) ~f:Fn.id

let () =
In_channel.input_lines stdin
|> format_input
|> sum_load
|> Int.to_string
|> Out_channel.print_endline