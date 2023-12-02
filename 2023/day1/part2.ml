open Base
open Stdio

let format_string str = 
  let patterns = [("one", "1"); ("two", "2"); ("three", "3"); ("four", "4");
  ("five", "5"); ("six", "6"); ("seven", "7"); ("eight", "8"); ("nine", "9")] in
  List.fold ~init:str patterns
  ~f:(fun acc (num, rep) -> String.Search_pattern.replace_all 
  (String.Search_pattern.create num) ~in_:acc ~with_:(num ^ rep ^ num) )

let first_digit str = String.find str ~f:(fun c -> Char.is_digit c)

let calibration str = 
  let formatted = format_string str in
  let digit1 = match first_digit formatted with
  | None -> failwith "no digit found"
  | Some x -> String.of_char x
in
  let digit2 = match first_digit (String.rev formatted) with
  | None -> failwith "no digit found"
  | Some x -> String.of_char x
in
  Int.of_string (digit1 ^ digit2)

let () =
let readings = In_channel.input_lines stdin in
let cal = List.fold readings ~init:0 ~f:(fun acc str -> acc + calibration str) in
Out_channel.output_string stdout (Int.to_string cal ^ "\n");
Out_channel.flush stdout