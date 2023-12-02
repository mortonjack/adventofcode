open Base
open Stdio

type game = {
  id : int;
  blue : int;
  red : int;
  green : int;
}

let max_counts game_info = 
  let games = String.Search_pattern. (split_on (create "; ") game_info ) in
  let rec get_counts games (blue, red, green) =
    match games with
    | [] -> (blue, red, green)
    | hd :: tl -> 
      let colours = String.Search_pattern. (split_on (create ", ") hd) in
      let rec count colours (blue, red, green) =
        match colours with
        | [] -> (blue, red, green)
        | hd :: tl -> 
          let num = Int.of_string (String.strip hd ~drop:(fun c -> not (Char.is_digit c))) in
          if String.is_suffix ~suffix:"blue" hd then count tl (num, red, green)
          else if String.is_suffix ~suffix:"red" hd then count tl (blue, num, green)
          else if String.is_suffix ~suffix:"green" hd then count tl (blue, red, num)
          else failwith "Invalid colour" 
        in
      let (b, r, g) = count colours (0, 0, 0) in
      get_counts tl ((if b > blue then b else blue), (if r > red then r else red), (if g > green then g else green))
    in
  get_counts games (0, 0, 0)

let analyse_game input_line = 
  let split_str = String.Search_pattern. (split_on (create ": ") input_line ) in
  match split_str with
  | [] | [_] -> failwith "No game info"
  | _ :: _ :: _ :: _ -> failwith "Two game IDs in one line"
  | [game_id; game_info] -> let id = Int.of_string (String.strip game_id ~drop:(fun c -> not (Char.is_digit c))) in
    let (blue, red, green) = max_counts game_info in
    { id; blue; red; green }

let power game = 
  game.blue * game.red * game.green

let rec sum_valid_games games count = 
  match games with 
  | [] -> count
  | hd :: tl -> 
    sum_valid_games tl (count + power hd)

let () = 
let raw_games = In_channel.input_lines stdin in
let games = List.fold raw_games ~init:[] ~f:(fun acc game -> (analyse_game game) :: acc) in
let sum = sum_valid_games games 0 in
Out_channel.output_string stdout ((Int.to_string sum) ^ "\n");
Out_channel.flush stdout