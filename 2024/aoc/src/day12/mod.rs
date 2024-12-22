struct DSU {
    row_size: usize,
    parent: Vec<usize>,
    size: Vec<usize>,
    perimeter: Vec<usize>,
}

impl DSU {
    fn new(rows: usize, cols: usize) -> Self {
        let row_size = cols;
        let vec_size = rows * cols;
        let parent = (0..vec_size).collect();
        let size = vec![1; vec_size];
        let perimeter = vec![0; vec_size];

        Self {
            row_size,
            parent,
            size,
            perimeter,
        }
    }

    fn key(&self, r: usize, c: usize) -> usize {
        r * self.row_size + c
    }

    fn root(&mut self, key: usize) -> usize {
        if key == self.parent[key] {
            key
        } else {
            self.parent[key] = self.root(self.parent[key]);
            self.parent[key]
        }
    }

    fn merge(&mut self, (r1, c1): (usize, usize), (r2, c2): (usize, usize)) {
        let mut k1 = self.root(self.key(r1, c1));
        let mut k2 = self.root(self.key(r2, c2));
        if k1 == k2 {
            return;
        }

        if self.size[k1] < self.size[k2] {
            std::mem::swap(&mut k1, &mut k2);
        }

        self.size[k1] += self.size[k2];
        self.perimeter[k1] += self.perimeter[k2];
        self.parent[k2] = k1;
    }

    fn add_perimeter(&mut self, r: usize, c: usize) {
        let root = self.root(self.key(r, c));
        self.perimeter[root] += 1;
    }

    fn price(&self) -> usize {
        self.parent
            .iter()
            .copied()
            .enumerate()
            .filter(|&(i, root)| i == root)
            .map(|(_, root)| root)
            .map(|key| (self.perimeter[key], self.size[key]))
            .map(|(perimeter, size)| perimeter * size)
            .sum()
    }
}

fn parse(input: &str) -> Vec<Vec<u8>> {
    input
        .lines()
        .map(|line| line.bytes().map(|b| b - b'A').collect())
        .collect()
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let map = parse(input);
    let mut dsu = DSU::new(map.len(), map[0].len());

    for (i, j, curr) in map
        .iter()
        .enumerate()
        .flat_map(|(i, row)| row.iter().copied().enumerate().map(move |(j, b)| (i, j, b)))
    {
        let mut process = |r: usize, c: usize| {
            if let Some(adj) = map.get(r).and_then(|row| row.get(c)).copied() {
                if adj == curr {
                    dsu.merge((i, j), (r, c));
                } else {
                    dsu.add_perimeter(i, j);
                }
            } else {
                dsu.add_perimeter(i, j);
            }
        };

        process(i + 1, j);
        process(i, j + 1);
        process(i.wrapping_sub(1), j);
        process(i, j.wrapping_sub(1));
    }

    dsu.price()
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Dir {
    Up,
    Down,
    Left,
    Right,
}

impl Dir {
    fn advance(self, r: usize, c: usize) -> (usize, usize) {
        match self {
            Dir::Up => (r.wrapping_sub(1), c),
            Dir::Down => (r + 1, c),
            Dir::Left => (r, c.wrapping_sub(1)),
            Dir::Right => (r, c + 1),
        }
    }
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    let map = parse(input);
    let mut dsu = DSU::new(map.len(), map[0].len());

    let get = |r: usize, c: usize| -> Option<u8> { map.get(r).and_then(|row| row.get(c)).copied() };

    for (i, j, curr) in map
        .iter()
        .enumerate()
        .flat_map(|(i, row)| row.iter().copied().enumerate().map(move |(j, b)| (i, j, b)))
    {
        let mut process = |wall_dir: Dir, region_dir: Dir| {
            let (r, c) = wall_dir.advance(i, j);
            if let Some(adj) = get(r, c) {
                if adj == curr {
                    dsu.merge((i, j), (r, c));
                    return;
                }
            };

            let (r, c) = region_dir.advance(i, j);
            let (r2, c2) = wall_dir.advance(r, c);
            if let Some(adj) = get(r, c) {
                if let Some(adj2) = get(r2, c2) {
                    if adj == curr && adj != adj2 {
                        return;
                    }
                } else if adj == curr {
                    return;
                }
            }

            dsu.add_perimeter(i, j);
        };

        process(Dir::Up, Dir::Left);
        process(Dir::Left, Dir::Up);
        process(Dir::Right, Dir::Up);
        process(Dir::Down, Dir::Left);
    }

    dsu.price()
}
