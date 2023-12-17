# advent_of_code_2023
My C++20 solutions to advent of code 2023 puzzles

# Some C++ traps I encountered this year...

# day03

* I fell into a signed and unsigned compare again!

This loop will do nothing when c is negative (which happens for the start coordinate 0,0)

        for (int c = first_token.pos.col - 1; c <= first_token.pos.col + first_token.label.size(); ++c) {
          std::cout << " c: " << c;
          frame.push_back({r,c});
        }

The problem is that the left hand side expression is cast to unsigned due to using teh result of size() which is size_t, i.e., unsigned.

Then the whole comparison is made "as if" the left hand side is also unsigned. And '-1' uses twos complement so when interpreted as unsigned it is a very large number!

For me this seems to be the "most vexing untyped int" trap of all (so easy to fall into)!

# day09

* I got the semantics of std::adjacent_difference wrong.

I expected it to write to the output the difference of a two value window of the input starting at (begin,begin+1) and end at (last-1,last).
In effect producing one less value that in the input!
Instead it uses a first window of (begin,begin) and generates a copy of *begin! It also places the result from window (begin,begin+1) at output+1! *sigh*

I found that out after reading the specification.

"If [first, last) is not empty, computes the differences between the second and the first of each adjacent pair of its elements and writes the differences to the range beginning at d_first + 1. An unmodified copy of *first is written to *d_first"

Ok, as usual there may be a good reason for it behaves like this. But it violated the sematics of "adjacent difference" that I found natural...

* I used unsigned integer in looping down and failed to terminate the loop on i>=0

    for (auto row = result.size() - 2; row >= 0; --row)

    This loop will never end as size() returns a size_t which is unsigned, causing --row to overflow on 0 so the break condition >=0 will always be TRUE!!
* I used auto from a size() call and fed into a 

#day10

* I got the semantics of std::adjacent_difference wrong again (this time I got the order of the passed argument to the lambda wrong)

This code does NOT create a key with the first and second character from the input string.

      std::string crossing_ops{};
      std::adjacent_difference(enclosure_model[row].begin(), enclosure_model[row].end(), std::back_inserter(crossing_ops), [](char c1, char c2) {
        std::string key{};
        key += c1;
        key += c2;
        std::map<std::string,char> CROSSING_OPS{
           {"|.",'2'} // plain crossing
          ,{"L-",'+'} // incoming from above
          ,{"F-",'+'} // incoming from below
          ,{"-J",'-'} // outgoing to above
          ,{"-7",'+'} // outgoing to below
        };

To get the result I expected I need to assemble the key by adding c2 and then c1!

        key += c2;
        key += c1;

An reading the specification it "kind of" states that the passed argument are (second,first) but I missed it!

* computes the differences between the second and the first of each adjacent pair *

This caused me to spend at least an hour trying to understand why I failed to get any matches for the key!
