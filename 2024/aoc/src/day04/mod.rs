fn check(map: &[Vec<u8>], i: usize, j: usize, r: isize, c: isize) -> bool {
    let word = b"XMAS";
    word.into_iter().enumerate().all(|(m, &b)| {
        let i = (i as isize + (m as isize * r)) as usize;
        let j = (j as isize + (m as isize * c)) as usize;
        Some(b) == map.get(i).map(|v| v.get(j)).flatten().copied()
    })
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let map: Vec<Vec<u8>> = input.split('\n').map(|s| s.bytes().collect()).collect();
    let mut matches = 0;
    for i in 0..map.len() {
        for j in 0..map[i].len() {
            for r in -1..=1 {
                for c in -1..=1 {
                    if r == 0 && c == 0 {
                        continue;
                    }
                    matches += check(&map, i, j, r, c) as i32;
                }
            }
        }
    }
    matches
}

fn check_p2(map: &[Vec<u8>], i: usize, j: usize) -> bool {
    let a = map[i - 1][j - 1] == b'A';
    let tl = map[i - 2][j - 2];
    let tr = map[i - 2][j];
    let bl = map[i][j - 2];
    let br = map[i][j];
    a && ((tl == b'M' && br == b'S') || (tl == b'S' && br == b'M'))
        && ((tr == b'M' && bl == b'S') || (tr == b'S' && bl == b'M'))
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    let map: Vec<Vec<u8>> = input.split('\n').map(|s| s.bytes().collect()).collect();
    let mut matches = 0;
    for i in 2..map.len() {
        for j in 2..map[i].len() {
            matches += check_p2(&map, i, j) as i32;
        }
    }
    matches
}
