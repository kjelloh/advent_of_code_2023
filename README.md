# advent_of_code_2023
My C++20 solutions to advent of code 2023 puzzles

# Some C++ traps I encountered this year...

# day03

## I fell into a signed and unsigned compare again!

This loop will do nothing when c is negative (which happens for the start coordinate 0,0)

        for (int c = first_token.pos.col - 1; c <= first_token.pos.col + first_token.label.size(); ++c) {
          std::cout << " c: " << c;
          frame.push_back({r,c});
        }

The problem is that the left hand side expression is cast to unsigned due to using teh result of size() which is size_t, i.e., unsigned.

Then the whole comparison is made "as if" the left hand side is also unsigned. And '-1' uses twos complement so when interpreted as unsigned it is a very large number!

For me this seems to be the "most vexing untyped int" trap of all (so easy to fall into)!

# day05

## In part 2 I got bitten by an int that got sneaked in by CoPilot that translated some python code to C++ for me.

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

## With LLM (for me CoPilot in VSCode) C++ programming has gotten so much better!

I asked CoPilot for recomended sanitizers and it told me about the flags to gcc and clang!

Gcc and clang supports:

        "-fsanitize=undefined",
        "-fsanitize=address",

Clang support the additional:

        "-fsanitize=integer",

It even told me what syntax to use in VSCode tasks.json :)

Come to think about it, this is of course exactly what we should expect a large language model to be good at!

As a final sugar on the cake CoPilot can also help me read the runtime error from code compiled with these flags
and direct me to what to change in my code to address the problem!

Programming has gotten so much easier now!

# day09

## I got the semantics of std::adjacent_difference wrong.

I expected it to write to the output the difference of a two value window of the input starting at (begin,begin+1) and end at (last-1,last).
In effect producing one less value that in the input!

Instead it uses a first window of (begin,begin) and generates a copy of element at begin! It also places the result from window (begin,begin+1) at output+1! *sigh*

I found that out after reading the specification.

"If [first, last) is not empty, computes the differences between the second and the first of each adjacent pair of its elements and writes the differences to the range beginning at d_first + 1. An unmodified copy of *first is written to *d_first"

Ok, as usual there may be a good reason for it behaves like this. But it violated the semantics of "adjacent difference" that I found natural...

## I used unsigned integer in looping down and failed to terminate the loop on i>=0

    for (auto row = result.size() - 2; row >= 0; --row)

    This loop will never end as size() returns a size_t which is unsigned, causing --row to overflow on 0 so the break condition >=0 will always be TRUE!!

#day10

## I got the semantics of std::adjacent_difference wrong again (this time I got the order of the passed argument to the lambda wrong)

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

**"computes the differences between the second and the first of each adjacent pair"**

This caused me to spend at least an hour trying to understand why I failed to get any matches for the key!

# day18

## Another hidden "int instead of Integer type" detour...

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

# day19

## Silently got a boolean when expecting an integer and failed to understand why my find() did not work in parsing

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

## C++ ridiculously long compiler error listings did not fit into the limits of VSCode output window

When solving this puzzle I change a type from std::map to std::vector<std::tuple<...>>. The consequence of this is of course
that all usage of objects "thing" of this type no longer could use thing[key].
I knew this but my strategy was to have the compiler help me find all those locations so I could attend to them.

But - because the C++ compiler so tediously listed all the rabbit wholes where the thing[key] did not conform with thing no longer
being an std::map, together with the window limitations of VSCode, meant I was unable to see where the compiler started complaining in my code!

You know what - I would like the compiler to list its complaints in the reverse order!
That is, if the compiler would list the compiler error in a stack-like manner, having the root of the error at the bottom
and then have ever deeper complaints above in the listing, this would make reading the compiler complaints much easier!

# day20

## Failed to realize that std::map::insert did not magically merge two maps with elements being std::vector<strings>

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

## Falsely believed an std::string_view::substr::data could be used to create an std::string from the sub-view.

The code:

        std::string name = left.substr(1).data();

So, data() remains to be the char* to the *original* string. It is not adjusted to reflect the substring operation!

So creating a string from an std::string_view::substr turns out to require some "magic" thinking.

You can't assign an std::string_view to a string:

        std::string name = left.substr(1); // Nope

But you *can* construct a string from a string_view!

        std::string name{left.substr(1)}; // ok

Took me a good half hour or more to debug my code before I realized it was the parser with the "std::string name = left.substr(1).data();" that was the culprit.

Again, the code compiled, and to mee, looked liked it was doing the right thing...

# day 23

