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

  let dp = Hashtbl.create (module Int) in

  let key state =
    (state.num_i * 1000000) + (state.spring_i * 1000) + (state.count * 10) + (Char.to_int state.need)
  in

  let is_stored state =
    Hashtbl.mem dp (key state)
  in

  let stored_result_of state =
    Hashtbl.find_exn dp (key state)
  in

  let store state res =
    Hashtbl.set dp ~key:(key state) ~data:res
  in

  let rec helper state =
    if is_stored state then stored_result_of state else let res = (
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
    ) in
    store state res; res
  in
  helper { spring_i = 0; num_i = 0; count = 0; need = '?' }

let format_input = 
  List.map ~f:(fun line -> 
    match String.split line ~on:' ' with
    | [] | [_] | _ :: _ :: _ :: _ -> failwith "Invalid input"
    | [springs; nums] ->
      let springs = String.concat [springs; springs; springs; springs; springs] ~sep:"?" in
      let nums = String.concat [nums; nums; nums; nums; nums] ~sep:"," in
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