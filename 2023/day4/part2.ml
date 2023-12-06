open! Core
open Stdio

let format_input input = 
  List.map input ~f:(fun s -> 
    match String.split s ~on:':' with 
    | [_; s] -> s
    | _ -> failwith "Invalid input")
  |> List.map ~f:(fun s -> String.split s ~on:'|')
  |> List.map ~f:(fun nums_str -> 
    List.map nums_str ~f:(fun s -> String.split s ~on:' ' 
    |> List.filter ~f:(fun s -> not (String.equal s "")) ))

let contains l elem ~equals =
  List.filter l ~f:(fun e -> equals e elem)
  |> List.is_empty |> not

let rec count_matching_numbers winning_nums chosen_nums =
  match chosen_nums with
  | [] -> 0
  | hd :: tl ->
    let add = if contains winning_nums hd ~equals:(String.equal) then 1 else 0 in
    add + count_matching_numbers winning_nums tl

let winning_numbers points = 
  List.map points ~f:(fun nums -> 
    match nums with 
    | [] | [_] | _ :: _ :: _ :: _ -> failwith "Invalid input"
    | [winning_nums; chosen_nums] -> count_matching_numbers winning_nums chosen_nums
    )

let () = 
format_input (In_channel.input_lines stdin)
|> winning_numbers 
|> List.sum (module Int) ~f:(fun x -> Int.of_float (2. **. (Float.of_int (x-1))))
|> Int.to_string
|> Out_channel.print_endline;

