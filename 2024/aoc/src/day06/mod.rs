type Map = Vec<Vec<u8>>;
type MapRef<'a> = &'a [Vec<u8>];
type MapRefMut<'a> = &'a mut [Vec<u8>];

type Map2 = Vec<Vec<Node>>;
type MapRefMut2<'a> = &'a mut [Vec<Node>];

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
#[repr(u8)]
enum Dir {
    Up = 1 << 0,
    Down = 1 << 1,
    Left = 1 << 2,
    Right = 1 << 3,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
struct Node {
    data: u8,
}

impl Dir {
    fn next_dir(self) -> Self {
        match self {
            Dir::Up => Dir::Right,
            Dir::Right => Dir::Down,
            Dir::Down => Dir::Left,
            Dir::Left => Dir::Up,
        }
    }

    fn next(self, (row, col): (usize, usize)) -> (usize, usize) {
        match self {
            Dir::Up => (row.wrapping_sub(1), col),
            Dir::Right => (row, col + 1),
            Dir::Down => (row + 1, col),
            Dir::Left => (row, col.wrapping_sub(1)),
        }
    }
}

impl Node {
    pub fn from(c: u8) -> Self {
        let data = match c {
            b'.' => 0,
            b'#' => 1 << 4,
            b'^' => 1 << 6,
            _ => panic!("Unknown input token: {} or {}", c, c as char),
        };
        Self { data }
    }

    fn set_dir(&mut self, dir: Dir) {
        self.data |= dir as u8;
    }

    fn unset_dir(&mut self, dir: Dir) {
        self.data &= !(dir as u8);
    }

    fn check_dir(self, dir: Dir) -> bool {
        self.data & (dir as u8) != 0
    }

    fn set_barrier(&mut self) {
        self.data |= 1 << 4
    }

    fn unset_barrier(&mut self) {
        self.data &= !(1 << 4);
    }

    fn check_barrier(self) -> bool {
        self.data & (1 << 4) != 0
    }

    fn set_makeshift_barrier(&mut self) {
        self.data |= 1 << 5;
    }

    fn check_makeshift_barrier(self) -> bool {
        self.data & (1 << 5) != 0
    }

    fn check_start(self) -> bool {
        self.data & (1 << 6) != 0
    }

    fn can_makeshift_barrier(self) -> bool {
        !self.check_makeshift_barrier()
            && !self.check_dir(Dir::Up)
            && !self.check_dir(Dir::Down)
            && !self.check_dir(Dir::Left)
            && !self.check_dir(Dir::Right)
    }
}

fn starting_pos(map: MapRef) -> (usize, usize) {
    for i in 0..map.len() {
        for j in 0..map[i].len() {
            if map[i][j] == b'^' {
                return (i, j);
            }
        }
    }
    panic!("No starting position found");
}

fn process(map: MapRefMut, (row, col): (usize, usize), dir: Dir) {
    map[row][col] = b'X';
    let in_bounds = |(r, c): (usize, usize)| r < map.len() && c < map[r].len();
    let (next_row, next_col) = dir.next((row, col));
    if in_bounds((next_row, next_col)) {
        if map[next_row][next_col] == b'#' {
            process(map, (row, col), dir.next_dir());
        } else {
            process(map, (next_row, next_col), dir);
        }
    }
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let mut map: Map = input.lines().map(|s| s.bytes().collect()).collect();
    let start = starting_pos(&map);
    let dir = Dir::Up;

    process(&mut map, start, dir);

    map.into_iter().flatten().filter(|&x| x == b'X').count()
}

fn check_loop(map: MapRefMut2, (row, col): (usize, usize), dir: Dir) -> bool {
    if map[row][col].check_dir(dir) {
        return true;
    }
    map[row][col].set_dir(dir);
    let in_bounds = |(r, c): (usize, usize)| r < map.len() && c < map[r].len();
    let (next_row, next_col) = dir.next((row, col));
    let retval = if in_bounds((next_row, next_col)) {
        if map[next_row][next_col].check_barrier() {
            check_loop(map, (row, col), dir.next_dir())
        } else {
            check_loop(map, (next_row, next_col), dir)
        }
    } else {
        false
    };
    map[row][col].unset_dir(dir);
    retval
}

fn process_p2(map: MapRefMut2, (row, col): (usize, usize), dir: Dir) {
    map[row][col].set_dir(dir);
    let in_bounds = |(r, c): (usize, usize)| r < map.len() && c < map[r].len();
    let (next_row, next_col) = dir.next((row, col));
    if in_bounds((next_row, next_col)) {
        if map[next_row][next_col].check_barrier() {
            process_p2(map, (row, col), dir.next_dir());
        } else {
            // Try placing a barrier ahead of us
            if map[next_row][next_col].can_makeshift_barrier() {
                map[next_row][next_col].set_barrier();
                if check_loop(map, (row, col), dir.next_dir()) {
                    map[next_row][next_col].set_makeshift_barrier();
                }
                map[next_row][next_col].unset_barrier();
            }
            process_p2(map, (next_row, next_col), dir);
        }
    }
}

// 1812 too high
pub fn part2(input: &str) -> impl std::fmt::Display {
    let map: Map = input.lines().map(|s| s.bytes().collect()).collect();
    let start = starting_pos(&map);
    let mut map: Map2 = map
        .into_iter()
        .map(|x| x.into_iter().map(Node::from).collect())
        .collect();
    let dir = Dir::Up;

    process_p2(&mut map, start, dir);

    map.into_iter()
        .flatten()
        .filter(|x| x.check_makeshift_barrier() && !x.check_start())
        .count()
}