## I first thought "icy" meant I could not stand there but would immediately slip to a tile nearby...

The puzzle states "...if you step onto a slope tile, your next step must be downhill (in the direction the arrow is pointing).".

So, from this I suppose it is clear I can stand on that tile. But that the next step must then be in the direction of the slope.

But, what first throw me off was "Because of all the mist from the waterfall, the slopes are probably quite icy;". 

To me, that meant the tile should behave as icy spots in real life? And if so, to step on an icy spot would cause you to slip and end up in a position next to the icy spot.

As it turned out, this was *not* the case. 

## I was reminded how factually correct compiler errors can be confusing and unhelpful if you don't have your "C++ matrix" glasses tuned

The code:

        struct Vector {
          int r, c;
          auto operator<=>(const Vector& lhs, const Vector& rhs) const {
            return std::tie(lhs.r, lhs.c) <=> std::tie(rhs.r, rhs.c);
          }
        };


Makes the compiler complain:

      /Users/kjell-olovhogdal/Documents/Github/advent_of_code_2023/day23/day23.cpp:222:8: error: overloaded 'operator<=>' must be a binary operator (has 3 parameters)
        222 |   bool operator<=>(const Vector& lhs, const Vector& rhs) const {

At first glance this is a bit confusing. Firstly it says "operator<=>" must be *binary operator* (taking two arguments). Which is understandable. But then, the distracted programmer may look at the signature and think, well, it takes two argument! It is already binary? What is the problem?

Then, the second part, "has 3 parameters", makes no sense? We can clearly see it takes two parameters!

Well, time to put on the C++ matrix glasses. And now we understand:

1. Every member function of a struct or class has a hidden "this" parameter. So, yes, we provided two arguments but the compiler provided the this-pointer as the third.
2. A binary member operator is defined to implicitly take "ourselves" as the left hand term and "the other" as an argument.

So, a seasoned (and alert) C++ programmer should read "overloaded 'operator<=>' must be a binary operator (has 3 parameters)" and correct the code to:

        struct Vector {
          int r, c;
          auto operator<=>(Vector const& other) const {
            return std::tie(this->r, this->c) <=> std::tie(other.r, other.c);
          }
        };

But still, this compiler behavior belongs to the "correct but could be more helpful" bucket of C++ idiosyncrasies. 

## I solved creating a Vector struct from size_t (size() call expressions) arguments with a generic constructor

To be able to create a Vector with plain int coordinates from a call like:

        Vector start = {0,std::distance(model[0].begin(),sit)};
        Vector end = {model.size()-1,std::distance(model.back().begin(),eit)};

,which would make the arguments of type size_t (inferred by the size() call). And this would not be acceptable by the compiler to be passed on to the int r,c of the Vector.

To address this problem, I tried providing a generic constructor that silence any size inconsistencies (a convenience helper constructor).

        struct Vector {
          int r, c;
          Vector(auto r,auto c) {
            // Ensure I can create a vector from container size() call expressions (they are are unsigned size_t...)
            this->r = static_cast<decltype(Vector::r)>(r); // decltype makes refactoring of type r and c easier
            this->c = static_cast<decltype(Vector::c)>(c);
          }
          bool operator==(Vector const& other) const {
            return this->r == other.r && this->c == other.c;
          }
        };


# day 24

## ILearned today that a C++ structured binding does not keep const refs "alive" in the same way as cont ref arguments to functions do.

The code:

    auto const& [head,tail] = SplitOn<'@'>{}.parse(line)[0];

Uses structured binding to get head and tail be cont refs to the temporary returned by left hand size call.

But - "structured bound const refs" does *not* keep the temporary alive. So in the following code head and tail will refer to deleted object!

That was not the semantics I inferred...

## Learned that std::array and std::vector uses mirrored syntax to define a fixed count of values.

To declare an *std::array* with six members you provide the *count* as the *second* argument

    std::array<int,6> values{};

But to instantiate an *std::vector* with six member of an int value you provide the *count* as the *first* argument.

    std::vector<int> vec(6, value);

To add to confusion, trying to use "uniform" initialization (also called brace initialization) can *not* be used to get the "count of value" constructor of std::vector

The code:

    std::vector{6,0}

,will create a vector with two elements 6 and 0 in that order...

## I learned that C++ does not infer a type to hold a literal integer too large for the built in int.

The code:

    if (file == "puzzle.txt") {
      min = 200000000000000;
      max = 400000000000000;
    }

,does not compile. I need, as a programmer, suffix the literals with the correct type suffix (in this case LL for lon long int)

## And I did it again, sneaked in an int that caused my algorithm to silently overflow (fail)

The code:

    using Entry = std::array<int,6>; // px py pz @ vx vy vz

,parsed from input as,

      ...
      result.back()[ix++] = std::stoll(std::string(split[0].first));
      ...

,then propagated to code like

    class Hailstone {
      public:
        Integer sx, sy, sz, vx, vy, vz;
        Integer a, b, c;

        Hailstone(Integer sx, Integer sy, Integer sz, Integer vx, Integer vy, Integer vz)
          : sx(sx), sy(sy), sz(sz), vx(vx), vy(vy), vz(vz), a(vy), b(-vx), c(vy * sx - vx * sy) {}

and finally used as,

          double a1 = hailstones[i].a, b1 = hailstones[i].b, c1 = hailstones[i].c;
          double a2 = hailstones[j].a, b2 = hailstones[j].b, c2 = hailstones[j].c;
          if (a1 * b2 == b1 * a2) {
            std::cout << NL << "Parallel lines i:" << i << " j:" << j;
            continue; // skip parallel lines
          }
          auto x = static_cast<double>(c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1);
          auto y = static_cast<double>(c2 * a1 - c1 * a2) / (a1 * b2 - a2 * b1);

Interestingly enough, even code compiled with "-fsanitize=undefined" failed to detect that the result of std::stoll may not fit into an int!

So much for the help a sanitized-compiled code can help me with.

Hm...I wonder if I should find me a static analyzer to check future code I write?

## Learned today to see the potential overflows when multiplying large integers :)

In the code:

          auto x = (c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1);
          auto y = (c2 * a1 - c1 * a2) / (a1 * b2 - a2 * b1);

If the arguments are std::int64_t we must ensure the result does not overflow.

==> In todays problem we cared only that we got the whole number part correct to compare against the integer test area.
    This is because in an expression (x > a) with x a float and a an int, the int if first converted to float and then
    the comparison is made on the two floats.

After asking an AI I got to know that in C++ "When an operation involves two operands of different types, 
the operand of the "smaller" type is converted to the "larger" type before the operation is performed.

Now this is a bit fuzzy to me. Ok, so a float is deemed "larger" than an int (based on the size of the value it can hold I suppose).
But "larger" can also imply "precision" in my intuition. Because which value is actually larger is not known until runtime.
But already at compile time we know that the int that are say 32 bits, have a value range of -2,147,483,648..2,147,483,647, which 
is 10 digits of precision. While a float only have 7 digits of precision.

So, for floats with no decimals, it would in fact bet better to cast the float to an int to *not* loose precision in the int?
Say the int x=10000001 (8 digits) and the float a = 10000000.0. Now (x > a) becomes (float(10000001) > 10000000.0)
which is (10000000.0 > 10000000.0) which is *false*! The other way around would work - (10000001 > int(10000000.0))
becomes (10000001 > 10000000) with is *true* as expected :)

## I learned that C++ is able to feed a single initializer list to two separate member constructors.

So given the types:

    using Vector = std::array<Integer, 3>;
        class Trajectory {
        public:
          Vector position; // x0,y0,z0
          Vector orientation; // dx,dy,dz
        };

I can write:

      Trajectory a{24,13,10,-3,1,2};

And the compiler will feed the first three values to position and the following three to orientation :)

