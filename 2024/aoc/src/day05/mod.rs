#[derive(Clone, Copy, Debug, PartialEq, Eq)]
struct Bitset {
    bitset: u128,
}
type Rules = [Bitset; 100];
type Updates = Vec<Vec<u8>>;

impl Bitset {
    pub fn new() -> Self {
        Bitset { bitset: 0 }
    }

    pub fn check(&self, pos: u32) -> bool {
        ((self.bitset >> pos) & 1) == 1
    }

    pub fn set(&mut self, pos: u32) {
        self.bitset |= 1u128 << pos;
    }

    pub fn unset(&mut self, pos: u32) {
        self.bitset &= !(1u128 << pos);
    }

    pub fn set_all(&mut self, other: Bitset) {
        self.bitset |= other.bitset;
    }
}

/*
fn dfs(mut checked: &mut Bitset, mut rules: &mut [Bitset; 100], page: u32) {
    if checked.check(page) {
        return;
    }
    checked.set(page);

    for new_page in 10..100 {
        if !rules[page as usize].check(new_page) {
            continue;
        }
        dfs(&mut checked, &mut rules, new_page);
        rules[page as usize].set_all(rules[new_page as usize]);
    }
}
*/

fn parse_rules(input: &str) -> Rules {
    // let mut checked = Bitset::new();
    let mut rules = [Bitset::new(); 100];
    for line in input.lines() {
        let mut it = line.split('|');
        let mut consume = || it.next().unwrap().parse::<u32>().unwrap();
        let before = consume();
        let after = consume();
        rules[after as usize].set(before);
    }
    // are you kidding me this isn't topological sort
    // (10..100).for_each(|page| dfs(&mut checked, &mut rules, page));
    rules
}

fn parse_updates(input: &str) -> Updates {
    input
        .lines()
        .map(|l| l.split(',').map(|x| x.parse().unwrap()).collect())
        .collect()
}

fn parse(input: &str) -> (Rules, Updates) {
    let mut it = input.split("\n\n");
    let rules_input = it.next().unwrap();
    let updates_input = it.next().unwrap();
    (parse_rules(rules_input), parse_updates(updates_input))
}

fn validate(rules: &Rules, update: &[u8]) -> bool {
    let mut seen = Bitset::new();
    update.iter().copied().all(|page| {
        let ans = seen.check(page as u32);
        seen.set_all(rules[page as usize]);
        !ans
    })
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let (rules, updates) = parse(input);
    updates
        .iter()
        .filter(|update| validate(&rules, update))
        .map(|update| update[update.len() / 2] as u32)
        .sum::<u32>()
}

fn sort(rules: &Rules, update: Vec<u8>) -> Vec<u8> {
    use std::collections::HashSet;
    let mut present = Bitset::new();
    update.iter().for_each(|&page| present.set(page as u32));
    let mut update: HashSet<_> = update.into_iter().collect();
    let mut ans = Vec::with_capacity(update.len());
    while !update.is_empty() {
        let next = update
            .iter()
            .copied()
            .min_by_key(|&page| (rules[page as usize].bitset & present.bitset).count_ones())
            .unwrap();
        update.remove(&next);
        present.unset(next as u32);
        ans.push(next);
    }
    ans
}

pub fn part2(input: &str) -> impl std::fmt::Display {
    let (rules, updates) = parse(input);
    updates
        .into_iter()
        .filter(|update| !validate(&rules, update))
        .map(|update| sort(&rules, update))
        .map(|update| update[update.len() / 2] as u32)
        .sum::<u32>()
}
