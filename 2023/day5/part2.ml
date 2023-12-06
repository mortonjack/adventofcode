open! Core
open Stdio

type range = {
  dest_start: int;
  source_start: int;
  length: int;
}

type map = {
  ranges: range list
}

type input = {
  seeds: map;
  maps: map list;
}

let is_in_range seed range =
  if seed >= range.source_start && seed < range.source_start + range.length then
    Some (range.dest_start + seed - range.source_start)
  else None

let rec is_in_map seed = function
| [] -> false
| hd :: tl -> 
  if seed >= hd.source_start && seed < hd.source_start + hd.length then true 
  else is_in_map seed tl

let maps_to seed map =
  match List.find_map map.ranges ~f:(fun range -> is_in_range seed range) with
  | Some x -> x
  | None -> seed

let rec convert_seeds_to_ranges = function
| [] | [_] -> []
| start :: length :: tl -> {dest_start = start; source_start = start; length} :: (convert_seeds_to_ranges tl)

let make_map ranges = { ranges }

let read_seeds line = 
  match String.split line ~on:':' with
  | [_; nums] -> String.split nums ~on:' ' 
  |> List.filter ~f:(fun s -> not (String.equal s "")) 
  |> List.map ~f:(fun s -> Int.of_string s)
  |> convert_seeds_to_ranges
  |> make_map
  | _ -> failwith "Invalid input"

let read_range s = 
  String.split s ~on:' '
  |> List.map ~f:(fun s -> Int.of_string s)
  |> function 
  (* NOTE: Reversed from original input *)
  | source_start :: dest_start :: length :: [] -> { dest_start; source_start; length }
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
    { seeds; maps = List.rev maps }

let rec distance ~maps x = 
  match maps with
  | [] -> x
  | hd :: tl -> maps_to x hd |> distance ~maps:tl

let rec compute_closest_location location input = 
  let seed = distance ~maps:input.maps location in
  if is_in_map seed input.seeds.ranges then location 
  else compute_closest_location (location+1) input

let () = 
format_input (In_channel.input_lines stdin)
|> compute_closest_location 1
|> Int.to_string
|> Out_channel.print_endline;
