open! Core
open Stdio

type gate = Broadcaster | Flipflop of bool | Conjunction of (string, bool) Hashtbl.t | Output
type module' = {
  gate : gate;
  outputs: string list
}
type signal = {
  from : string;
  pulse : bool;
  outputs : string list
}

let process_signal signal ~module_table =
  List.fold ~init:[] signal.outputs ~f:(fun acc key ->
    let x = Hashtbl.find_exn module_table key in
    match x.gate with
    | Output -> acc
    | Broadcaster -> { signal with outputs = x.outputs; from = key } :: acc
    | Flipflop on -> if signal.pulse then acc else (
      Hashtbl.set module_table ~key ~data:{x with gate = Flipflop (not on)};
      { pulse = not on; outputs = x.outputs; from = key } :: acc )
    | Conjunction prev_signals -> (
      (Hashtbl.set prev_signals ~key:signal.from ~data:signal.pulse);
      let output = Hashtbl.exists prev_signals ~f:(fun b -> not b) in
      { from = key; pulse = output; outputs = x.outputs } :: acc)
  )

let rec process_signals (lo_cnt, hi_cnt) signals ~module_table = 
  if List.is_empty signals then (lo_cnt, hi_cnt) else
    let new_signals = 
      List.fold signals ~init:[] 
      ~f:(fun acc signal -> process_signal signal ~module_table :: acc)
      |> List.concat
    in let (lo_cnt, hi_cnt) = 
      List.fold new_signals ~init:(lo_cnt, hi_cnt)
      ~f:(fun (lo_cnt, hi_cnt) signal -> 
        if signal.pulse 
          then (lo_cnt, hi_cnt + List.length signal.outputs) 
          else (lo_cnt + List.length signal.outputs, hi_cnt))
    in process_signals (lo_cnt, hi_cnt) new_signals ~module_table

let press_button module_table = 
  let signals_to_send = [{pulse = false; outputs = ["broadcaster"]; from = "button"}] in
  process_signals (1, 0) signals_to_send ~module_table

let rec press_button_1k_times ~count module_table =
  if count = 0 then (0, 0)
  else 
    let (c1, c2) = press_button module_table in
    let (c3, c4) = press_button_1k_times ~count:(count-1) module_table in
    (c1+c3, c2+c4)

let process_output (c1, c2) = 
  c1 * c2
  |> Int.to_string

let format_input lines = 
  let module_table = Hashtbl.create (module String) in
  List.iter lines ~f:(fun s ->
      let gate = match String.get s 0 with
        | 'b' -> Broadcaster
        | '&' -> Conjunction (Hashtbl.create (module String))
        | '%' -> Flipflop false
        | _ -> failwith "unknown gate type"
      in
      match String.filter s ~f:(fun c -> 
        not (Char.(is_whitespace c || c = '>' || c = '&' || c = '%')))
      |> String.split_on_chars ~on:['-';','] with
      | [] | [_] -> failwith "0/1 entry in input line (no output modules)"
      | key :: outputs ->
        Hashtbl.set module_table ~key ~data:{
          gate;
          outputs
        }
    );
    Hashtbl.fold module_table ~init:[] 
    ~f:(fun ~key:key ~data:{outputs; _} acc -> (key, outputs) :: acc)
    |> List.iter ~f:(fun (from, outputs) -> 
      List.iter outputs ~f:(fun key -> 
      match Hashtbl.find module_table key with
      | None -> Hashtbl.set module_table ~key ~data:{gate = Output; outputs = []}
      | Some x ->
        match x.gate with
        | Broadcaster | Flipflop _ | Output -> ();
        | Conjunction n -> 
          Hashtbl.set n ~key:from ~data:false
    ));
  module_table

let () =
In_channel.input_lines stdin
|> format_input
|> press_button_1k_times ~count:1000
|> process_output
|> Out_channel.print_endline