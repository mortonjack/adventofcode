use std::ops::Range;

use num::integer::lcm;

struct Robot {
    x: usize,
    y: usize,
    vx: usize,
    vy: usize,
}

fn parse_four_ints(line: &str) -> (i8, i8, i8, i8) {
    let mut it = line
        .split("+")
        .flat_map(|s| s.split("="))
        .flat_map(|s| s.split(","))
        .flat_map(|s| s.split(" "))
        .map(|s| s.parse::<i8>())
        .filter_map(|e| e.ok());
    (
        it.next().expect("Could not find position x in line"),
        it.next().expect("Could not find position y in line"),
        it.next().expect("Could not find velocity x in line"),
        it.next().expect("Could not find velocity y in line"),
    )
}

fn parse_robot(line: &str, r: usize, c: usize) -> Robot {
    let (x, y, vx, vy) = parse_four_ints(line);
    let x = x as usize;
    let y = y as usize;
    let vx = vx.rem_euclid(c as i8) as usize;
    let vy = vy.rem_euclid(r as i8) as usize;
    Robot { x, y, vx, vy }
}

fn final_position(robot: Robot, r: usize, c: usize, iterations: usize) -> (usize, usize) {
    let iterations = iterations % lcm(lcm(r, c), lcm(robot.vx as usize, robot.vy as usize));
    let transform = |pos: usize, velocity: usize, limit: usize| -> usize {
        (pos + velocity * iterations) % limit
    };
    (
        transform(robot.y, robot.vy, r),
        transform(robot.x, robot.vx, c),
    )
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let (r, c) = if input.len() < 240 {
        (7, 11)
    } else {
        (103, 101)
    };
    let mut map = vec![vec![0; c]; r];
    input
        .lines()
        .map(|line| parse_robot(line, r, c))
        .map(|robot| final_position(robot, r, c, 100))
        .for_each(|(i, j)| map[i][j] += 1);
    let mr = r / 2;
    let mc = c / 2;
    let flat_iter = |(rows, cols): (Range<usize>, Range<usize>)| {
        rows.flat_map(move |row| cols.clone().map(move |col| (row, col)))
    };
    let robots_in_quadrant = |quadrant| {
        flat_iter(quadrant)
            .map(|(i, j)| map[i][j] as usize)
            .sum::<usize>()
    };
    let q1 = (0..mr, 0..mc);
    let q2 = (0..mr, mc + 1..c);
    let q3 = (mr + 1..r, 0..mc);
    let q4 = (mr + 1..r, mc + 1..c);
    let quadrants = [q1, q2, q3, q4];
    quadrants
        .into_iter()
        .map(robots_in_quadrant)
        .product::<usize>()
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    let (r, c) = if input.len() < 240 {
        (7, 11)
    } else {
        (103, 101)
    };

    let mut map = vec![vec![0; c]; r];
    (0..)
        .filter(|&iterations| {
            for row in map.iter_mut() {
                for cell in row.iter_mut() {
                    *cell = 0;
                }
            }
            input
                .lines()
                .map(|line| parse_robot(line, r, c))
                .map(|robot| final_position(robot, r, c, iterations))
                .for_each(|(i, j)| map[i][j] += 1);
            map.iter()
                .flat_map(|row| row.iter().copied())
                .all(|h| h <= 1)
        })
        .next()
        .unwrap()
}
