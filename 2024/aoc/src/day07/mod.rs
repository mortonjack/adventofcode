#[derive(Clone, Debug, PartialEq, Eq)]
struct Input {
    value: u64,
    nums: Vec<u64>,
}

impl Input {
    fn from(input: &str) -> Self {
        let value = input.split(": ").next().unwrap().parse().unwrap();
        let nums = input
            .split(' ')
            .skip(1)
            .map(|x| x.parse().unwrap())
            .collect();
        Input { value, nums }
    }
}

fn possible(value: u64, ongoing: u64, nums: &[u64]) -> bool {
    if value < ongoing {
        false
    } else if nums.len() == 0 {
        ongoing == value
    } else {
        let add = || {
            ongoing
                .checked_add(nums[0])
                .map(|x| possible(value, x, &nums[1..]))
                .unwrap_or(false)
        };
        let mul = || {
            ongoing
                .checked_mul(nums[0])
                .map(|x| possible(value, x, &nums[1..]))
                .unwrap_or(false)
        };
        add() || mul()
    }
}

fn possible_p2(value: u64, ongoing: u64, nums: &[u64]) -> bool {
    if value < ongoing {
        false
    } else if nums.len() == 0 {
        ongoing == value
    } else {
        let add = || {
            ongoing
                .checked_add(nums[0])
                .map(|x| possible_p2(value, x, &nums[1..]))
                .unwrap_or(false)
        };
        let mul = || {
            ongoing
                .checked_mul(nums[0])
                .map(|x| possible_p2(value, x, &nums[1..]))
                .unwrap_or(false)
        };
        let concat = || {
            ongoing
                .checked_mul(10u64.pow(nums[0].ilog10() as u32 + 1) as u64)
                .map(|x| x.checked_add(nums[0]))
                .flatten()
                .map(|x| possible_p2(value, x, &nums[1..]))
                .unwrap_or(false)
        };
        add() || mul() || concat()
    }
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    input
        .lines()
        .map(Input::from)
        .filter(|input| possible(input.value, input.nums[0], &input.nums[1..]))
        .map(|x| x.value)
        .sum::<u64>()
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    input
        .lines()
        .map(Input::from)
        .filter(|input| possible_p2(input.value, input.nums[0], &input.nums[1..]))
        .map(|x| x.value)
        .sum::<u64>()
}
