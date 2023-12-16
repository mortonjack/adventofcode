open! Core
open Stdio

type mirror = RightDown | LeftDown
type splitter = Vertical | Horizontal
type direction = Up | Down | Left | Right
type tile =
| Mirror of mirror
| Splitter of splitter
| Empty

type coord = {
  r : int;
  c : int
}

type visited = {
  up: bool array array;
  down: bool array array;
  left: bool array array;
  right: bool array array
}

let get_tile = function
| '.' -> Empty
| '/' -> Mirror RightDown
| '\\' -> Mirror LeftDown
| '-' -> Splitter Horizontal
| '|' -> Splitter Vertical
| _ -> failwith "invalid character"

let in_dir coord dir = match dir with
| Up -> { coord with r = coord.r - 1 }
| Down -> { coord with r = coord.r + 1 }
| Left -> { coord with c = coord.c - 1 }
| Right -> { coord with c = coord.c + 1 }

let format_input lines = 
  List.map lines ~f:(fun line -> String.to_list line 
  |> List.map ~f:get_tile 
  |> List.to_array)
  |> List.to_array

let make_visited arr =
  let copy arr = 
    Array.init (Array.length arr) ~f:(fun _ -> 
      Array.init (Array.length arr.(0)) ~f:(fun _ -> false))
  in
  { up    = copy arr
  ; down  = copy arr
  ; left  = copy arr
  ; right = copy arr}

let visited_dir visited dir =
  match dir with
  | Up -> visited.up
  | Down -> visited.down
  | Left -> visited.left
  | Right -> visited.right

let already_visited visited coord dir = 
  let visited = visited_dir visited dir in
  visited.(coord.r).(coord.c)

let visit visited coord dir = 
  let visited = visited_dir visited dir in
  visited.(coord.r).(coord.c) <- true

let next_coords contraption coord ~dir =
  let down = (in_dir coord Down, Down) in
  let up = (in_dir coord Up, Up) in
  let left = (in_dir coord Left, Left) in
  let right = (in_dir coord Right, Right) in
  let same = (in_dir coord dir, dir) in
  match contraption.(coord.r).(coord.c) with
  | Empty -> [same]
  | Mirror x -> (
    match x, dir with
    | LeftDown, Right | RightDown, Left -> [down]
    | LeftDown, Left | RightDown, Right -> [up]
    | LeftDown, Down | RightDown, Up -> [right]
    | LeftDown, Up | RightDown, Down -> [left])
  | Splitter x -> match x, dir with
    | Horizontal, Left | Horizontal, Right 
    | Vertical, Up | Vertical, Down -> [same]
    | Horizontal, Up | Horizontal, Down -> [left; right]
    | Vertical, Left | Vertical, Right -> [up; down]
  
let rec search contraption visited coord ~dir =
  if coord.r < 0 || coord.r >= (Array.length contraption)
    || coord.c < 0 || coord.c >= (Array.length contraption.(0))
    || already_visited visited coord dir then () 
  else (
    visit visited coord dir;
    List.iter (next_coords contraption coord ~dir) 
    ~f:(fun (coord, dir) -> search contraption visited coord ~dir)
  )

let count_visited visited =
  Array.foldi ~init:0 visited.up ~f:(fun i acc x ->
    acc + Array.foldi ~init:0 x ~f:(fun j acc _ ->
      if visited.up.(i).(j)
      || visited.down.(i).(j)
      || visited.left.(i).(j)
      || visited.right.(i).(j) 
      then acc+1 else acc))

let () = 
let contraption = In_channel.input_lines stdin |> format_input
in let visited = make_visited contraption in
search contraption visited { r = 0; c = 0 } ~dir:Right;
count_visited visited
|> Int.to_string
|> Out_channel.print_endline