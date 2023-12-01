open Base
open Stdio

let patterns = [("one", "1"); ("two", "2"); ("three", "3"); ("four", "4");
("five", "5"); ("six", "6"); ("seven", "7"); ("eight", "8"); ("nine", "9")]

let format_string str = List.fold ~init:str patterns
  ~f:(fun acc (num, rep) -> String.Search_pattern.replace_all (String.Search_pattern.create num) ~in_:acc ~with_:(num ^ rep ^ num) )

let first_digit str = String.find str ~f:(fun c -> Char.(c >= '0' && c <= '9'))

let calibration str = 
  let digit1 = match first_digit str with
  | None -> failwith "no digit found"
  | Some x -> String.of_char x
in
  let digit2 = match first_digit (String.rev str) with
  | None -> failwith "no digit found"
  | Some x -> String.of_char x
in
  Int.of_string (digit1 ^ digit2)

let () =
let readings = In_channel.input_lines stdin in
let cal = List.fold readings ~init:0 ~f:(fun acc str -> acc + (calibration (format_string str))) in
Out_channel.output_string stdout (String.concat [(Int.to_string cal); "\n"]);
Out_channel.flush stdout