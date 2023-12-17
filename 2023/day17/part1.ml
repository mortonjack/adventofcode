open! Core
open Stdio
open Containers.Heap

let format_input lines = 
  List.map lines ~f:(fun line 
    -> String.to_list line
    |> List.map ~f:Char.to_string
    |> List.map ~f:Int.of_string
    |> List.to_array)
  |> List.to_array

let () =
In_channel.input_lines stdin
|> format_input
|> Int.to_string
|> Out_channel.print_endline