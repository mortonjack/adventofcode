open! Core
open Stdio

type direction = 
| Left 
| Right

type node = {
  left: string;
  right: string
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
    match String.filter hd ~f:(fun c -> Char.is_alpha c || Char.is_whitespace c)
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

let rec count_steps input ~key ~target ~directions =
  let directions = match directions with 
  | Some x -> x 
  | None -> input.directions in
  if String.equal key target then 0 else
  let (direction, directions) = match directions with
  | [] -> failwith "too few directions!"
  | [x] -> (x, input.directions)
  | hd :: tl -> (hd, tl) in
  let node = Hashtbl.find_exn input.nodes key in
  let key = match direction with
  | Left -> node.left
  | Right -> node.right
  in
  1 + count_steps input ~key ~target ~directions:(Some directions)
  

let () = 
format_input (In_channel.input_lines stdin)
|> count_steps ~key:"AAA" ~target:"ZZZ" ~directions:None
|> Int.to_string
|> Out_channel.print_endline;