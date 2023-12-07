open! Core
open Stdio

type race = {
  time: int;
  distance: int;
}

let line_to_list line = 
  let (_, nums) = String.lsplit2_exn line ~on:':' in
  String.split nums ~on:' ' 
  |> List.filter ~f:(fun s -> not (String.is_empty s))
  |> List.map ~f:(fun s -> Int.of_string s)

let format_input time distance = 
  let time_list = line_to_list time in
  let dist_list = line_to_list distance in
  List.map2_exn time_list dist_list ~f:(fun time distance -> {time; distance})
  
let ways_to_win race = 
  let rec count_ways lo hi =
    if lo = hi then 0
    else if (lo * (hi - lo)) > race.distance then 1 + count_ways (lo+1) hi
    else count_ways (lo+1) hi
  in count_ways 1 race.time


let () = 
let (time, distance) = match In_channel.input_lines stdin with
| [time; distance] -> (time, distance)
| _ -> failwith "invalid input"
in format_input time distance
|> List.map ~f:ways_to_win
|> List.fold ~init:1 ~f:(fun acc x -> acc * x)
|> Int.to_string
|> Out_channel.print_endline;