fn read_input() -> String {
    std::io::stdin().lines().map(|l| l.unwrap()).collect()
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    assert!(
        args.len() == 3,
        "Invalid format. Correct format: {} <day> <part>",
        args[0]
    );
    let day_message = "Day must be a number between 1-25";
    let part_message = "Part must be either 1 or 2";
    let day: usize = args[1].parse().expect(day_message);
    let part: usize = args[2].parse().expect(part_message);
    let input = read_input();

    // This will be annoying but whatever :/ why write a rust macro over a vim macro
    match (day, part) {
        (01, 1) => println!("{}", aoc::day01::part1(&input)),
        (01, 2) => println!("{}", aoc::day01::part2(&input)),
        (02, 1) => println!("{}", aoc::day02::part1(&input)),
        (02, 2) => println!("{}", aoc::day02::part2(&input)),
        (03, 1) => println!("{}", aoc::day03::part1(&input)),
        (03, 2) => println!("{}", aoc::day03::part2(&input)),
        (04, 1) => println!("{}", aoc::day04::part1(&input)),
        (04, 2) => println!("{}", aoc::day04::part2(&input)),
        (05, 1) => println!("{}", aoc::day05::part1(&input)),
        (05, 2) => println!("{}", aoc::day05::part2(&input)),
        (06, 1) => println!("{}", aoc::day06::part1(&input)),
        (06, 2) => println!("{}", aoc::day06::part2(&input)),
        (07, 1) => println!("{}", aoc::day07::part1(&input)),
        (07, 2) => println!("{}", aoc::day07::part2(&input)),
        (08, 1) => println!("{}", aoc::day08::part1(&input)),
        (08, 2) => println!("{}", aoc::day08::part2(&input)),
        (09, 1) => println!("{}", aoc::day09::part1(&input)),
        (09, 2) => println!("{}", aoc::day09::part2(&input)),
        (10, 1) => println!("{}", aoc::day10::part1(&input)),
        (10, 2) => println!("{}", aoc::day10::part2(&input)),
        (11, 1) => println!("{}", aoc::day11::part1(&input)),
        (11, 2) => println!("{}", aoc::day11::part2(&input)),
        (12, 1) => println!("{}", aoc::day12::part1(&input)),
        (12, 2) => println!("{}", aoc::day12::part2(&input)),
        (13, 1) => println!("{}", aoc::day13::part1(&input)),
        (13, 2) => println!("{}", aoc::day13::part2(&input)),
        (14, 1) => println!("{}", aoc::day14::part1(&input)),
        (14, 2) => println!("{}", aoc::day14::part2(&input)),
        (15, 1) => println!("{}", aoc::day15::part1(&input)),
        (15, 2) => println!("{}", aoc::day15::part2(&input)),
        (16, 1) => println!("{}", aoc::day16::part1(&input)),
        (16, 2) => println!("{}", aoc::day16::part2(&input)),
        (17, 1) => println!("{}", aoc::day17::part1(&input)),
        (17, 2) => println!("{}", aoc::day17::part2(&input)),
        (18, 1) => println!("{}", aoc::day18::part1(&input)),
        (18, 2) => println!("{}", aoc::day18::part2(&input)),
        (19, 1) => println!("{}", aoc::day19::part1(&input)),
        (19, 2) => println!("{}", aoc::day19::part2(&input)),
        (20, 1) => println!("{}", aoc::day20::part1(&input)),
        (20, 2) => println!("{}", aoc::day20::part2(&input)),
        (21, 1) => println!("{}", aoc::day21::part1(&input)),
        (21, 2) => println!("{}", aoc::day21::part2(&input)),
        (22, 1) => println!("{}", aoc::day22::part1(&input)),
        (22, 2) => println!("{}", aoc::day22::part2(&input)),
        (23, 1) => println!("{}", aoc::day23::part1(&input)),
        (23, 2) => println!("{}", aoc::day23::part2(&input)),
        (24, 1) => println!("{}", aoc::day24::part1(&input)),
        (24, 2) => println!("{}", aoc::day24::part2(&input)),
        (25, 1) => println!("{}", aoc::day25::part1(&input)),
        (25, 2) => println!("{}", aoc::day25::part2(&input)),
        _ => panic!(
            "Unknown day {day} or part {part}. Must be numbers between 1-25 and 1-2 respectively"
        ),
    }
}
