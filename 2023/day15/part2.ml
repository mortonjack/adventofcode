open! Core
open Stdio

type operation = 
| Minus
| Equals of int

type step = {
  label: string;
  hash: int;
  op: operation
}

let hash s =
  String.to_list s
  |> List.fold ~init:0 ~f:(fun acc c ->
    let n = Char.to_int c in
    ((acc + n) * 17) % 256)

let interpret_step s =
  match String.mem s '=' with
  | false -> 
    let label = String.drop_suffix s 1
    in {label; hash = hash label; op = Minus}
  | true -> let (label, num_str) = 
    String.lsplit2_exn s ~on:'=' in
    let num = Int.of_string num_str in
    { label; hash = hash label; op = Equals num}

let format_input lines = 
  String.concat lines
  |> String.split ~on:','
  |> List.map ~f:interpret_step

let make_step step boxes : unit =
  match step.op with
  | Minus -> boxes.(step.hash) <- List.filter boxes.(step.hash)
  ~f:(fun (label, _) -> not (String.equal label step.label))
  | Equals num -> 
    match List.exists boxes.(step.hash)
    ~f:(fun (label, _) -> String.equal label step.label) with
    | true -> boxes.(step.hash) <- 
    List.map boxes.(step.hash) ~f:(fun ((label, _) as curr) -> 
      if String.equal label step.label then (label, num) else curr)
    | false -> boxes.(step.hash) <- (step.label, num) :: boxes.(step.hash)

let rec make_steps steps boxes = 
  match steps with
  | [] -> ()
  | hd :: tl ->
    make_step hd boxes; make_steps tl boxes

let focusing_power = 
  Array.foldi ~init:0 ~f:(fun box_num acc box ->
    acc + 
    (box_num + 1) * (List.foldi box ~init:0 ~f:(fun slot acc (_, focal_length) -> 
      acc + focal_length * (List.length box - slot)))
    )

let () =
let steps = In_channel.input_lines stdin |> format_input in
let boxes = Array.init 256 ~f:(fun _ -> []) in 
make_steps steps boxes;
focusing_power boxes
|> Int.to_string 
|> Out_channel.print_endline