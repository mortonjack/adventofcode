use num::Integer;
use std::collections::{HashMap, HashSet};

type Map = HashMap<u8, Vec<(isize, isize)>>;

fn parse(input: &[u8]) -> (Map, isize, isize) {
    let mut ans = Map::new();
    let mut rows = 0;
    let mut cols = 0;
    for (i, line) in input.split(|&b| b == b'\n').enumerate() {
        rows += 1;
        cols = line.len() as isize;
        for (j, b) in line.iter().copied().enumerate() {
            if b != b'.' {
                ans.entry(b).or_default().push((i as isize, j as isize));
            }
        }
    }
    (ans, rows, cols)
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let (map, rows, cols) = parse(input.as_bytes());
    let mut antinodes = HashSet::new();
    for vec in map.into_values() {
        for (i, (r1, c1)) in vec.iter().copied().enumerate() {
            for (r2, c2) in vec[i + 1..].iter().copied() {
                antinodes.insert((r1 + (r1 - r2), c1 + (c1 - c2)));
                antinodes.insert((r2 - (r1 - r2), c2 - (c1 - c2)));
            }
        }
    }
    antinodes
        .into_iter()
        .filter(|&(x, y)| x >= 0 && y >= 0 && x < rows && y < cols)
        .count()
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    let (map, rows, cols) = parse(input.as_bytes());
    let mut antinodes = HashSet::new();
    let mut insert = |r_base, c_base, r_diff, c_diff| {
        let mut r = r_base;
        let mut c = c_base;
        while r < rows && r >= 0 && c < cols && c >= 0 {
            antinodes.insert((r, c));
            r += r_diff;
            c += c_diff;
        }
        r = r_base - r_diff;
        c = c_base - c_diff;
        while r < rows && r >= 0 && c < cols && c >= 0 {
            antinodes.insert((r, c));
            r -= r_diff;
            c -= c_diff;
        }
    };
    for vec in map.into_values() {
        for (i, (r1, c1)) in vec.iter().copied().enumerate() {
            for (r2, c2) in vec[i + 1..].iter().copied() {
                let r_diff = r1 - r2;
                let c_diff = c1 - c2;
                let gcd = r_diff.gcd(&c_diff);
                insert(r1, c1, r_diff / gcd, c_diff / gcd);
            }
        }
    }
    antinodes.len()
}
