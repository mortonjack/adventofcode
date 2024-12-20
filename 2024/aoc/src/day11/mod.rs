use num::{One, Zero};
use num_bigint::BigUint;
use std::collections::HashMap;
use std::str::FromStr;

fn count_stones(key: (BigUint, u8), mut dp: &mut HashMap<(BigUint, u8), u64>) -> u64 {
    if let Some(ans) = dp.get(&key).copied() {
        ans
    } else {
        let (stone, blinks) = &key;
        let ans = if *blinks == 0 {
            1
        } else if *stone == BigUint::zero() {
            count_stones((BigUint::one(), blinks - 1), &mut dp)
        } else if stone.to_string().len() % 2 == 0 {
            let stringified = stone.to_string();
            let halfway = stringified.len() / 2;
            let left = BigUint::from_str(&stringified[..halfway]).unwrap();
            let right = BigUint::from_str(&stringified[halfway..]).unwrap();
            count_stones((left, blinks - 1), &mut dp) + count_stones((right, blinks - 1), &mut dp)
        } else {
            count_stones((stone * BigUint::from(2024u64), blinks - 1), &mut dp)
        };
        dp.insert(key, ans);
        ans
    }
}

fn count_blinks(input: &str, blinks: u8) -> u64 {
    let mut cache = HashMap::new();
    input
        .split_whitespace()
        .map(|stone| {
            stone
                .parse::<BigUint>()
                .map(|stone| count_stones((stone, blinks), &mut cache))
                .expect("Failed to parse integer from stdin")
        })
        .sum::<u64>()
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    count_blinks(input, 25)
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    count_blinks(input, 75)
}
