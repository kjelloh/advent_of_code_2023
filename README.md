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

# day05

* In part 2 I got bitten by an int that got sneaked in by CoPilot that translated some python code to C++ for me.

I *had* searched and replaced all 'int' to 'Integer' but *still* one remained for some obscure reason.

The code:

      std::vector<int> input_values;
      ...
      while (ss >> n) {
        std::cout << " value:" << n << std::flush;
        Integer value = std::stoll(n);
        if (value<0) {
          std::cout << NL << "ERROR: value < 0" << std::flush;
          exit(1);
        }
        input_values.push_back(value);
      }
      ...
      std::vector<std::pair<Integer, Integer>> seeds;
      for (Integer i = 0; i < input_values.size(); i += 2) {
        seeds.push_back({input_values[i], input_values[i] + input_values[i + 1]});
      }

Because input_values was still an vector of 'int' I got a silent overflow which made seeds push negative values!

The har truth to swallow is that I *did* suspect this, I *did* searhc and replace - and *still* this vector<int> remained in the code.

Hm... Maybe it is time to start using some sanitizer for C++ to detect these for me?


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

#day18

* Another hidden "int instead of Integer type" detour...

I had initially and high up in the source file (for part 1) defined:

    using Vector = std::tuple<int,int>;

When in fact, for part 2 with the much larger numbers I needed:

    using Vector = std::tuple<Integer,Integer>;

With,

    using Integer = int64_t; // 16 bit int: 3.27 x 10^4, 32 bit int: 2.14 x 10^9, 64 bit int: 9.22 x 10^18

Now, I h a d ensured I was using the larger Integer type! Or so I thought. But I missed the using declaration for Vector! 
And combined with all the "auto" in the code I was working on, I failed to discover how "int" in Vector sneaked in into my code...

Needless to say - I got no compiler warning either (as all my code used the larger Integer ). And compiled C++ code of course comes with no runtime checks ;)...

To add insult to missery I then somehow made and error copy-pasting the final answer and though I still had a bug.
When, in fact, I had the correct answer! It took me an hour to implement two other solutions from Github to compare.
And when they all returned my answer I tried again... 

#day19

* Silently got a boolean when expecting an integer and failed to understand why my find() did not work in parsing

I wrote this code:

      if (auto at = rule.find(":") != std::string::npos) {
        std::cout << " at:" << at << " rule.find(\":\") : " << rule.find(":") << std::flush;
        ...

And expected "at" to be the same as the result of the find. As if what I wrote  meant 
"assign 'at' to the result of find but enter the if clause only if find succeeded".

Of course this is not what the compiler understood it to mean. 
Reading it as the compiler does "at" is assigned the boolean result of the "!=" expression.

Now, because C++ treats a boolean as an integer I got nor warnings nor errors. And the 'at' and the find() call had different values.

What I should have written was of course:

      if (auto at = rule.find(":"); at != std::string::npos) {
        std::cout << " at:" << at << " rule.find(\":\") : " << rule.find(":") << std::flush;

I can't wait for teh C++ compiler that applies AI before compiling my code ;)

* C++ ridiculously long compiler error listings did not fit into the limits of VSCode output window

When solving this puzzle I change a type from std::map to std::vector<std::tuple<...>>. The consequence of this is of course
that all usage of objects "thing" of this type no longer could use thing[key].
I knew this but my strategy was to have the compiler help me find all those locations so I could attend to them.

But - because the C++ compiler so tediously listed all the rabbit wholes where the thing[key] did not conform with thing no longer
being an std::map, together with the window limitations of VSCode, meant I was unable to see where the compiler started complaining in my code!

You know what - I would like the compiler to list its complaints in the reverse order!
That is, if the compiler would list the compiler error in a stack-like manner, having the root of the error at the bottom
and then have ever deeper complaints above in the listing, this would make reading the compiler complaints much easier!

#day20

* Failed to realize that std::map::insert did not magically merge two maps with elements being std::vector<strings>

The code:

      for (auto const& [type,new_root] : new_roots) {
          auto sub_map = to_root(new_root,level-1,model);
          // merge sub level results
          level_map.insert(sub_map.begin(),sub_map.end());
          // add this level result
          level_map[level].push_back(std::format("{} <-- {}{}",root,type,new_root));
        }

Was mean to mean that the resulting map from the recursion should be merged into the vector of each matching key. Of course this is not what insert does.
In fact, it prevents overwriting existing keys! So, if sub_map contains listing for a key already in level_map, it is silently ignored.

Oh C++, complaining so much on compilation errors while generating code that is so silent on runtime errors...

To get code that "does what I mean" I need to write:

        for (auto const& [type,new_root] : new_roots) {
          auto sub_map = to_root(new_root,level-1,model);
          // merge sub level results into the listing on the each level
          for (auto const& [level,entries] : sub_map) {
            level_map[level].insert(level_map[level].end(),entries.begin(),entries.end());
          }
          // add this level result
          level_map[level].push_back(std::format("{} <-- {}{}",root,type,new_root));
        }
