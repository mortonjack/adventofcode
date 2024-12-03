use regex::Regex;

fn search(input: &str) -> i64 {
    let re = Regex::new(r"mul\((\d{1,3}),(\d{1,3})\)").unwrap();
    re.captures_iter(input)
        .map(|s| s.extract())
        .map(|(_full, [a, b])| a.parse::<i64>().unwrap() * b.parse::<i64>().unwrap())
        .sum::<i64>()
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    search(input)
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    input
        .split_inclusive("do()")
        .flat_map(|i| i.split_inclusive("don't()"))
        .fold((true, 0), |(enabled, sum), s| {
            let disabled = s.ends_with("don't()");
            if enabled {
                (!disabled, sum + search(s))
            } else {
                (!disabled, sum)
            }
        })
        .1
}
