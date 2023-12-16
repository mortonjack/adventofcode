open! Core
open Stdio
let hash s =
  String.to_list s
  |> List.fold ~init:0 ~f:(fun acc c ->
    let n = Char.to_int c in
    ((acc + n) * 17) % 256)

let format_input lines = 
  String.concat lines
  |> String.split ~on:','

let () =
In_channel.input_lines stdin
|> format_input
|> List.map ~f:hash
|> List.sum (module Int) ~f:Fn.id
|> Int.to_string 
|> Out_channel.print_endline