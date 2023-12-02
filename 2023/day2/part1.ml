open Base
open Stdio

type cubes = {
  blue : int;
  red : int;
  green : int;
}
type game = {
  id : int;
  colours : cubes;
}

let no_cubes = { blue = 0; red = 0; green = 0 }

let compare_cubes cubes round_cubes = { 
  blue = max cubes.blue round_cubes.blue;
  red = max cubes.red round_cubes.red;
  green = max cubes.green round_cubes.green }

let rec count colours cubes =
  match colours with
  | [] -> cubes
  | hd :: tl -> 
    let num = Int.of_string (String.strip hd ~drop:(fun c -> not (Char.is_digit c))) in
    if String.is_suffix ~suffix:"blue" hd then count tl {cubes with blue = num}
    else if String.is_suffix ~suffix:"red" hd then count tl {cubes with red = num}
    else if String.is_suffix ~suffix:"green" hd then count tl {cubes with green = num}
    else failwith "Invalid colour"

let max_counts game_info = 
  let games = String.Search_pattern. (split_on (create "; ") game_info ) in
  let rec get_counts games cubes =
    match games with
    | [] -> cubes
    | round :: tl -> 
      let colours = String.Search_pattern. (split_on (create ", ") round) in
      let round_cubes = count colours no_cubes in
      get_counts tl (compare_cubes cubes round_cubes)
    in
  get_counts games no_cubes

let analyse_game input_line = 
  let split_str = String.Search_pattern. (split_on (create ": ") input_line ) in
  match split_str with
  | [] | [_] -> failwith "No game info"
  | _ :: _ :: _ :: _ -> failwith "Two game IDs in one line"
  | [game_id; game_info] -> let id = Int.of_string (String.strip game_id ~drop:(fun c -> not (Char.is_digit c))) in
    let colours = max_counts game_info in
    { id; colours }

let valid cubes = 
  cubes.blue <= 14 && cubes.red <= 12 && cubes.green <= 13

let rec sum_valid_games games count = 
  match games with 
  | [] -> count
  | hd :: tl -> 
    if (valid hd.colours) then sum_valid_games tl (count + hd.id)
    else sum_valid_games tl count

let () = 
let raw_games = In_channel.input_lines stdin in
let games = List.fold raw_games ~init:[] ~f:(fun acc game -> (analyse_game game) :: acc) in
let sum = sum_valid_games games 0 in
Out_channel.output_string stdout ((Int.to_string sum) ^ "\n");
Out_channel.flush stdout