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
    use std::collections::HashMap;
    let (mut left, right) = parse(input);
    left.sort_unstable();
    let right: HashMap<u32, u32> = right.into_iter().fold(HashMap::new(), |mut acc, x| {
        *acc.entry(x).or_insert(0) += 1;
        acc
    });
    left.into_iter()
        .map(|x| x * right.get(&x).copied().unwrap_or(0))
        .sum::<u32>()
}
