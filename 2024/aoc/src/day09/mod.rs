use std::cmp::Reverse;
use std::collections::BinaryHeap;
use std::num::NonZeroU32;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Block {
    Free,
    Id(NonZeroU32),
}

fn checksum(blocks: &[Block]) -> u64 {
    blocks
        .iter()
        .enumerate()
        .map(|(pos, &block)| match block {
            Block::Free => 0,
            Block::Id(id) => (id.get() as u64 - 1) * pos as u64,
        })
        .sum()
}

fn parse(input: &[u8]) -> Vec<Block> {
    let mut ans = Vec::with_capacity(input.len() * 9);
    let mut block = true;
    let mut id = NonZeroU32::MIN;
    for size in input.iter().copied().map(|x| x - b'0') {
        for _ in 0..size {
            ans.push(if block { Block::Id(id) } else { Block::Free })
        }
        if block {
            id = id.saturating_add(1);
        }
        block = !block;
    }
    ans
}

pub fn part1(input: &str) -> impl std::fmt::Display {
    let mut input = parse(input.as_bytes());
    let mut pq: BinaryHeap<_> = input
        .iter()
        .copied()
        .enumerate()
        .filter(|&(_, x)| x == Block::Free)
        .map(|(i, _)| Reverse(i))
        .collect();
    for i in (0..input.len()).rev() {
        if input[i] == Block::Free {
            continue;
        }
        if let Some(Reverse(j)) = pq.pop() {
            if j >= i {
                break;
            }
            input[j] = input[i];
            input[i] = Block::Free;
        } else {
            break;
        }
    }
    checksum(&input)
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum SparseBlock {
    Free(u32),
    Id(u32, u8),
}

fn checksum_p2(blocks: impl Iterator<Item = SparseBlock>) -> u64 {
    blocks
        .fold((0, 0), |(pos, sum), block| match block {
            SparseBlock::Free(count) => (pos + count as u64, sum),
            SparseBlock::Id(id, count) => (
                pos + count as u64,
                sum + (pos..pos + count as u64)
                    .map(|pos| pos * id as u64)
                    .sum::<u64>(),
            ),
        })
        .1
}

fn parse_p2(input: &[u8]) -> Vec<SparseBlock> {
    input
        .iter()
        .copied()
        .enumerate()
        .map(|(i, b)| match i % 2 == 0 {
            true => SparseBlock::Id(i as u32 / 2, b - b'0'),
            false => SparseBlock::Free(b as u32 - b'0' as u32),
        })
        .collect()
}

// slower than my heap impl but this one works
pub fn part2(input: &str) -> impl std::fmt::Display {
    let mut input = parse_p2(input.as_bytes());
    input.reverse();
    let mut i = 0;
    while i < input.len() {
        if let SparseBlock::Id(id, length) = input[i] {
            input[i] = SparseBlock::Free(length as u32);
            for j in (0..input.len()).rev() {
                if let SparseBlock::Free(free_length) = input[j] {
                    use std::cmp::Ordering;
                    match free_length.cmp(&(length as u32)) {
                        Ordering::Less => (),
                        Ordering::Equal => {
                            input[j] = SparseBlock::Id(id, length);
                            break;
                        }
                        Ordering::Greater => {
                            input.insert(j + 1, SparseBlock::Id(id, length));
                            input[j] = SparseBlock::Free(free_length - length as u32);
                            break;
                        }
                    };
                }
            }
        }
        i += 1;
    }
    checksum_p2(input.into_iter().rev())
}
