# Part 2
Part 2 takes *forever* to complete. It never actually completed on my machine. I just got
bored of waiting and input the current maximum and it worked. I tried optimising it with 
a hash map in a previous commit but it was hungrier than a protogen eating up all my RAM.
Excited to look at other solutions.

# Update 1 - January 2022
My program now runs in ~60 seconds! I was able to get my dynamic programming solution 
working properly by having the elephant start "after" me, rather than taking it in turns.
The DFS search remains the same, but now when memoising the result, I only need to pay
attention to the current node & time remaining, the visited nodes, and who's turn it is.
The elephant and I no longer need to worry about where the other is at!

# Update 2 - January 2022
I got the time down to under 30 seconds (beating my goal of <1min!) by using an integer
for the key, rather than a string, drastically improving hashmap lookup times. I got
another 10 seconds off the time by realising I was making copies of a couple vectors
in the dfs function rather than passing by reference :p
