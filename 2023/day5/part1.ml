open! Core
open Stdio

type range = {
  source_start: int;
  dest_start: int;
  length: int;
}

type map = {
  ranges: range list
}

type input = {
  seeds: int list;
  maps: map list;
}

let is_in_range seed range =
  if seed >= range.source_start && seed < range.source_start + range.length then
    Some (range.dest_start + seed - range.source_start)
  else None

let maps_to seed map =
  match List.find_map map.ranges ~f:(fun range -> is_in_range seed range) with
  | Some x -> x
  | None -> seed

let read_seeds line = 
  match String.split line ~on:':' with
  | [_; nums] -> String.split nums ~on:' ' 
  |> List.filter ~f:(fun s -> not (String.equal s "")) 
  |> List.map ~f:(fun s -> Int.of_string s)
  | _ -> failwith "Invalid input"

let read_range s = 
  String.split s ~on:' '
  |> List.map ~f:(fun s -> Int.of_string s)
  |> function 
  | source_start :: dest_start :: length :: [] -> { source_start; dest_start; length }
  | _ -> failwith "Invalid range"

let rec read_map = function
| [] -> { ranges = [] }, []
| hd :: tl -> if String.equal hd "" then { ranges = [] }, (hd :: tl) else 
let (map, tl) = read_map tl in
{ranges = (read_range hd) :: map.ranges}, tl

let rec read_maps = function
| [] -> []
| hd :: tl -> 
  if (String.equal hd "") then read_maps tl else
  let (map, tl) = read_map tl in
  map :: (read_maps tl)

let format_input = function
  | [] | [_] -> failwith "Invalid input"
  | hd :: tl -> 
    let seeds = read_seeds hd in
    let maps = read_maps tl in
    { seeds; maps }

let () = 
format_input (In_channel.input_lines stdin)
|> Out_channel.print_endline;
