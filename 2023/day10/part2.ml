open! Core
open Stdio

type node = Land | Horizontal | Vertical | NorthWest | NorthEast | SouthEast | SouthWest | Start
type coord = { i: int; j: int }
type inside = Inside | Outside | Up | Down

let connects_left = function
| Start | Horizontal | NorthWest | SouthWest -> true
| _ -> false

let connects_right = function
| Start | Horizontal | NorthEast | SouthEast -> true
| _ -> false

let connects_up = function
| Start | Vertical | NorthEast | NorthWest -> true
| _ -> false

let connects_down = function
| Start | Vertical | SouthEast | SouthWest -> true
| _ -> false

let char_to_node = function
  | '-' -> Horizontal
  | '|' -> Vertical
  | 'J' -> NorthWest
  | 'L' -> NorthEast
  | 'F' -> SouthEast
  | '7' -> SouthWest
  | 'S' -> Start
  | '.' -> Land
  | _ -> failwith "illegal character in input"

module Graph = struct 
  type t = { nodes: node array array; visited: bool array array }

  let create l : t = 
    let nodes = List.map l ~f:List.to_array |> List.to_array in
    let visited = Array.map nodes ~f:(fun a -> Array.map a ~f:(fun _ -> false)) in
    { nodes; visited }

  let find_start (t : t) =
    Array.foldi t.nodes ~init:{i = -1; j = -1} ~f:(fun i acc a ->
      match Array.findi a ~f:(fun _ n -> match n with | Start -> true | _ -> false) with
      | None -> acc
      | Some (j, _) -> {i; j}
    )

  let at (t : t) coord =
    t.nodes.(coord.i).(coord.j)

  let connects t coord1 coord2 = 
    let n = Array.length t.nodes in
    let m = Array.length t.nodes.(0) in
    if coord2.i < 0 || coord2.i >= n || coord2.j < 0 || coord2.j >= m then false else
    let node1 = at t coord1 in 
    let node2 = at t coord2 in
    if coord2.j = coord1.j - 1 then connects_left node1 && connects_right node2 else
    if coord2.j = coord1.j + 1 then connects_right node1 && connects_left node2 else
    if coord2.i = coord1.i - 1 then connects_up node1 && connects_down node2 else
    if coord2.i = coord1.i + 1 then connects_down node1 && connects_up node2 else false

  let visit t coord =
    t.visited.(coord.i).(coord.j) <- true

  let visited t coord =
    t.visited.(coord.i).(coord.j)

  let count_area t =
    Array.foldi t.visited ~init:0 ~f:(fun i acc a -> let (count, _) =
    Array.foldi a ~init:(0, Outside) ~f:(fun j (count, inside) node -> 
      Out_channel.print_string (Int.to_string count);
      if node then
        match (at t {i; j}, inside) with 
        | Vertical, Inside -> count, Outside
        | Vertical, Outside -> count, Inside 
        | NorthEast, Outside -> count, Up
        | SouthEast, Outside -> count, Down
        | SouthEast, Inside -> count, Up
        | NorthEast, Inside -> count, Down
        | NorthWest, Up | SouthWest, Down -> count, Outside
        | NorthWest, Down | SouthWest, Up -> count, Inside
        | Horizontal, x -> count, x
        (* Only works on my input because start = horizontal *)
        | Start, x -> count, x
        | _ -> failwith "Unknown configuration!"
      else if match inside with | Inside -> true | _ -> false then count+1, inside else count, inside)
    in Out_channel.print_endline (" " ^ (Int.to_string (acc + count))); acc + count)

  let display_graph t = 
    Out_channel.print_endline "";
    Array.iter t.visited ~f:(fun a ->
    Array.iter a ~f:(fun b -> let c = if b then "X" else "." in Out_channel.print_string c);
    Out_channel.print_endline "")

end

let surroundings graph coord = 
  let coord_up = {i = coord.i - 1; j = coord.j} in
  let coord_down = {i = coord.i + 1; j = coord.j} in
  let coord_left = {i = coord.i; j = coord.j - 1} in
  let coord_right = {i = coord.i; j = coord.j + 1} in
  let coord_list = [coord_up; coord_down; coord_left; coord_right] in
  match List.filter coord_list ~f:(fun surrounding_coord -> Graph.connects graph coord surrounding_coord) with
  | [coord1; coord2] -> (coord1, coord2)
  | _ -> failwith "Surrounding coords != 2"

let unvisited graph (coord1, coord2) =
  if not (Graph.visited graph coord1) then Some coord1 else
  if not (Graph.visited graph coord2) then Some coord2 else None

let find_furthest (graph, start) = 
  let rec dfs coord1 coord2 count = 
    Graph.visit graph coord1; Graph.visit graph coord2;
    let new_coord1 = unvisited graph (surroundings graph coord1) in
    let new_coord2 = unvisited graph (surroundings graph coord2) in 
    match new_coord1, new_coord2 with
    | Some coord1, Some coord2 -> dfs coord1 coord2 (count+1)
    | None, _ | _, None -> count
in
  Graph.visit graph start;
  let (coord1, coord2) = surroundings graph start in
  let count = dfs coord1 coord2 1 in
  let area = Graph.count_area graph in
  Graph.display_graph graph;
  (count, area)

let format_input lines = 
  let graph = 
    List.map lines ~f:(fun s -> String.to_list s |> List.map ~f:char_to_node)
    |> Graph.create
  in let start = Graph.find_start graph
in (graph, start)

let () = 
let (ans1, ans2) = format_input (In_channel.input_lines stdin) |> find_furthest 
in
Out_channel.print_endline (Int.to_string ans1);
Out_channel.print_endline (Int.to_string ans2)