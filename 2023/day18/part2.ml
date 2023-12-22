open! Core
open Stdio

type direction = Up | Down | Left | Right
type instruction = {
  dir : direction;
  len : int;
}
type coord = {
  row: int;
  col: int
}

let dir_of = function
  | '3' -> Up
  | '1' -> Down
  | '2' -> Left
  | '0' -> Right
  | _ -> failwith ("Invalid direction")

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

let rec instructions_to_points ~row ~col = function
| [] -> []
| instruction :: tl ->
  let (r, c) = match instruction.dir with
  | Up -> (-1, 0)
  | Down -> (1, 0)
  | Left -> (0, -1)
  | Right -> (0, 1)
  in
  {row; col} :: (instructions_to_points tl ~row:(row + r * instruction.len) ~col:(col + c * instruction.len))

let repeat_first_elem list =
  let first_elem = List.hd_exn list in
  list @ [first_elem]

let rec shoelace ~sum = function
| [] | [_] -> sum / 2 + 1
| p1 :: p2 :: tl ->
  let sum = sum + (p1.col * p2.row) - (p1.row * p2.col) + (abs (p2.row - p1.row + p2.col - p1.col)) in
  shoelace ~sum (p2 :: tl)

let () = 
In_channel.input_lines stdin |> format_input
|> instructions_to_points ~row:0 ~col:0
|> repeat_first_elem
|> shoelace ~sum:0
|> Int.to_string
|> Out_channel.print_endline