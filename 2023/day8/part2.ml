open! Core
open Stdio

type direction = 
| Left 
| Right

type node = {
  left: string;
  right: string;
}

type input = {
  directions: direction list;
  nodes: (string, node) Hashtbl.t;
}

let read_directions str =
  String.to_list str 
  |> List.map ~f:(fun c -> let open Char in 
  if c = 'L' then Left else Right)

let read_nodes l = 
  let rec read_line = function
  | [] -> []
  | hd :: tl -> 
    match String.filter hd ~f:(fun c -> Char.is_alpha c || Char.is_whitespace c || Char.is_digit c)
    |> String.split ~on:' ' |> List.filter ~f:(fun s -> not (String.is_empty s)) with
    | [key; left; right] -> (key, {left; right}) :: read_line tl
    | _ -> failwith "invalid node reading" in
  read_line l |> Hashtbl.of_alist_exn (module String)

let format_input = function
| [] | [_] | [_; _] -> failwith "invalid input"
| directions_str :: _ :: tl -> 
  let directions = read_directions directions_str in
  let nodes = read_nodes tl in
  { directions; nodes }

let is_target key = 
  String.equal (String.suffix key 1) "Z"

let is_starter key =
  String.equal (String.suffix key 1) "A"

let rec count_steps input ~key ~directions =
  let directions = match directions with 
  | Some x -> x 
  | None -> input.directions in
  if is_target key then 0 else
  let (direction, directions) = match directions with
  | [] -> failwith "too few directions!"
  | [x] -> (x, input.directions)
  | hd :: tl -> (hd, tl) in
  let node = Hashtbl.find_exn input.nodes key in
  let key = match direction with
  | Left -> node.left
  | Right -> node.right
  in
  1 + count_steps input ~key ~directions:(Some directions)

let starting_keys input = 
  Hashtbl.fold input.nodes ~init:[] ~f:(fun ~key ~data:_ acc -> if is_starter key then key :: acc else acc)

(*
let rec gcd n1 n2 =
  if n1 < n2 then gcd n2 n1 else
  if n1 % n2 = 0 then n2
  else gcd n2 (n1 % n2)

let rec gcd_list = function
  | [] | [_] -> failwith "too few numbers for gcd"
  | [n1; n2] -> gcd n1 n2
  | n1 :: tl -> gcd n1 (gcd_list tl)

let lcm nums : int =
  let gcd = gcd_list nums in
  let mul = List.fold nums ~init:1 ~f:(fun acc x -> acc * x) in
  mul / gcd
*)

let () = 
let input = format_input (In_channel.input_lines stdin) in
starting_keys input
|> List.map ~f:(fun key -> count_steps input ~key ~directions:None)
|> List.iter ~f:(fun x -> Int.to_string x |> Out_channel.print_endline)
