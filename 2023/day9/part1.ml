open! Core
open Stdio

let format_input _ = ""

let () = 
format_input (In_channel.input_lines stdin)
|> Out_channel.print_endline;