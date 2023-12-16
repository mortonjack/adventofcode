open! Core
open Stdio

type rock = Rounded | Cube | Empty
type direction = North | South | East | West

let char_to_rock = function
| '#' -> Cube
| 'O' -> Rounded
| '.' -> Empty
| _ -> failwith "Invalid char in input"

let format_input lines = 
  List.map lines ~f:(fun line -> String.to_list line
    |> List.map ~f:char_to_rock
    |> List.to_array)
  |> List.to_array
  
let update rocks r c row col addr addc = 
  match rocks.(r).(c) with
  | Empty -> ()
  | Cube -> row := r + addr; col := c + addc;
  | Rounded ->
    (* Swap rounded w/ [row][col] and then return true *)
    (rocks.(r).(c) <- rocks.(!row).(!col);
    rocks.(!row).(!col) <- Rounded;
    row := !row + addr; col := !col + addc)

let rotate rocks ~rb ~re ~cb ~ce dir =
  let add = match dir with
    | North | West -> 1
    | South | East -> -1
  in
  match dir with
  | East | West -> 
    let r = ref rb in
    while !r <= re do
      let row = ref !r in
      let col = ref cb in
      let c = ref cb in
      while !c <> ce + add do
        update rocks !r !c row col 0 add;
        c := !c + add
      done;
      r := !r + 1
    done
  | North | South ->
    let c = ref cb in
    while !c <= ce do
      let row = ref rb in
      let col = ref !c in
      let r = ref rb in
      while !r <> re + add do
        update rocks !r !c row col add 0;
        r := !r + add
      done;
      c := !c + 1
    done

let rotate rocks direction =
  let row_end = Array.length rocks - 1 in
  let col_end = Array.length rocks.(0) - 1 in
  match direction with
  | North -> rotate rocks ~rb:0 ~re:row_end ~cb:0 ~ce:col_end North
  | South -> rotate rocks ~rb:row_end ~re:0 ~cb:0 ~ce:col_end South
  | West -> rotate rocks ~rb:0 ~re:row_end ~cb:0 ~ce:col_end West
  | East -> rotate rocks ~rb:0 ~re:row_end ~cb:col_end ~ce:0 East

let to_string rocks = 
  Array.map rocks ~f:(fun row -> Array.map row ~f:(fun x -> 
  match x with
  | Cube -> "#"
  | Rounded -> "O"
  | Empty -> "."
  ) |> Array.to_list |> String.concat) 
  |> Array.to_list
  |> String.concat ~sep:"\n"

let cycle rocks = 
  rotate rocks North;
  rotate rocks West;
  rotate rocks South;
  rotate rocks East

let rec cycle_x_more_times rocks to_go =
  if to_go = 0 then ()
  else (
    cycle rocks;
    cycle_x_more_times rocks (to_go-1))
let find_repeat rocks =
  let dp = Hashtbl.create (module String) in

  let is_repeat key =
    Hashtbl.mem dp key
  in

  let stored_result_of key =
    Hashtbl.find_exn dp key
  in

  let store key data =
    Hashtbl.set dp ~key ~data
  in

  let rec cycle_until_repeat rocks count = 
    let key = to_string rocks in
    if is_repeat key then
      let cycle_length = count - stored_result_of key in
      let remaining = 1000000000 - count in
      let to_go = remaining % cycle_length in
      cycle_x_more_times rocks to_go
    else (
      store key count; 
      cycle rocks; 
      cycle_until_repeat rocks (count+1)
    ) in

  cycle_until_repeat rocks 0

let sum_load rocks = 
  let sum = ref 0 in
  for c = 0 to Array.length rocks.(0) - 1 do
    for r = 0 to Array.length rocks - 1 do
      match rocks.(r).(c) with
      | Rounded -> sum := !sum + (Array.length rocks.(0)) - r
      | _ -> ()
    done
  done;
  !sum

let () =
let rocks = In_channel.input_lines stdin |> format_input in
find_repeat rocks;
sum_load rocks
|> Int.to_string
|> Out_channel.print_endline