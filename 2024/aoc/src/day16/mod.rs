use std::cmp::Reverse;
use std::collections::BinaryHeap;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Dir {
    North,
    South,
    East,
    West,
}

impl Dir {
    fn opposite(self) -> Self {
        match self {
            Dir::North => Dir::South,
            Dir::South => Dir::North,
            Dir::East => Dir::West,
            Dir::West => Dir::East,
        }
    }

    fn clockwise(self) -> Self {
        match self {
            Dir::North => Dir::East,
            Dir::East => Dir::South,
            Dir::South => Dir::West,
            Dir::West => Dir::North,
        }
    }
}

#[derive(Clone, Copy, Debug)]
enum Tile {
    Empty,
    Wall,
    Start,
    End,
}

#[derive(Clone, Copy, Debug)]
struct Pos {
    r: usize,
    c: usize,
    moves: usize,
    direction: Dir,
}

#[derive(Debug)]
struct DirMap {
    north: Option<Pos>,
    south: Option<Pos>,
    east: Option<Pos>,
    west: Option<Pos>,
}

impl DirMap {
    fn new() -> Self {
        Self {
            north: None,
            south: None,
            east: None,
            west: None,
        }
    }
}

impl std::ops::IndexMut<Dir> for DirMap {
    fn index_mut(&mut self, index: Dir) -> &mut Self::Output {
        match index {
            Dir::North => &mut self.north,
            Dir::South => &mut self.south,
            Dir::East => &mut self.east,
            Dir::West => &mut self.west,
        }
    }
}

impl std::ops::Index<Dir> for DirMap {
    type Output = Option<Pos>;

    fn index(&self, index: Dir) -> &Self::Output {
        match index {
            Dir::North => &self.north,
            Dir::South => &self.south,
            Dir::East => &self.east,
            Dir::West => &self.west,
        }
    }
}

impl Pos {
    fn advance(self) -> Self {
        let (r, c) = match self.direction {
            Dir::North => (self.r - 1, self.c),
            Dir::South => (self.r + 1, self.c),
            Dir::West => (self.r, self.c - 1),
            Dir::East => (self.r, self.c + 1),
        };
        let moves = self.moves + 1;
        let direction = self.direction;
        Self {
            r,
            c,
            moves,
            direction,
        }
    }

    fn rotate(self, dir: Dir) -> Self {
        let moves = self.moves
            + match (dir, self.direction) {
                (d1, d2) if d1 == d2.opposite() => 2000,
                (d1, d2) if d1 == d2 => 0,
                _ => 1000,
            };

        Self {
            r: self.r,
            c: self.c,
            direction: dir,
            moves,
        }
    }

    fn key(&self) -> Reverse<isize> {
        let distance = self.r as isize - self.c as isize;
        let dir = match self.direction {
            Dir::North | Dir::East => 0,
            Dir::South | Dir::West => 1000,
        };
        let moves = self.moves as isize;
        Reverse(distance + dir + moves)
    }

    fn coords(&self) -> (usize, usize) {
        (self.r, self.c)
    }
}

impl PartialEq for Pos {
    fn eq(&self, other: &Self) -> bool {
        self.key().eq(&other.key())
    }
}

impl Eq for Pos {}

impl PartialOrd for Pos {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        self.key().partial_cmp(&other.key())
    }
}

impl Ord for Pos {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.key().cmp(&other.key())
    }
}

fn parse(input: &str) -> Vec<Vec<Tile>> {
    input
        .lines()
        .map(|s| {
            s.bytes()
                .map(|b| match b {
                    b'#' => Tile::Wall,
                    b'.' => Tile::Empty,
                    b'S' => Tile::Start,
                    b'E' => Tile::End,
                    _ => panic!("Unknown tile form"),
                })
                .collect()
        })
        .collect()
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let mut map = parse(input);
    let mut pq = BinaryHeap::new();
    pq.push(Pos {
        r: map.len() - 2,
        c: 1,
        direction: Dir::East,
        moves: 0,
    });

    loop {
        let pos = pq.pop().expect("Could not reach the end of the maze");

        if matches!(map[pos.r][pos.c], Tile::Wall) {
            continue;
        }
        map[pos.r][pos.c] = Tile::Wall;

        if pos.r == 1 && pos.c == map[pos.r].len() - 2 {
            return pos.moves;
        }

        let mut add = |dir: Dir| pq.push(pos.rotate(dir).advance());

        add(Dir::North);
        add(Dir::South);
        add(Dir::East);
        add(Dir::West);
    }
}

fn dfs(pos: Pos, mut map: &mut [Vec<Tile>], mut path: &mut [Vec<DirMap>]) -> () {
    let (r, c) = Pos {
        r: pos.r,
        c: pos.c,
        moves: pos.moves,
        direction: pos.direction.opposite(),
    }
    .advance()
    .coords();

    map[r][c] = Tile::Empty;

    let mut check_direction = |direction: Dir, cost: usize| {
        if let Some(incoming) = path[r][c][direction] {
            if incoming.moves + cost == pos.moves {
                path[r][c][direction] = None;
                dfs(incoming, &mut map, &mut path)
            }
        }
    };

    check_direction(pos.direction, 1);
    check_direction(pos.direction.clockwise(), 1001);
    check_direction(pos.direction.clockwise().opposite(), 1001);
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    let mut map = parse(input);
    let mut visited: Vec<Vec<_>> = map
        .iter()
        .map(|r| r.iter().map(|_| DirMap::new()).collect())
        .collect();
    let mut pq = BinaryHeap::new();
    pq.push(Pos {
        r: map.len() - 2,
        c: 1,
        direction: Dir::East,
        moves: 0,
    });

    while let Some(pos) = pq.pop() {
        if matches!(map[pos.r][pos.c], Tile::Wall) {
            continue;
        }
        if visited[pos.r][pos.c][pos.direction].is_some() {
            continue;
        }
        visited[pos.r][pos.c][pos.direction] = Some(pos);

        let mut add = |dir: Dir| pq.push(pos.rotate(dir).advance());

        add(Dir::North);
        add(Dir::South);
        add(Dir::East);
        add(Dir::West);
    }

    let (end_r, end_c) = (1usize, map[0].len() - 2);
    let end = &visited[end_r][end_c];
    let optimal_moves = end
        .north
        .map(|pos| {
            let north_moves = pos.moves;
            end.east
                .map(|pos| north_moves.min(pos.moves))
                .unwrap_or(north_moves)
        })
        .unwrap_or_else(|| end.east.unwrap().moves);
    map.iter_mut()
        .flat_map(|row| row.iter_mut())
        .for_each(|tile| *tile = Tile::Wall);
    dfs(
        Pos {
            r: end_r - 1,
            c: end_c,
            moves: optimal_moves + 1,
            direction: Dir::North,
        },
        &mut map,
        &mut visited,
    );
    dfs(
        Pos {
            r: end_r,
            c: end_c + 1,
            moves: optimal_moves + 1,
            direction: Dir::East,
        },
        &mut map,
        &mut visited,
    );

    // we must 'trim off the edges' since we'll overstep the origin - hence skip
    map.into_iter()
        .rev()
        .skip(1)
        .flat_map(|row| row.into_iter().skip(1))
        .filter(|&tile| matches!(tile, Tile::Empty))
        .count()
}
