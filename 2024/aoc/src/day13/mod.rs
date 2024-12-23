use nalgebra::{Matrix2, Vector2};

struct Machine {
    a: (usize, usize),
    b: (usize, usize),
    prize: (usize, usize),
}

fn parse_machine(input: &str) -> Machine {
    let mut lines = input.lines();
    let a = lines.next().expect("No button A in input");
    let b = lines.next().expect("No button B in input");
    let prize = lines.next().expect("No prize in input");

    let parse_two_ints = |line: &str| -> (usize, usize) {
        let mut it = line
            .split("+")
            .flat_map(|s| s.split("="))
            .flat_map(|s| s.split(","))
            .map(|s| s.parse::<usize>())
            .filter_map(|e| e.ok());
        (it.next().unwrap(), it.next().unwrap())
    };

    Machine {
        a: parse_two_ints(a),
        b: parse_two_ints(b),
        prize: parse_two_ints(prize),
    }
}

fn fix_machine(machine: Machine) -> Machine {
    const PRIZE_OFFSET: usize = 10000000000000;
    let (prize_x, prize_y) = machine.prize;
    Machine {
        a: machine.a,
        b: machine.b,
        prize: (prize_x + PRIZE_OFFSET, prize_y + PRIZE_OFFSET),
    }
}

/*
fn fewest_tokens(machine: Machine) -> Option<usize> {
    (0..101)
        .flat_map(|a| (0..101).map(move |b| (a, b)))
        .filter(|&(a, b)| {
            let (x, y) = machine.prize;
            let (xa, ya) = machine.a;
            let (xb, yb) = machine.b;
            xa * a + xb * b == x && ya * a + yb * b == y
        })
        .map(|(a, b)| a * 3 + b)
        .min()
}
*/

fn fewest_tokens(machine: Machine) -> Option<usize> {
    let (ax, ay) = machine.a;
    let (bx, by) = machine.b;
    let (prize_x, prize_y) = machine.prize;
    let matrix = Matrix2::new(ax as f64, bx as f64, ay as f64, by as f64);
    let prize = Vector2::new(prize_x as f64, prize_y as f64);
    let decomp = matrix.lu();
    let sol = decomp.solve(&prize)?;
    let (a, b): (usize, usize) = (sol[0].round() as usize, sol[1].round() as usize);
    if a * ax + b * bx == prize_x && a * ay + b * by == prize_y {
        Some(a * 3 + b)
    } else {
        None
    }
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    input
        .split("\n\n")
        .map(parse_machine)
        .filter_map(fewest_tokens)
        .sum::<usize>()
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    input
        .split("\n\n")
        .map(parse_machine)
        .map(fix_machine)
        .filter_map(fewest_tokens)
        .sum::<usize>()
}