## For part 2 I kept forgetting that relative start positions parallel with relative velocity is true only for actual collision (NOT trajectory intersection in general)

I spent quite some time to find a "nice" solution for part 2 with the result that I went down quite a number of rabbit holes...

One rabbit hole was that I fell in love with the insight that two objects collide if they keep their relative position while they travel.

And the "nice" way of checking this os to take the cross product of their relative start positions and their relative velocities. 
If these are parallel the cross product is the zero vector.

But, if two objects arrive at the same point at different times..., then their relative position may vary over time. Like, one arrive before the other although it started "behind".

Took me some time to dig that bug out!

## I learned the hard way that this days Puzzle had ben set up so that solving for intersection using lines on the fow ax+by+c=0 will overflow in an int64_t!

It was not until I compiled my C++ code with the "-fsanitize=undefined" flag that I discovered that the expressions for intersection would fail due to products as well as differences would occasionally overflow in int64_t value space!

I spent some time trying to rewrite the expression to "divide before multiplying" in some valid way (not violating integer results) but I did not succeed.
There may still be a gcd-way to do this, but I gave up after having tried checking for a%b==0 to find safe fraction terms and did not find any.

So, after having slept on the matter I realized solving for time of intersections may lead to expressions that will not overflow?

An assumption that turned out correct!

