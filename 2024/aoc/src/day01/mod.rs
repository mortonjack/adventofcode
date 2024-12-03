use itertools::Itertools;

fn parse(input: &str) -> (Vec<u32>, Vec<u32>) {
    input
        .split('\n')
        .map(|line| {
            let mut it = line.split_whitespace().map(|s| s.parse::<u32>().unwrap());
            (it.next().unwrap(), it.next().unwrap())
        })
        .collect()
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let (mut left, mut right) = parse(input);
    left.sort_unstable();
    right.sort_unstable();
    left.into_iter()
        .zip(right)
        .map(|(l, r)| u32::abs_diff(l, r))
        .sum::<u32>()
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    let (mut left, right) = parse(input);
    left.sort_unstable();
    let right = right.into_iter().counts();
    left.into_iter()
        .map(|x| x as usize * right.get(&x).copied().unwrap_or(0))
        .sum::<usize>()
}
