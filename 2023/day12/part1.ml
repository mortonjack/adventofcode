open! Core
open Stdio

type row = {
  springs: string;
  nums: int list;
}

let calc_line { springs; nums } = 1


let format_input = 
  List.map ~f:(fun line -> 
    match String.split line ~on:' ' with
    | [] | [_] | _ :: _ :: _ :: _ -> failwith "Invalid input"
    | [springs; nums] ->
      let nums_list = List.map (String.split nums ~on:',') ~f:Int.of_string
    in {springs; nums = nums_list})

let () =
format_input (In_channel.input_lines stdin)
|> List.map ~f:calc_line
|> List.sum (module Int) ~f:Fn.id
|> Int.to_string
|> Out_channel.print_endline