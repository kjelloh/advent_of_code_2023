# advent_of_code_2023
My C++20 solutions to advent of code 2023 puzzles

# Some C++ traps I encountered this year...

day09

I got the semantics of std::adjacent_difference wrong.

I expected it to write to the output the difference of a two value window of the input starting at (begin,begin+1) and end at (last-1,last).
In effect producing one less value that in the input!
Instead it uses a first window of (begin,begin) and generates a copy of *begin! It also places the result from window (begin,begin+1) at output+1! *sigh*

I found that out after reading the specification.

"If [first, last) is not empty, computes the differences between the second and the first of each adjacent pair of its elements and writes the differences to the range beginning at d_first + 1. An unmodified copy of *first is written to *d_first"

Ok, as usuall there may be a good reason for it behaves like this. But it violated the sematics of "adjacent difference" that I found natural...