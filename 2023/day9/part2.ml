open! Core
open Stdio

let rec differences = function
| [] | [_] -> []
| n1 :: n2 :: tl -> n2-n1 :: (differences (n2 :: tl))


let rec calc_line nums =
  if List.is_empty (List.filter nums ~f:(fun x -> x <> 0)) then 0 else 
  let diff_list = differences nums in
  let diff = calc_line diff_list in
  let sub_from = List.hd_exn nums in
  sub_from - diff

let format_input lines = 
  List.map lines 
  ~f:(fun s -> String.split s ~on:' ' |> List.map ~f:(fun s -> Int.of_string s))

let () = 
format_input (In_channel.input_lines stdin)
|> List.map ~f:(fun line -> calc_line line)
|> List.sum (module Int) ~f:Fn.id
|> Int.to_string
|> Out_channel.print_endline;