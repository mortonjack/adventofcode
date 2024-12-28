#[derive(Clone, Copy, Debug)]
enum Node {
    Wall,
    Box,
    Robot,
    Empty,
}

#[derive(Clone, Copy, Debug)]
enum Node2 {
    Wall,
    BoxLeft,
    BoxRight,
    Robot,
    Empty,
}

#[derive(Clone, Copy, Debug)]
enum Move {
    Up,
    Down,
    Left,
    Right,
}

impl Move {
    fn advance(self, r: usize, c: usize) -> (usize, usize) {
        match self {
            Move::Up => (r - 1, c),
            Move::Down => (r + 1, c),
            Move::Left => (r, c - 1),
            Move::Right => (r, c + 1),
        }
    }
}

fn parse_map(input: &str) -> Vec<Vec<Node>> {
    input
        .lines()
        .map(|line| {
            line.bytes()
                .map(|b| match b {
                    b'#' => Node::Wall,
                    b'O' => Node::Box,
                    b'.' => Node::Empty,
                    _ => Node::Robot,
                })
                .collect()
        })
        .collect()
}

fn parse_moves(input: &str) -> Vec<Move> {
    input
        .lines()
        .map(|line| line.bytes())
        .flatten()
        .map(|b| match b {
            b'^' => Move::Up,
            b'v' => Move::Down,
            b'<' => Move::Left,
            b'>' => Move::Right,
            _ => panic!("Unknown move"),
        })
        .collect()
}

fn parse(input: &str) -> (Vec<Vec<Node>>, Vec<Move>) {
    let mut it = input.split("\n\n");
    (
        parse_map(it.next().unwrap()),
        parse_moves(it.next().unwrap()),
    )
}

fn calc_answer(map: Vec<Vec<Node>>) -> usize {
    map.into_iter()
        .enumerate()
        .flat_map(|(i, row)| {
            row.into_iter()
                .enumerate()
                .filter(|&(_j, tile)| matches!(tile, Node::Box))
                .map(move |(j, _tile)| (i, j))
        })
        .map(|(i, j)| i * 100 + j)
        .sum::<usize>()
}

fn push(mut map: &mut [Vec<Node>], (r, c): (usize, usize), mv: Move) -> bool {
    let node = map[r][c];
    match node {
        Node::Wall => false,
        Node::Empty => true,
        Node::Robot | Node::Box if push(&mut map, mv.advance(r, c), mv) => {
            let (next_r, next_c) = mv.advance(r, c);
            map[next_r][next_c] = map[r][c];
            map[r][c] = Node::Empty;
            true
        }
        Node::Robot | Node::Box => false,
    }
}

fn simulate(mut map: Vec<Vec<Node>>, moves: &[Move], (r, c): (usize, usize)) -> usize {
    match moves {
        [] => calc_answer(map),
        [mv, mvs @ ..] => {
            let moved = push(&mut map, (r, c), *mv);
            simulate(map, mvs, if moved { mv.advance(r, c) } else { (r, c) })
        }
    }
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let (map, moves) = parse(input);
    let (r, c) = map
        .iter()
        .enumerate()
        .flat_map(|(i, row)| {
            row.iter()
                .copied()
                .enumerate()
                .filter(|&(_j, node)| matches!(node, Node::Robot))
                .map(move |(j, _node)| (i, j))
        })
        .next()
        .expect("No robot found");
    simulate(map, &moves, (r, c))
}

fn reshape(map: Vec<Vec<Node>>) -> Vec<Vec<Node2>> {
    map.into_iter()
        .map(|row| {
            row.into_iter()
                .map(|node| match node {
                    Node::Box => [Node2::BoxLeft, Node2::BoxRight],
                    Node::Robot => [Node2::Robot, Node2::Empty],
                    Node::Empty => [Node2::Empty, Node2::Empty],
                    Node::Wall => [Node2::Wall, Node2::Wall],
                })
                .flat_map(|arr| arr.into_iter())
                .collect()
        })
        .collect()
}

fn calc_answer2(map: Vec<Vec<Node2>>) -> usize {
    map.into_iter()
        .enumerate()
        .flat_map(|(i, row)| {
            row.into_iter()
                .enumerate()
                .filter(|&(_j, tile)| matches!(tile, Node2::BoxLeft))
                .map(move |(j, _tile)| (i, j))
        })
        .map(|(i, j)| i * 100 + j)
        .sum::<usize>()
}

fn push2(mut map: &mut [Vec<Node2>], (r, c): (usize, usize), mv: Move, mutate: bool) -> bool {
    let node = map[r][c];
    match node {
        Node2::Wall => false,
        Node2::Empty => true,
        Node2::BoxLeft | Node2::BoxRight if matches!(mv, Move::Up) || matches!(mv, Move::Down) => {
            let (lc, rc) = if matches!(node, Node2::BoxLeft) {
                (c, c + 1)
            } else {
                (c - 1, c)
            };
            if push2(&mut map, mv.advance(r, lc), mv, false)
                && push2(&mut map, mv.advance(r, rc), mv, false)
            {
                if mutate {
                    push2(&mut map, mv.advance(r, lc), mv, mutate);
                    push2(&mut map, mv.advance(r, rc), mv, mutate);
                    let (next_r, next_c) = mv.advance(r, lc);
                    map[next_r][next_c] = map[r][lc];
                    map[next_r][next_c + 1] = map[r][rc];
                    map[r][lc] = Node2::Empty;
                    map[r][rc] = Node2::Empty;
                }
                true
            } else {
                false
            }
        }
        Node2::Robot | Node2::BoxLeft | Node2::BoxRight
            if push2(&mut map, mv.advance(r, c), mv, mutate) =>
        {
            if mutate {
                let (next_r, next_c) = mv.advance(r, c);
                map[next_r][next_c] = map[r][c];
                map[r][c] = Node2::Empty;
            }
            true
        }
        Node2::Robot | Node2::BoxLeft | Node2::BoxRight => false,
    }
}

fn simulate2(mut map: Vec<Vec<Node2>>, moves: &[Move], (r, c): (usize, usize)) -> usize {
    match moves {
        [] => calc_answer2(map),
        [mv, mvs @ ..] => {
            let moved = push2(&mut map, (r, c), *mv, true);
            simulate2(map, mvs, if moved { mv.advance(r, c) } else { (r, c) })
        }
    }
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    let (map, moves) = parse(input);
    let map = reshape(map);
    let (r, c) = map
        .iter()
        .enumerate()
        .flat_map(|(i, row)| {
            row.iter()
                .copied()
                .enumerate()
                .filter(|&(_j, node)| matches!(node, Node2::Robot))
                .map(move |(j, _node)| (i, j))
        })
        .next()
        .expect("No robot found");
    simulate2(map, &moves, (r, c))
}
