open! Core
open Stdio

type cards = {
  hand: string;
  bid: int
}

let hand_type card =
  let card_list = String.to_list card.hand 
  |> List.sort ~compare:Char.compare in
  let (_, card_count, j_count) = List.fold card_list ~init:('X', [], 0) 
  ~f:(fun (prev, acc, j_count) card -> if Char.equal card 'J' then (prev, acc, j_count+1) else
    if Char.equal card prev then 
    match acc with 
  | hd :: tl -> card, (hd+1) :: tl, j_count
  | _ -> failwith "matching non-existent card"
  else (card, 1 :: acc, j_count)) in
  match List.sort card_count ~compare:Int.compare with
  | [_] -> 6
  | [_; n] -> if (n+j_count) = 4 then 5 else 4
  | [_; _; n] -> if (n+j_count) = 3 then 3 else 2
  | [_; _; _; _] -> 1
  | _ -> if j_count = 5 then 6 else 0

let beats c1 c2 = 
  let card_order = ['A'; 'K'; 'Q'; 'T'; '9'; '8'; '7'; '6'; '5'; '4'; '3'; '2'; 'J' ] in
  let (c1_rank,  _) = List.findi_exn card_order ~f:(fun _ c -> Char.equal c c1) in
  let (c2_rank,  _) = List.findi_exn card_order ~f:(fun _ c -> Char.equal c c2) in
  c1_rank < c2_rank

let rec compare_cards_directly card1 card2 =
  match card1, card2 with
  | c1 :: t1, c2 :: t2 -> if Char.equal c1 c2 then compare_cards_directly t1 t2
  else if beats c1 c2 then 1 else -1
  | _, _ -> failwith "cards the same"

let compare_hands card1 card2 = 
  let type1 = hand_type card1 in 
  let type2 = hand_type card2 in
  if type1 < type2 then -1 else if type1 > type2 then 1 else
  let card1_list = String.to_list card1.hand in 
  let card2_list = String.to_list card2.hand in
  compare_cards_directly card1_list card2_list

let format_input lines = 
  List.map lines ~f:(fun s ->
    let (hand, bid_str) = String.lsplit2_exn s ~on:' ' in
    let bid = Int.of_string bid_str in {hand; bid} )
  |> List.sort ~compare:compare_hands 

let find_ranks cards =
  List.mapi cards ~f:(fun i card -> (i+1) * card.bid)

let () = 
format_input (In_channel.input_lines stdin)
|> find_ranks
|> List.sum (module Int) ~f:Fn.id
|> Int.to_string
|> Out_channel.print_endline;
