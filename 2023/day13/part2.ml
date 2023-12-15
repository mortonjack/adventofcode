open! Core
open Stdio

type pattern = {
  rows : string list;
  cols : string list;
}
let string_diff s1 s2 = 
  let s1 = String.to_list s1 in
  let s2 = String.to_list s2 in
  List.fold ~init:0 (List.zip_exn s1 s2) ~f:(fun acc (c1, c2) -> if Char.(c1 = c2) then acc else acc+1)

let rows_to_cols rows = 
  List.fold rows ~init:(String.to_list (List.hd_exn rows) |> List.map ~f:(fun _ -> ""))
  ~f:(fun acc row ->
    List.mapi acc ~f:(fun i acc -> acc ^ Char.to_string (String.get row i)))

let rec seperate_patterns pattern_acc = function
| [] -> [pattern_acc]
| hd :: tl -> 
  if String.is_empty hd then pattern_acc :: seperate_patterns [] tl
  else seperate_patterns (pattern_acc @ [hd]) tl

let format_input lines =
  let seperate = seperate_patterns [] lines in  
  List.map seperate ~f:(fun pattern ->
    let rows = pattern in
    let cols = rows_to_cols rows in
    {rows; cols})

let rec reflection left right curr_diff = 
  match left, right with
  | _, [] | [], _ -> if curr_diff = 1 then Some (List.length left) else None
  | lhd :: ltl, rhd :: rtl ->
    let str_diff = string_diff lhd rhd in
    if str_diff + curr_diff <= 1 then 
      match reflection ltl rtl (str_diff + curr_diff) with
      | Some x -> Some (x + 1)
      | None -> None
    else None

let rec try_reflect left right =
  match right with
  | [] -> None
  | hd :: tl -> 
    let left = hd :: left in
    let right = tl in
    if List.is_empty right then None else
    match reflection left right 0 with
    | Some _ as ans -> ans
    | None -> try_reflect left right

let calc_reflection pattern =
  match try_reflect [] pattern.rows with
  | Some x -> 100 * x
  | None -> match try_reflect [] pattern.cols with
    | Some x -> x
    | None -> failwith "No reflection found"

let () =
format_input (In_channel.input_lines stdin)
|> List.map ~f:calc_reflection
|> List.sum (module Int) ~f:Fn.id
|> Int.to_string
|> Out_channel.print_endline