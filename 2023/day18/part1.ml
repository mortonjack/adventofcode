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

let next_coord coord instruction =
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
  | _ -> failwith "Invalid direction"

let format_input = 
  List.map ~f:(fun line ->
    match 
      String.split line ~on:' '
      |> List.map ~f:(String.strip 
      ~drop:(fun c -> not Char.(c = '(' || c = ')' || c = '#')))
    with
    | [direction; length; colour] -> 
      let dir = dir_of direction in
      let len = Int.of_string length in
      let col =  Int.Hex.of_string ("0x" ^ colour) in
      {dir; len; col}
    | _ -> failwith "Invalid line length")

let () = 
let instructions = In_channel.input_lines stdin |> format_input in

|> Out_channel.print_endline