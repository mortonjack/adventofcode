open! Core
open Stdio

type inside = Inside of int | Outside | Up of int | Down of int
type direction = Up | Down | Left | Right

type instruction = {
  dir : direction;
  len : int;
}
type coord = {
  row: int;
  col: int
}
type node = {
  row : int;
  col : int;
  dir : direction;
  dir_from : direction
}

let node_compare node1 node2 = 
  if node1.col = node2.col then 0
  else if node1.col < node2.col then -1 else 1

let pos_or_neg = function
| Up | Left -> -1
| Down | Right -> 1

let next_coord ~(coord : coord) ~(instruction : instruction) =
  match instruction.dir with
  | Up -> { coord with row = coord.row - instruction.len }
  | Down -> { coord with row = coord.row + instruction.len }
  | Left -> { coord with col = coord.col - instruction.len }
  | Right -> { coord with col = coord.col + instruction.len }

let dir_of = function
  | '3' -> Up
  | '1' -> Down
  | '2' -> Left
  | '0' -> Right
  | _ -> failwith ("Invalid direction")

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

let format_input = 
  List.map ~f:(fun line ->
    match 
      String.split line ~on:' '
      |> List.map ~f:(String.strip 
      ~drop:(fun c -> Char.(c = '(' || c = ')' || c = '#')))
    with
    | [_; _; hex] -> 
      let chars = String.to_list hex in
      let colour = String.of_char_list (List.take chars 5) in
      let dir = dir_of (List.last_exn chars) in
      let len =  Int.Hex.of_string ("0x" ^ colour) in
      {dir; len}
    | _ -> failwith "Invalid line length")

let rec map_instructions points (instructions : instruction list) r c dir_from = 
  match instructions with
  | [] -> ();
  | instruction :: remaining ->
      let first_new_node = {
        row = r;
        col = c;
        dir = instruction.dir;
        dir_from;
      } in
      points.(r) <- first_new_node :: points.(r);
    let (new_r, new_c) = match instruction.dir with
    | Up | Down as dir -> (
      let multiplier = pos_or_neg dir in
      for i = 1 to instruction.len - 1 do
        let new_node = {
          row = r + i * multiplier;
          col = c;
          dir;
          dir_from = dir
        } in
        points.(r + i * multiplier) <- new_node :: points.(r + i * multiplier)
      done;
      (r + instruction.len * multiplier, c)
    )
    | Left | Right as dir -> (
      let multiplier = pos_or_neg dir in
      (r, c + instruction.len * multiplier)
    )
    in
    map_instructions points remaining new_r new_c instruction.dir

let rec count_tiles_in_row points ~inside ~running =
  match points with
  | [] -> running
  | pt :: tl ->
    match pt.dir, pt.dir_from, inside with
    | Up, Up, Inside c | Down, Down, Inside c -> count_tiles_in_row tl ~inside:Outside ~running:(running + pt.col - c + 1)
    | Up, Up, Outside | Down, Down, Outside -> count_tiles_in_row tl ~inside:(Inside pt.col) ~running
    | (Right | Left), Up, Outside | Up, (Left | Right), Outside -> count_tiles_in_row tl ~inside:(Down pt.col) ~running
    | (Right | Left), Down, Outside | Down, (Left | Right), Outside -> count_tiles_in_row tl ~inside:(Up pt.col) ~running
    | (Right | Left), Down, Inside c | Down, (Left | Right), Inside c -> count_tiles_in_row tl ~inside:(Down c) ~running
    | (Right | Left), Up, Inside c | Up, (Left | Right), Inside c -> count_tiles_in_row tl ~inside:(Up c) ~running
    | Up, (Right | Left), Up c | Down, (Right | Left), Down c | (Right | Left), Up, Up c | (Right | Left), Down, Down c 
      -> count_tiles_in_row tl ~inside:Outside ~running:(running + pt.col - c + 1)
    | Up, (Right | Left), Down c | Down, (Right | Left), Up c | (Right | Left), Up, Down c | (Right | Left), Down, Up c
    -> count_tiles_in_row  tl ~inside:(Inside c) ~running
    | (Right | Left), _, (Down _ | Up _) -> failwith  "ending strip with non up/left"
    | (Right | Left), (Right | Left), _ -> failwith "rightright/leftleft"
    | (Up | Down), (Up | Down),  _ -> failwith "upup/downdown"

let () = 
let instructions = In_channel.input_lines stdin |> format_input in
let (min_r, min_c, max_r, _max_c) = 
boundaries instructions (0, 0, 0, 0) ~coord:{ row = 0; col = 0 } in
let points = Array.init (max_r - min_r + 1) ~f:(fun _ -> []) in
map_instructions points instructions (-min_r) (-min_c) ((List.last_exn instructions).dir);
let points = Array.map points ~f:(List.dedup_and_sort ~compare:node_compare) in
Array.sum (module Int) ~f:(count_tiles_in_row ~inside:Outside ~running:0) points
|> Int.to_string
|> Out_channel.print_endline