open Base
open Stdio

let rec extract_num str i =
  if (String.length str) = i then ""
  else if not (Char.is_digit (String.get str i)) then ""
  else (Char.to_string (String.get str i)) ^ (extract_num str (i+1))

let rec num_len str i =
  if (String.length str) = (i+1) then i
  else if not (Char.is_digit (String.get str i)) then i
  else num_len str (i+1)

let is_symbol c =
  not (Char.is_digit c || Char.equal c '.')

let rec find_symbol former curr next l r =
  if is_symbol (String.get former l) || is_symbol (String.get curr l) ||  is_symbol (String.get next l)
    then true 
  else if l = r then false 
  else find_symbol former curr next (l+1) r

let rec extract_line_nums former curr next i = 
  if i >= (String.length curr) then [] else
  if (not (Char.is_digit (String.get curr i))) then
    extract_line_nums former curr next (i+1)
  else 
    let r = num_len curr i in
    let l = if i = 0 then i else i-1 in
    let num = Int.of_string (extract_num curr i) in
    if (find_symbol former curr next l r) then num :: extract_line_nums former curr next (r+1)
    else extract_line_nums former curr next (i+1)


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