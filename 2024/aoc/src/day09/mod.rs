use itertools::Itertools;
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

// 12865833349546 too high
pub fn part2(input: &str) -> impl std::fmt::Display {
    let mut input = parse(input.as_bytes());
    let mut queues: [BinaryHeap<Reverse<usize>>; 9] = Default::default();
    input
        .iter()
        .copied()
        .enumerate()
        .chunk_by(|&(_, block)| block == Block::Free)
        .into_iter()
        .filter(|&(group, _)| group)
        .map(|(_, it)| it)
        .for_each(|mut it| {
            let idx = it.next().unwrap().0;
            queues[it.count()].push(Reverse(idx));
        });
    let mut take_at_least = |space| {
        let queue = queues[space..]
            .iter()
            .enumerate()
            .filter(|(_, q)| !q.is_empty())
            .map(|(i, q)| (i, q.peek().unwrap()))
            .min_by_key(|&(_, pos)| pos)
            .map(|(i, _)| i + space);
        if let Some(queue_index) = queue {
            if queue_index != space {
                let Reverse(pos) = queues[queue_index].peek().copied().unwrap();
                queues[queue_index - space - 1].push(Reverse(pos + space + 1));
            }
        }
        queue.map(|q| queues[q].pop()).flatten()
    };
    let mut seen = false;
    for i in (0..input.len()).rev() {
        if input[i] == Block::Free {
            seen = false;
            continue;
        } else if seen && input[i] == input[i + 1] {
            continue;
        }
        seen = true;
        let k = {
            let mut k = i;
            while k > 0 && input[k - 1] == input[i] {
                k -= 1;
            }
            k
        };
        if let Some(Reverse(j)) = take_at_least(i - k) {
            if j >= k {
                break;
            }
            for offset in 0..=i - k {
                input[j + offset] = input[k + offset];
                input[k + offset] = Block::Free;
            }
        }
    }
    println!("{input:?}");
    checksum(&input)
}
