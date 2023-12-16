open! Core
open Stdio

let format_input lines = ""

let () =
In_channel.input_lines stdin
|> format_input
|> Out_channel.print_endline