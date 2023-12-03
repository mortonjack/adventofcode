open Base
open Stdio

let rec extract_num str i =
  if (String.length str) = i then ""
  else if not (Char.is_digit (String.get str i)) then ""
  else (Char.to_string (String.get str i)) ^ (extract_num str (i+1))

let rec num_start str i =
  if i < 0 || not (Char.is_digit (String.get str i)) then i+1
  else num_start str (i-1)

let find_nums_line str i =
  if Char.is_digit (String.get str i) then [extract_num str (num_start str i)]
  else let n1 = if not (i = 0) && Char.is_digit (String.get str (i-1)) then [extract_num str (num_start str (i-1))] else [] in
  if not ((i+1) = String.length str) && Char.is_digit (String.get str (i+1)) then (extract_num str (i+1)) :: n1 else n1

let find_nums former curr next i =
  let l1_nums = find_nums_line former i in
  let l2_nums = List.append l1_nums (find_nums_line curr i) in
  List.append l2_nums (find_nums_line next i)

let rec extract_line_nums former curr next i = 
  if i >= (String.length curr) then [] else
  if (not (Char.equal (String.get curr i) '*')) then
    extract_line_nums former curr next (i+1)
  else 
    match find_nums former curr next i with
    | [] | [_] | _ :: _ :: _ :: _ -> extract_line_nums former curr next (i+1)
    | [num1; num2] -> ((Int.of_string num1) * (Int.of_string num2)) :: extract_line_nums former curr next (i+1)


let rec get_numbers lines numbers = 
  match lines with
  | [] | [_] | [_; _] -> numbers
  | former :: curr :: next :: tl -> 
    get_numbers (curr :: next :: tl) (List.append (extract_line_nums former curr next 0) numbers)

let format_input input_lines = 
  let line_length = match input_lines with
  | [] -> failwith "no lines"
  | hd :: _ -> String.length hd in
  let empty_line = String.make line_length '.' in
  List.append (empty_line :: input_lines) [empty_line]

let () =
let lines = format_input (In_channel.input_lines stdin) in
let part_numbers = get_numbers lines [] in
let sum = Int.to_string (List.fold part_numbers ~init:0 ~f:(fun acc x -> acc + x)) in
Out_channel.print_endline sum