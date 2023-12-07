open! Core
open Stdio

type race = {
  time: int;
  distance: int;
}

let read_line line = 
  let (_, nums) = String.lsplit2_exn line ~on:':' in
  String.filter nums ~f:(fun c -> not (Char.is_whitespace c))
  |> Int.of_string

let format_input time distance = 
  let time = read_line time in
  let distance = read_line distance in
  {time; distance}
  
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
|> ways_to_win
|> Int.to_string
|> Out_channel.print_endline;