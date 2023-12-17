open! Core
open Stdio
open! Containers.Heap

type direction = Up | Down | Left | Right
let new_movecount dir1 dir2 moves =
  match dir1, dir2 with
  | Up, Up | Down, Down | Left, Left | Right, Right -> moves - 1
  | _ -> 3

module Pos = struct
  type t = {
    r : int;
    c : int;
    moves : int;
    heat_lost : int;
    dir : direction
  } [@@deriving fields ~getters]

  let in_dir pos dir = 
    let moves = new_movecount dir pos.dir pos.moves in
    match dir with
    | Up -> { pos with r = pos.r - 1; moves; dir }
    | Down -> { pos with r = pos.r + 1; moves; dir }
    | Left -> { pos with c = pos.c - 1; moves; dir }
    | Right -> { pos with c = pos.c + 1; moves; dir }

  let leq first second =
    first.heat_lost <= second.heat_lost

  let add_heat pos ~map =
    { pos with heat_lost = pos.heat_lost + map.(pos.r).(pos.c)}

  let create ~r ~c ~moves ~heat_lost ~dir = {r; c; moves; heat_lost; dir}

end

module PosHeap = Containers.Heap.Make(struct 
type t = Pos.t 
let leq = Pos.leq 
end)

let new_valid_dirs map pos = 
  let dir = Pos.dir pos in
  let straight = Pos.in_dir pos dir in
  let move_list = match dir with
  | Up | Down -> Pos.in_dir pos Left :: Pos.in_dir pos Right :: straight :: []
  | Left | Right -> Pos.in_dir pos Up :: Pos.in_dir pos Down :: straight :: []
  in
  List.filter move_list ~f:(fun pos ->
    let r = Pos.r pos in
    let c = Pos.c pos in
    let moves = Pos.moves pos in  
    moves > 0 && r >= 0 && c >= 0 && r < Array.length map && c < Array.length map.(0)
  ) |> List.map ~f:(Pos.add_heat ~map)

let key pos = 
  let r = Pos.r pos in
  let c = Pos.c pos in
  let dir = match Pos.dir pos with | Up -> 0 | Down -> 1 | Left -> 2 | Right -> 3 in
  let moves = Pos.moves pos in
  Int.(shift_left ((shift_left ((shift_left r 8) + c) 8) + moves) 3) + dir

let is_seen pos seen =
  let key = key pos in
  Hashtbl.mem seen key

let make_seen pos seen =
  let data = Pos.heat_lost pos in
  let key = key pos in
  Hashtbl.set seen ~key ~data

let rec dijkstras map heap seen (end_r, end_c as end_pos) = 
  let (heap, pos) = PosHeap.take_exn heap in
  if Pos.r pos = end_r && Pos.c pos = end_c then Pos.heat_lost pos
  else if is_seen pos seen then dijkstras map heap seen end_pos else (
    make_seen pos seen;
    let heap = PosHeap.add_list heap (new_valid_dirs map pos) in
    dijkstras map heap seen end_pos
  )

let find_shortest_path map = 
  let start1 = Pos.create ~r:0 ~c:0 ~moves:0 ~heat_lost:0 ~dir:Down in
  let start2 = Pos.create ~r:0 ~c:0 ~moves:0 ~heat_lost:0 ~dir:Right in
  let heap = PosHeap.of_list [start1; start2] in
  let seen = Hashtbl.create (module Int) in
  dijkstras map heap seen (Array.length map - 1, Array.length map.(0) - 1)

let format_input lines = 
  List.map lines ~f:(fun line 
    -> String.to_list line
    |> List.map ~f:Char.to_string
    |> List.map ~f:Int.of_string
    |> List.to_array)
  |> List.to_array

let () =
In_channel.input_lines stdin
|> format_input
|> find_shortest_path
|> Int.to_string
|> Out_channel.print_endline