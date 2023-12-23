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
  let th = ref false in
  let sv = ref false in
  let ch = ref false in
  let gh = ref false in
  let new_signals = List.fold ~init:[] signal.outputs ~f:(fun acc key ->
    let x = Hashtbl.find_exn module_table key in
    (if String.(key = "th") && not signal.pulse then th := true else ());
    (if String.(key = "sv") && not signal.pulse then sv := true else ());
    (if String.(key = "ch") && not signal.pulse then ch := true else ());
    (if String.(key = "gh") && not signal.pulse then gh := true else ());
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
  ) in ((!th, !sv, !ch, !gh), new_signals)

let combine_rx (a1, b1, c1, d1) (a2, b2, c2, d2)
= (a1 || a2, b1 || b2, c1 || c2, d1 || d2)

let rec process_signals rx signals ~module_table = 
  if List.is_empty signals then rx else
    let (new_rx, new_sigs) = List.fold signals ~init:((false, false, false, false), [])
    ~f:(fun (rx, acc) signal -> let (new_rx, new_sigs) = process_signal signal ~module_table in
    (combine_rx rx new_rx, (new_sigs :: acc))) in
    let new_signals = List.concat new_sigs
    in process_signals (combine_rx rx new_rx) new_signals ~module_table

let press_button module_table = 
  let signals_to_send = [{pulse = false; outputs = ["broadcaster"]; from = "button"}] in
  process_signals (false, false, false, false) signals_to_send ~module_table
let choose_between somenum bool count = 
  match somenum with
  | Some _ -> somenum
  | None -> if bool then Some count else None

let rec press_button_999999k_times ~th ~sv ~gh ~ch ~count module_table =
  (* th, sv, gh, ch *)
  let (nth, nsv, ngh, nch) = press_button module_table in
  let th = choose_between th nth count in
  let sv = choose_between sv nsv count in
  let gh = choose_between gh ngh count in
  let ch = choose_between ch nch count in
  match th, sv, gh, ch with
  | Some a, Some b, Some c, Some d -> Out_channel.printf "%d %d %d %d\n" a b c d
  | _ -> press_button_999999k_times ~th ~sv ~gh ~ch ~count:(count+1) module_table

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
|> press_button_999999k_times ~th:None ~sv:None ~gh:None ~ch:None ~count:1