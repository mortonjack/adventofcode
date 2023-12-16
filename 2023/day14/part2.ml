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

let output rocks = Array.iter rocks ~f:(fun row -> Array.iter row ~f:(fun x -> 
  match x with
  | Cube -> printf "#"
  | Rounded -> printf "O"
  | Empty -> printf "."
  ); print_endline ""); print_endline ""

let cycle rocks = 
  rotate rocks North;
  rotate rocks West;
  rotate rocks South;
  rotate rocks East;
  output rocks

let () =
let rocks = In_channel.input_lines stdin |> format_input in
cycle rocks;
cycle rocks;
cycle rocks;
cycle rocks;
cycle rocks;
cycle rocks;
cycle rocks;
cycle rocks;
cycle rocks