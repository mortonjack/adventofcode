open! Core
open Stdio

type xmas = X | M | A | S
type op = Lt | Gte
type workflow = {
  xmas : xmas;
  required : int;
  op : op;
  key : string
}
type range = {
  (* [lo, hi) *)
  lo : int;
  hi : int
}
type input = {
  x : range;
  m : range;
  a : range;
  s : range
}
let dfl_range = { lo = 1; hi = 4001 }
let dfl_input = { x = dfl_range; m = dfl_range; a = dfl_range; s = dfl_range }

let is_valid_range range = 
  range.lo < range.hi

let is_valid_xmas xmas =
  is_valid_range xmas.x
  && is_valid_range xmas.m 
  && is_valid_range xmas.a
  && is_valid_range xmas.s

let combinations range = 
  (range.x.hi - range.x.lo)
  * (range.m.hi - range.m.lo)
  * (range.a.hi - range.a.lo)
  * (range.s.hi - range.s.lo)

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
  let (op, plus) = match String.get hd 1 with
  | '<' -> Lt, 0
  | _ -> Gte, 1 in
  match String.drop_prefix hd 2 |> String.split ~on:':' with
  | [num; key] -> let required = Int.of_string num + plus in
  (`Action {xmas; required; op; key}) :: (process_workflow tl)
  | _ -> failwith "invalid workflow"

let rec try_workflow ~input = function
| [] -> failwith "empty workflow"
| hd :: tl -> match hd with
  | `Default key -> [(key, input)] 
  | `Action workflow -> 
    let (hi_range, lo_range) = match workflow.xmas with
      | X -> ({input with x = {input.x with lo = max workflow.required input.x.lo}}, 
              {input with x = {input.x with hi = min workflow.required input.x.hi}})
      | M -> ({input with m = {input.m with lo = max workflow.required input.m.lo}}, 
              {input with m = {input.m with hi = min workflow.required input.m.hi}})
      | A -> ({input with a = {input.a with lo = max workflow.required input.a.lo}}, 
              {input with a = {input.a with hi = min workflow.required input.a.hi}})
      | S -> ({input with s = {input.s with lo = max workflow.required input.s.lo}}, 
              {input with s = {input.s with hi = min workflow.required input.s.hi}})
    in
      match workflow.op with
      | Lt -> let remaining = if is_valid_xmas hi_range 
        then try_workflow ~input:hi_range tl else [] in
        if is_valid_xmas lo_range then (workflow.key, lo_range) :: remaining else remaining
      | Gte -> let remaining = if is_valid_xmas lo_range 
        then try_workflow ~input:lo_range tl else [] in
        if is_valid_xmas hi_range then (workflow.key, hi_range) :: remaining else remaining

let rec count_combinations workflows ~key ~input =
  match key with
  | "A" -> combinations input
  | "R" -> 0
  | _ ->
    let ranges = try_workflow ~input (Hashtbl.find_exn workflows key) in
    List.sum (module Int) ranges ~f:(fun (key, input) -> count_combinations ~input workflows ~key)

let format_input lines = 
  let workflow_tbl = String.Table.create () in
  let (workflows, _) = 
  List.split_while lines ~f:(fun s -> not (String.is_empty s)) in
  List.iter workflows ~f:(fun s ->
    let (key, thingo) = String.lsplit2_exn s ~on:'{' in
    let data = String.strip thingo ~drop:(fun c -> Char.(c = '}'))
    |> String.split ~on:',' 
    |> process_workflow in
    Hashtbl.set workflow_tbl ~key ~data);
  workflow_tbl

let () =
In_channel.input_lines stdin 
|> format_input
|> count_combinations ~key:"in" ~input:dfl_input
|> Int.to_string
|> Out_channel.print_endline