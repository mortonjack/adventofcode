use std::collections::HashMap;

fn count_stones(stone: u64, blinks: u8, mut dp: &mut HashMap<(u64, u8), u64>) -> u64 {
    if let Some(ans) = dp.get(&(stone, blinks)).copied() {
        return ans;
    }

    let ans = match stone {
        _ if blinks == 0 => 1,
        0 => count_stones(1, blinks - 1, &mut dp),
        _ if (stone.ilog10() + 1) % 2 == 0 => {
            let divisor = 10u64.pow((stone.ilog10() + 1) / 2);
            count_stones(stone % divisor, blinks - 1, &mut dp)
                + count_stones(stone / divisor, blinks - 1, &mut dp)
        }
        _ => count_stones(stone * 2024, blinks - 1, &mut dp),
    };

    dp.insert((stone, blinks), ans);
    ans
}

fn count_blinks(input: &str, blinks: u8) -> u64 {
    let mut cache = HashMap::new();
    input
        .split_whitespace()
        .map(|stone| {
            stone
                .parse::<u64>()
                .map(|stone| count_stones(stone, blinks, &mut cache))
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
