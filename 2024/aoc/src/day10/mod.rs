use std::collections::HashSet;

fn parse(input: &str) -> Vec<Vec<u8>> {
    input
        .lines()
        .map(|x| x.as_bytes().iter().copied().map(|x| x - b'0').collect())
        .collect()
}

fn get_scores(grid: &[Vec<u8>]) -> Vec<Vec<HashSet<(usize, usize)>>> {
    let mut scores = vec![vec![HashSet::new(); grid[0].len()]; grid.len()];
    let row_size = grid.len();
    let col_size = grid[0].len();
    for (height, i, j) in (0..10)
        .rev()
        .map(|height| (0..row_size).map(move |i| (0..col_size).map(move |j| (height, i, j))))
        .flatten()
        .flatten()
        .filter(|&(height, i, j)| grid[i][j] == height)
    {
        if height == 9 {
            scores[i][j].insert((i, j));
            continue;
        }
        let mut add = |r: usize, c: usize| {
            if let Some(grid_height) = grid.get(r).map(|g: &Vec<u8>| g.get(c)).flatten().copied() {
                if grid_height == height + 1 {
                    scores[i][j] = scores[i][j].union(&scores[r][c]).copied().collect();
                }
            }
        };
        add(i + 1, j);
        add(i.wrapping_sub(1), j);
        add(i, j + 1);
        add(i, j.wrapping_sub(1));
    }
    scores
}

fn get_ratings(grid: &[Vec<u8>]) -> Vec<Vec<usize>> {
    let mut scores = vec![vec![0; grid[0].len()]; grid.len()];
    let row_size = grid.len();
    let col_size = grid[0].len();
    for (height, i, j) in (0..10)
        .rev()
        .map(|height| (0..row_size).map(move |i| (0..col_size).map(move |j| (height, i, j))))
        .flatten()
        .flatten()
        .filter(|&(height, i, j)| grid[i][j] == height)
    {
        if height == 9 {
            scores[i][j] = 1;
            continue;
        }
        let mut add = |r: usize, c: usize| {
            if let Some(grid_height) = grid.get(r).map(|g: &Vec<u8>| g.get(c)).flatten().copied() {
                if grid_height == height + 1 {
                    scores[i][j] += scores[r][c];
                }
            }
        };
        add(i + 1, j);
        add(i.wrapping_sub(1), j);
        add(i, j + 1);
        add(i, j.wrapping_sub(1));
    }
    scores
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let grid = parse(input);
    let scores = get_scores(&grid);
    (0..grid.len())
        .map(|i| (0..grid[i].len()).map(move |j| (i, j)))
        .flatten()
        .filter(|&(i, j)| grid[i][j] == 0)
        .map(|(i, j)| scores[i][j].len())
        .sum::<usize>()
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    let grid = parse(input);
    let ratings = get_ratings(&grid);
    (0..grid.len())
        .map(|i| (0..grid[i].len()).map(move |j| (i, j)))
        .flatten()
        .filter(|&(i, j)| grid[i][j] == 0)
        .map(|(i, j)| ratings[i][j])
        .sum::<usize>()
}
