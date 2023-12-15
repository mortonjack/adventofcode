open! Core
open Stdio

type row = {
  springs: string;
  nums: int array;
}

type state = {
  spring_i: int;
  num_i: int;
  count: int;
  need: char;
}

let calc_line { springs; nums } = 
  let valid_char state = 
    let c = String.get springs state.spring_i in
    if Char.equal c '#' && state.num_i >= Array.length nums 
    then false else Char.(c = '?' || c = state.need || state.need = '?')
  in
  let completed num_i count = 
    nums.(num_i) = count
  in

  let state_of spring_i =
    match String.get springs spring_i with
    | '#' -> `Damaged
    | '.' -> `Operational
    | '?' -> `Unknown
    | _ -> failwith "unknown char in spring"
  in

  let damaged state =
    if completed state.num_i (state.count+1)
    then { spring_i = state.spring_i + 1; num_i = state.num_i + 1; count = 0; need = '.' }
    else { spring_i = state.spring_i + 1; num_i = state.num_i; count = state.count + 1; need = '#'}
  in
  
  let operational state = { state with spring_i = state.spring_i + 1; need = '?' } in

  let is_valid state = 
    if state.num_i = Array.length nums && state.count = 0
    then 1 else 0
  in

  let rec helper state =
    if state.spring_i >= String.length springs then is_valid state else
    if valid_char state then 
      match state_of state.spring_i with
      | `Damaged -> if Char.(state.need = '.') || state.num_i >= Array.length nums
        then 0 else helper (damaged state)
      | `Operational -> if Char.(state.need = '#') then 0 else helper (operational state)
      | `Unknown -> (if Char.(state.need = '#') then 0 else helper (operational state)) +
      if state.num_i < Array.length nums && not Char.(state.need = '.') 
        then helper (damaged state) else 0
    else 0
  in
  helper { spring_i = 0; num_i = 0; count = 0; need = '?' }

let format_input = 
  List.map ~f:(fun line -> 
    match String.split line ~on:' ' with
    | [] | [_] | _ :: _ :: _ :: _ -> failwith "Invalid input"
    | [springs; nums] ->
      let nums = 
        List.map (String.split nums ~on:',') ~f:Int.of_string
        |> List.to_array
    in {springs; nums})

let () =
format_input (In_channel.input_lines stdin)
|> List.map ~f:calc_line
|> List.sum (module Int) ~f:Fn.id
|> Int.to_string
|> Out_channel.print_endline