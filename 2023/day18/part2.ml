open! Core
open Stdio

type direction = Up | Down | Left | Right
type instruction = {
  dir : direction;
  len : int;
  col : int;
}

type coord = {
  row : int;
  col : int
}

let next_coord ~coord ~instruction =
  match instruction.dir with
  | Up -> { coord with row = coord.row - instruction.len }
  | Down -> { coord with row = coord.row + instruction.len }
  | Left -> { coord with col = coord.col - instruction.len }
  | Right -> { coord with col = coord.col + instruction.len }

let dir_of = function
  | "U" -> Up
  | "D" -> Down
  | "L" -> Left
  | "R" -> Right
  | dir -> failwith ("Invalid direction: " ^ dir)

let rec boundaries instructions (min_r, min_c, max_r, max_c) ~coord =
  match instructions with
  | [] -> (min_r, min_c, max_r, max_c)
  | instruction :: remaining ->
    let coord = next_coord ~coord ~instruction in
    boundaries remaining ~coord (
      min min_r coord.row,
      min min_c coord.col,
      max max_r coord.row,
      max max_c coord.col
    )

let start_to_end ~instruction ~coord ~map = 
  let (inc_r, inc_c) = match instruction.dir with
  | Up -> (-1, 0)
  | Down -> (1, 0)
  | Left -> (0, -1)
  | Right -> (0, 1) in
  let r = ref coord.row in
  let c = ref coord.col in
  for _ = 1 to instruction.len do
    r := !r + inc_r;
    c := !c + inc_c;
    map.(!r).(!c) <- instruction.col;
  done

let rec fill_map instructions ~coord ~map = 
  match instructions with
  | [] -> ()
  | instruction :: remaining -> 
    start_to_end ~map ~coord ~instruction;
    let coord = next_coord ~coord ~instruction in
    fill_map remaining ~coord ~map

let format_input = 
  List.map ~f:(fun line ->
    match 
      String.split line ~on:' '
      |> List.map ~f:(String.strip 
      ~drop:(fun c -> Char.(c = '(' || c = ')' || c = '#')))
    with
    | [direction; length; colour] -> 
      let dir = dir_of direction in
      let len = Int.of_string length in
      let col =  Int.Hex.of_string ("0x" ^ colour) in
      {dir; len; col}
    | _ -> failwith "Invalid line length")

let surrounding ~coord ~map =
  [{coord with row = coord.row + 1}; {coord with row = coord.row - 1};
   {coord with col = coord.col + 1}; {coord with col = coord.col - 1}]
  |> List.filter ~f:(fun {row; col} ->
    row >= 0 && row < Array.length map
    && col >= 0 && col < Array.length map.(0)
    && map.(row).(col) = -1)

let rec dfs ~coords ~map ~count =
  match coords with
  | [] -> count
  | coord :: remaining ->
    if map.(coord.row).(coord.col) <> -1
    then dfs ~coords:remaining ~map ~count
    else (
      map.(coord.row).(coord.col) <- (-2);
      dfs ~coords:(coords @ surrounding ~coord ~map) ~map ~count:(count + 1)
    )

let count_tiles map = 
  let start_coords_row = 
    Array.mapi map ~f:(fun i _ -> [{row = i; col = 0}; {row = i; col = Array.length map.(0) - 1}])
    |> Array.to_list |> List.concat in
  let start_coords_col =
    Array.mapi map.(0) ~f:(fun j _ -> [{row = 0; col = j}; {row = Array.length map - 1; col = j}])
    |> Array.to_list |> List.concat in
  let coords = List.concat [start_coords_row; start_coords_col] in
  let unfilled = dfs ~coords ~map ~count:0 in
  Array.length map * Array.length map.(0) - unfilled

let _output_map =
  Array.iter ~f:(fun row -> 
    Array.iter row ~f:(fun n -> let tile = if n = -1 then "." else "#" in
    Out_channel.print_string tile); Out_channel.print_endline "")

let () = 
let instructions = In_channel.input_lines stdin |> format_input in
let (min_r, min_c, max_r, max_c) = 
boundaries instructions (0, 0, 0, 0) ~coord:{ row = 0; col = 0 } in
let map = Array.init (max_r - min_r + 1) ~f:(fun _ -> 
  Array.init (max_c - min_c + 1) ~f:(fun _ -> -1)) in
fill_map instructions ~map ~coord:{ row = 0 - min_r; col = 0 - min_c};
count_tiles map
|> Int.to_string
|> Out_channel.print_endline