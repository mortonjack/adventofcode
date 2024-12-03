use itertools::Itertools;

fn parse(input: &str) -> Vec<Vec<u8>> {
    input
        .split('\n')
        .map(|l| {
            l.split_whitespace()
                .map(|x| x.parse::<u8>().unwrap())
                .collect()
        })
        .collect()
}

fn check_valid(report: impl std::iter::Iterator<Item = u8> + Clone) -> bool {
    let increasing = report.clone().tuple_windows().all(|(a, b)| a < b);
    let decreasing = report.clone().tuple_windows().all(|(a, b)| a > b);
    let in_range = report.tuple_windows().all(|(a, b)| u8::abs_diff(a, b) <= 3);
    (increasing || decreasing) && in_range
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let input = parse(input);
    input
        .into_iter()
        .filter(|v| check_valid(v.iter().copied()))
        .count()
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    let input = parse(input);
    input
        .into_iter()
        .filter(|v| {
            (0..=v.len()).any(|i| {
                let iter = v.iter().take(i).chain(v.iter().skip(i + 1)).copied();
                check_valid(iter.clone()) || check_valid(iter.rev())
            })
        })
        .count()
}
