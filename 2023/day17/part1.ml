open! Core
open Stdio
open Containers

type direction = Up | Down | Left | Right
let new_movecount dir1 dir2 moves =
  match dir1, dir2 with
  | Up, Up | Down, Down | Left, Left | Right, Right -> 3
  | _ -> moves - 1

module Pos = struct
  type t = {
    r : int;
    c : int;
    moves : int;
    heat_lost : int;
    dir : direction
  }
  let in_dir pos dir = 
    let moves = new_movecount dir pos.dir pos.moves in
    match dir with
    | Up -> { pos with r = pos.r - 1; moves; dir }
    | Down -> { pos with r = pos.r + 1; moves; dir }
    | Left -> { pos with c = pos.c - 1; moves; dir }
    | Right -> { pos with c = pos.c + 1;  moves; dir }

  let leq first second =
    first.heat_lost <= second.heat_lost

  let create ~r ~c ~moves ~heat_lost ~dir = {r; c; moves; heat_lost; dir}

end

module PosHeap = Containers.Heap.Make(struct 
type t = Pos.t 
let leq = Pos.leq 
end)

let find_shortest_path map = 
  let start1 = Pos.create ~r:0 ~c:0 ~moves:0 ~heat_lost:0 ~dir:Down in
  let start2 = Pos.create ~r:0 ~c:0 ~moves:0 ~heat_lost:0 ~dir:Right in
  let pq = PosHeap


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