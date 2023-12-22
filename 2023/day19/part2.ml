open! Core
open Stdio

type xmas = X | M | A | S
type workflow = {
  xmas : xmas;
  required : int;
  op : int -> int -> bool;
  key : string
}
type input = {
  x : int;
  m : int;
  a : int;
  s : int
}

let rec process_workflow = function
| [] -> []
| [s] -> [`Default s]
| hd :: tl ->
  let xmas = match String.get hd 0 with
  | 'x' -> X
  | 'm' -> M
  | 'a' -> A
  | 's' -> S
  | _ -> failwith "unknown var" in
  let op = match String.get hd 1 with
  | '<' -> (<)
  | _ -> (>) in
  match String.drop_prefix hd 2 |> String.split ~on:':' with
  | [num; key] -> let required = Int.of_string num in
  (`Action {xmas; required; op; key}) :: (process_workflow tl)
  | _ -> failwith "invalid workflow"

let rec try_workflow ~input = function
| [] -> failwith "empty workflow"
| hd :: tl -> match hd with
  | `Default key -> `Next key 
  | `Action workflow -> 
    let xmas = match workflow.xmas with
      | X -> input.x
      | M -> input.m
      | A -> input.a
      | S -> input.s
    in
      if workflow.op xmas workflow.required 
      then `Next workflow.key
      else try_workflow ~input tl

let rec process_input input ~workflows ~key =
  match key with
  | "A" -> input.x + input.m + input.a + input.s
  | "R" -> 0
  | _ ->
    match try_workflow ~input (Hashtbl.find_exn workflows key) with
    | `Accepted -> input.x + input.m + input.a + input.s
    | `Rejected -> 0
    | `Next key -> process_input input ~workflows ~key

let format_input lines = 
  let workflow_tbl = String.Table.create () in
  let (workflows, inputs) = 
  List.split_while lines ~f:(fun s -> not (String.is_empty s)) in
  List.iter workflows ~f:(fun s ->
    let (key, thingo) = String.lsplit2_exn s ~on:'{' in
    let data = String.strip thingo ~drop:(fun c -> Char.(c = '}'))
    |> String.split ~on:',' 
    |> process_workflow in
    Hashtbl.set workflow_tbl ~key ~data);
  let inputs = List.drop inputs 1 in
  let inputs = List.map inputs ~f:(fun s ->
    match String.strip s ~drop:(fun c -> Char.(c = '}'))
    |> String.split_on_chars ~on:[',';'=']
    |> List.filteri ~f:(fun i _ -> i % 2 = 1)
    |> List.map ~f:Int.of_string with
    | [x; m; a; s] -> {x; m; a; s}
    | _ -> failwith "error on xmas input") in
  (workflow_tbl, inputs)

let () =
let (workflows, inputs) = In_channel.input_lines stdin |> format_input in
List.sum (module Int) inputs ~f:(process_input ~workflows ~key:"in")
|> Int.to_string
|> Out_channel.print_endline