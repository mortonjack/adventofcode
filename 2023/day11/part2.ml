open! Core
open Stdio

type node = Space | Galaxy

let is_galaxy = function
| Space -> false 
| Galaxy -> true

type observations = {
  space_map: node array array;
  row_space: int array;
  col_space: int array
}

let find_expanded_space space_map = 
  let row_space = Array.fold space_map ~init:[] ~f:(fun acc line -> 
    match Array.find line ~f:is_galaxy with
    | Some _ -> acc @ [1]
    | None -> acc @ [1000000])
  |> List.to_array in
  let col_space = 
    Array.fold space_map ~init:[] ~f:(fun acc line ->
    let line = Array.to_list line in
    let acc = match acc with
    | [] -> List.map line ~f:(fun _ -> 1000000)
    | _ -> acc in
    List.zip_exn line acc
    |> List.map ~f:(fun (node, acc) -> if is_galaxy node then 1 else acc))
    |> List.to_array in
    {space_map; row_space; col_space}

let find_galaxies observations =
  let (galaxies, _) = Array.foldi observations.space_map ~init:([], 0) ~f:(fun i (acc, true_i) line ->
    let (new_galaxies, _) = Array.foldi line ~init:([], 0) ~f:(fun j (acc, true_j) node -> 
      if is_galaxy node then ((true_i, true_j) :: acc, true_j + 1)
      else (acc, true_j + observations.col_space.(j)))
    in (new_galaxies :: acc, true_i + observations.row_space.(i)))
  in List.concat galaxies

let format_input lines = 
  List.map lines ~f:(fun s -> String.to_array s 
    |> Array.map ~f:(fun c -> if Char.(c = '#') then Galaxy else Space))
  |> List.to_array

let rec calculate_distances (x, y) list =
  match list with
  | [] -> 0
  | (i, j) :: tl -> (abs (x - i)) + (abs (y - j)) + (calculate_distances (x, y) tl) 

let rec calculate_distance = function
  | [] | [_] -> 0
  | hd :: tl -> (calculate_distances hd tl) + (calculate_distance tl)

let () =
format_input (In_channel.input_lines stdin)
|> find_expanded_space
|> find_galaxies
|> calculate_distance
|> Int.to_string
|> Out_channel.print_endline