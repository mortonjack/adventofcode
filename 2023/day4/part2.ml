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

let rec add_to_count count num amount = 
  if (num = 0) then count else 
  match count with
  | [] -> (1 + amount) :: (add_to_count [] (num-1) amount)
  | hd :: tl -> (hd + amount) :: (add_to_count tl (num-1) amount)

let rec count_cards list ~count =
  match list with 
  | [] -> 0
  | hd :: tl -> 
    let (num, count) = match count with 
    | [] -> (1, [])
    | [x] -> (x, [])
    | hd :: tl -> (hd, tl)
  in
    let new_count = add_to_count count hd num in
    num + count_cards tl ~count:new_count

let () = 
format_input (In_channel.input_lines stdin)
|> winning_numbers 
|> count_cards ~count:[]
|> Int.to_string
|> Out_channel.print_endline;