## I learned NOT to trust CoPilot to solve equations for me *sigh*

I set up the equations for the times when two objects i and j on their trajectories would collide (same point, different times).
I had tried to solve them myself but got tired and made silly mistakes confusing x and y and i and j (my handwriting is not the best)...

So I asked CoPilot to solve my equations for collision times ti and tj and it gave me:

        Integer numerator_ti = dx_i * z0_j + dx_i * x0_j - dx_i * z0_i - dz_i * x0_i;
        Integer numerator_tj = dz_j * x0_i + dz_j * z0_j - dz_j * x0_j - dx_j * z0_i;
        Integer denominator = dz_i * dx_j - dx_i * dz_j;

Well, looked legit so I tried them. AS it turned out it did not even work on the known example rock!

So I provided my code to CoPilot and asked it to spot the problem. It now found a bug in its own code (!) and suggested the correction:

        Integer numerator_ti = dx_j * (y0_i - y0_j) - dy_j * (x0_i - x0_j);
        Integer numerator_tj = dx_i * (y0_j - y0_i) - dy_i * (x0_j - x0_i);
        Integer denominator = dy_i * dx_j - dx_i * dy_j;

Again, dreading to solve the equations myself I tried this fix. Still to no avail!

So, I finally sat down and forced myself to solve my equations. And I got:

        Integer numerator_ti = dx_j * (z0_i - z0_j) - dz_j * (x0_i - x0_j);
        Integer numerator_tj = dx_i * (z0_j - z0_i) - dz_i * (x0_j - x0_i);
        Integer denominator_ti = dx_i * dz_j - dz_i * dx_j;
        Integer denominator_tj = -denominator_ti;

You see the bug? Yeas, CoPilot had missed the sign on the denominator!

So close, bit no cigar for CoPilot / ChatGPT!

## I got bitten by C++ silently allowing an initializer list that is too short

I had written the code:

          return { IntersectionType::Intersection,xi,yi,ti,tj };

Which compiled just fine. But the thing I returned actually had 6 members!

I had refactored the code to return x,y,z coordinates so I could use the same type for both xy-plane and xz-plane function.

So the code required was:

          return { IntersectionType::Intersection,xi,yi,0,ti,tj };

Again, C++ has, for my slightly dyslectic programming style, a strong tendency to punish me for incorrect syntax but allow me to write incorrect type applications.

## I learned that expressions using time of intersection allows start positions that are "max velocity" times larger than intersection of line on the form ax+by+c=0.

So, Why does expressions for intersection using the line equation ax+by+c=0 overflow for smaller starting positions than intersection expressions finding intersection time?

*Given:*
auto const& [start_i,orientation_i] = hi_prim;
auto const& [start_j,orientation_j] = hj_prim;
auto const [x0_i,y0_i,z0_i] = start_i;
auto const [x0_j,y0_j,z0_j] = start_j;
auto const [dx_i,dy_i,dz_i] = orientation_i;
auto const [dx_j,dy_j,dz_j] = orientation_j;

*Study the expression for intersection from a line on the for ax+by+c=0*
Integer a = -dy;
Integer b = dx;
Integer c = dy * x0 - dx * y0;
for lines i,j
Integer det = a_j * b_i - a_i * b_j; 
Integer nom_x = b_j * c_i - b_i * c_j;

==> nom_x contains terms like b_j * c_i = dx_j * (dy_i*x0_i - dx_i * y0_i)

We see terms like dx_j*dy_i and dx_i*dy_j multiplied by x or y start position.
Which is proportional to *"velocity squared" times s"start position"*.

For this to NOT overflow in int64_t we need to ensure that the start positions are roughly less than int_64_max / velocity squared. 

*Study the expressions for finding time of intersection*
Integer numerator_ti = dx_j * (y0_i - y0_j) - dy_j * (x0_i - x0_j);
Integer denominator_ti = dx_i * dy_j - dy_i * dx_j;
ti = numerator_ti / denominator_ti;

==> numerator_ti contains terms dx_j * (y0_i - y0_j) and dy_j * (x0_i - x0_j)

This is roughly proportional to *velocity times start position*.

So for intersection using line equation ax+by+c=0 we have terms "velocity squared" times "start position"
And for intersection time we have terms "velocity" times "start position"

For this to NOT overflow in int64_t we need to ensure that the start positions are roughly less than int_64_max / velocity. 

*Thus, intersection time expressions allows for starting positions roughly "max velocity" times larger than intersection expressions from line equation :)*


