#include <cctype>
#include <iostream>
#include <iomanip> // E.g., std::quoted
#include <string>
#include <string_view>
#include <sstream> // E.g., std::istringstream, std::ostringstream
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <deque>
#include <array>
#include <ranges> // E.g., std::subrange, std::view 
#include <utility> // E.g., std::pair,..
#include <algorithm> // E.g., std::find, std::all_of,...
#include <numeric> // E.g., std::accumulate
#include <limits> // E.g., std::numeric_limits
#include <fstream>
// #include <experimental/generator> // supported by visual studio 2022 17.8.2 with project setting/compiler switch /std:c++latest
#include <format>
#include <optional>
#include <regex>
#include <cassert>
#include <random>
#include <vector>

char const* example = R"(19, 13, 30 @ -2,  1, -2
18, 19, 22 @ -1, -1, -2
20, 25, 34 @ -2, -2, -4
12, 31, 28 @ -1, -2, -1
20, 19, 15 @  1, -5, -3
)";
std::tuple<int,int> const example_test_area{7,27}; // x,y

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int64_t; // 16 bit int: 3.27 x 10^4, 32 bit int: 2.14 x 10^9, 64 bit int: 9.22 x 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
using Solution = std::map<int, Answers>; // Puzzle part -> Answers

/*
--- Day 24: Never Tell Me The Odds ---

It seems like something is going wrong with the snow-making process. 
Instead of forming snow, the water that's been absorbed into the air seems to be forming hail!

Maybe there's something you can do to break up the hailstones?

Due to strong, probably-magical winds, 
the hailstones are all flying through the air in perfectly linear trajectories. 
You make a note of each hailstone's position and velocity (your puzzle input). 

For example:

19, 13, 30 @ -2,  1, -2
18, 19, 22 @ -1, -1, -2
20, 25, 34 @ -2, -2, -4
12, 31, 28 @ -1, -2, -1
20, 19, 15 @  1, -5, -3

Each line of text corresponds to the position and velocity of a single hailstone. 
The positions indicate where the hailstones are right now (at time 0). 
The velocities are constant and indicate exactly how far each hailstone will move in one nanosecond.

Each line of text uses the format px py pz @ vx vy vz. 
For instance, the hailstone specified by 20, 19, 15 @ 1, -5, -3 
  has initial X position 20, Y position 19, Z position 15, 
  X velocity 1, Y velocity -5, and Z velocity -3. 
  After one nanosecond, the hailstone would be at 21, 14, 12.

Perhaps you won't have to do anything. 
How likely are the hailstones to collide with each other and smash into tiny ice crystals?

To estimate this, 
consider only the X and Y axes; 
  ==> ignore the Z axis. 
  
Looking forward in time, 
how many of the hailstones' paths will intersect within a test area? 
  (The hailstones themselves don't have to collide, 
   just test for intersections between the paths they will trace.)

In this example, 
look for intersections that happen with an X and Y position each at least 7 and at most 27; 
in your actual data, you'll need to check a much larger test area. 
Comparing all pairs of hailstones' future paths produces the following results:

Hailstone A: 19, 13, 30 @ -2, 1, -2
Hailstone B: 18, 19, 22 @ -1, -1, -2
Hailstones' paths will cross inside the test area (at x=14.333, y=15.333).

Hailstone A: 19, 13, 30 @ -2, 1, -2
Hailstone B: 20, 25, 34 @ -2, -2, -4
Hailstones' paths will cross inside the test area (at x=11.667, y=16.667).

Hailstone A: 19, 13, 30 @ -2, 1, -2
Hailstone B: 12, 31, 28 @ -1, -2, -1
Hailstones' paths will cross outside the test area (at x=6.2, y=19.4).

Hailstone A: 19, 13, 30 @ -2, 1, -2
Hailstone B: 20, 19, 15 @ 1, -5, -3
Hailstones' paths crossed in the past for hailstone A.

Hailstone A: 18, 19, 22 @ -1, -1, -2
Hailstone B: 20, 25, 34 @ -2, -2, -4
Hailstones' paths are parallel; they never intersect.

Hailstone A: 18, 19, 22 @ -1, -1, -2
Hailstone B: 12, 31, 28 @ -1, -2, -1
Hailstones' paths will cross outside the test area (at x=-6, y=-5).

Hailstone A: 18, 19, 22 @ -1, -1, -2
Hailstone B: 20, 19, 15 @ 1, -5, -3
Hailstones' paths crossed in the past for both hailstones.

Hailstone A: 20, 25, 34 @ -2, -2, -4
Hailstone B: 12, 31, 28 @ -1, -2, -1
Hailstones' paths will cross outside the test area (at x=-2, y=3).

Hailstone A: 20, 25, 34 @ -2, -2, -4
Hailstone B: 20, 19, 15 @ 1, -5, -3
Hailstones' paths crossed in the past for hailstone B.

Hailstone A: 12, 31, 28 @ -1, -2, -1
Hailstone B: 20, 19, 15 @ 1, -5, -3
Hailstones' paths crossed in the past for both hailstones.

So, in this example, 
2 hailstones' future paths cross inside the boundaries of the test area.

However, you'll need to search a much larger test area 
if you want to see if any hailstones might collide. 

Look for intersections that happen 
  with an X and Y position each at least 200000000000000 and at most 400000000000000. 
  Disregard the Z axis entirely.

Considering only the X and Y axes, 
check all pairs of hailstones' future paths for intersections. 

How many of these intersections occur within the test area?

*/

/*
--- Part Two ---

Upon further analysis, it doesn't seem like any hailstones will naturally collide. 
It's up to you to fix that!

You find a rock on the ground nearby. 
While it seems extremely unlikely, 
if you throw it just right, 
you should be able to hit every hailstone in a single throw!

You can use the probably-magical winds to reach any integer position you like 
and to propel the rock at any integer velocity. 
Now including the Z axis in your calculations, 
if you throw the rock at time 0, 
where do you need to be 
  so that the rock perfectly collides with every hailstone? 
  Due to probably-magical inertia, 
  the rock won't slow down or change direction when it collides with a hailstone.

In the example above, you can achieve this 
by moving to position 24, 13, 10 
and throwing the rock at velocity -3, 1, 2. 

If you do this, you will hit every hailstone as follows:

Hailstone: 19, 13, 30 @ -2, 1, -2
Collision time: 5
Collision position: 9, 18, 20

Hailstone: 18, 19, 22 @ -1, -1, -2
Collision time: 3
Collision position: 15, 16, 16

Hailstone: 20, 25, 34 @ -2, -2, -4
Collision time: 4
Collision position: 12, 17, 18

Hailstone: 12, 31, 28 @ -1, -2, -1
Collision time: 6
Collision position: 6, 19, 22

Hailstone: 20, 19, 15 @ 1, -5, -3
Collision time: 1
Collision position: 21, 14, 12

Above, each hailstone is identified 
by its initial position and its velocity. 
Then, the time and position of that hailstone's collision with your rock are given.

After 1 nanosecond, 
the rock has exactly the same position as one of the hailstones, 
obliterating it into ice dust! 
Another hailstone is smashed to bits two nanoseconds after that. 

After a total of 6 nanoseconds, all of the hailstones have been destroyed.

So, at time 0, the rock needs to be at X position 24, Y position 13, and Z position 10. 

Adding these three coordinates together produces 47. (Don't add any coordinates from the rock's velocity.)

Determine the exact position and velocity the rock needs to have at time 0 
so that it perfectly collides with every hailstone. 

What do you get if you add up the X, Y, and Z coordinates of that initial position?

*/

class Parser {
public:
  using Candidate = std::pair<std::string_view, std::string_view>;
  using Candidates = std::vector<Candidate>;
    virtual Candidates parse(std::string_view input) = 0;
};

template <char C>
class SplitOn : public Parser  {
public:
  using Candidate = std::pair<std::string_view, std::string_view>;
  using Candidates = std::vector<Candidate>;
    Candidates parse(std::string_view input) {
      if (input.empty()) return Candidates{}; // return empty parse
      Candidates result;
      // Find the next char C
      size_t pos = input.find(C);

      // If there's no char C, this is the tail
      if (pos == std::string_view::npos)
      {
        result.push_back({input,{}});
      }
      else {
        auto head = input.substr(0, pos);
        auto tail = input.substr(pos + 1);
        result.push_back({head,tail});
      }
      return result;
    }
};

using Entry = std::array<Integer,6>; // px py pz @ vx vy vz
using Model = std::vector<Entry>;

Model parse(auto& in) {
  Model result{};
  std::string line{};
  while (std::getline(in,line)) {
    result.push_back({});
    auto split_line = SplitOn<'@'>{}.parse(line)[0]; // assume success
    std::cout << NT << "head : " << std::quoted(split_line.first) << " tail : " << std::quoted(split_line.second);
    int ix{};
    for (auto split = SplitOn<','>{}.parse(split_line.first); !split.empty(); split = SplitOn<','>{}.parse(split[0].second)) {
      std::cout << NT << NT << "head : " << std::quoted(split[0].first) << " tail : " << std::quoted(split[0].second);
      result.back()[ix++] = std::stoll(std::string(split[0].first));
    }
    for (auto split = SplitOn<','>{}.parse(split_line.second); !split.empty(); split = SplitOn<','>{}.parse(split[0].second)) {
      std::cout << NT << NT << "head : " << std::quoted(split[0].first) << " tail : " << std::quoted(split[0].second);
      result.back()[ix++] = std::stoll(std::string(split[0].first));
    }
  }
  return result;
}

void print_model(Model const& model) {
  std::cout << NL << "Model";
  for (auto const& entry : model) {
    std::cout << NL << "px py pz vx vy vz : ";
    for (auto const& value : entry) {
      std::cout << " " << value;
    }
  }
}

// Hailstone trajectory 
// 1) in 3D space on the form s + t * v
// 2) on the xy-plane on the form ax + by  = c
//    with: a = dx/dt, b = -dy/dt, the velocities vx and vy.
class Hailstone {
public:
  Integer sx, sy, sz, vx, vy, vz; // Trajectory s + t * v in 3D space
  Integer a, b, c;

  Hailstone(Integer sx, Integer sy, Integer sz, Integer vx, Integer vy, Integer vz)
    : sx(sx), sy(sy), sz(sz), vx(vx), vy(vy), vz(vz), a(vy), b(-vx), c(vy * sx - vx * sy) {}

  friend std::ostream& operator<<(std::ostream& os, const Hailstone& hs) {
    return os << "Hailstone{a=" << hs.a << ", b=" << hs.b << ", c=" << hs.c << "}";
  }
};

namespace part1 {
  // Refactored from hyperneutrino python https://github.com/hyper-neutrino/advent-of-code/blob/main/2023/day24p1.py
  namespace hyperneutrino {

    int count(Model const& model,auto args) {
      std::vector<Hailstone> hailstones;
      for (auto const& entry : model) {
        hailstones.push_back({entry[0], entry[1], entry[2], entry[3], entry[4], entry[5]});
      }

      int total = 0;
      auto const& [_,__,min,max] = args;

      for (Integer i = 0; i < hailstones.size(); ++i) {
        for (Integer j = 0; j < i; ++j) {
          // Solve the system:
          // e1: a1 * x + b1 * y = c1
          // e2: a2 * x + b2 * y = c2
          // e1 * a2 - e2 * a1: (a1 * b2 - a2 * b1) * y = c1 * a2 - c2 * a1; x = (c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1)
          // e2 * b1 - e1 * b2: (a1 * b2 - a2 * b1) * x = c2 * b1 - c1 * b2; y = (c2 * a1 - c1 * a2) / (a1 * b2 - a2 * b1)
          // BUT: a1 * b2 - a2 * b1 == 0 when a1 * b2 == a2 * b1 (the lines are parallel = never intersects)
          double a1 = hailstones[i].a, b1 = hailstones[i].b, c1 = hailstones[i].c;
          double a2 = hailstones[j].a, b2 = hailstones[j].b, c2 = hailstones[j].c;
          if (a1 * b2 == b1 * a2) {
            std::cout << NL << "Parallel lines i:" << i << " j:" << j;
            continue; // skip parallel lines
          }
          // Note: To ensure the multiplication does not overflow we ensure the arguments are floats and not int64_t (See a and b arguments above)
          auto x = (c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1);
          auto y = (c2 * a1 - c1 * a2) / (a1 * b2 - a2 * b1);
          std::cout << NL << "Intersection candidate x : " << x << " y : " << y;
          if (min <= x && x <= max && min <= y && y <= max) {
            // Intersection point is in the test area
            // But, is the intersection point in the future?
            // Note: We can imagine a point starting beyond the test area, and intersect with the test area for negative time.
            // Also, we have solved for any time t such that the lines intersect at time t. But we don't know if t is positive or negative... 
            // The intersection is in the future (positive time) if the point p(t) > p(0) for dp > 0 and or p(t) < p(0) for dp < 0.
            // That is, if the point p(t) is in the same direction as the velocity vector v, we will move to p(t) in the future.
            // Here p(0), start p, is represented by sx,sy And intersection point p(t) is represented by x,y.
            // So, the intersection is in the future if dx = x - sx > 0 and vx > 0 (test area to the right of start) or dx < 0 and vx < 0 (test area to the left of start)
            // And the same goes for y.
            if ((x - hailstones[i].sx) * hailstones[i].vx >= 0 && (y - hailstones[i].sy) * hailstones[i].vy >= 0 &&
              (x - hailstones[j].sx) * hailstones[j].vx >= 0 && (y - hailstones[j].sy) * hailstones[j].vy >= 0) {
              total += 1;
            }
            else {
              std::cout << " is in the past";
            }
          }
          else {
            std::cout <<  " is outside the test area x:" << min << ".." << max << " y:" << min << ".." << max;
          }
        }
      }

      std::cout << NL << "hyperneutrion says : " << total << std::endl;

      return total; // 17867
    }
  }

  Result solve_for(Model& model,auto args) {
    Result result{};
    std::cout << NL << NL << "part1";
    print_model(model);
    result = hyperneutrino::count(model,args);
    return result; // 17867
  }
}

namespace part2 {


  namespace mine {
    class Trajectory {
    public:
      Integer x0, y0, z0, dx, dy, dz;
    };

    Trajectory to_stone_trajectory(Model const& model) {
      return {0,0,0,0,0,0}; // Todo, implement ;)

      // Idea: To find the trajectory of a stone that will hit every hailstone at some time t in the future,
      //       maybe we can begin by finding a plane that all hailstones trajectory will intersect at some time t in the future.
      //       Hm... I can imagine hailstone trajectories such that many such planes exist that all hailstones goes through at some point in time.
      //       But we want the plane where the intersection point on this plane for a line!     
    }
  }

  // Refactored from C++ solution https://github.com/tbeu/AdventOfCode/blob/master/2023/day24/day24.cpp
  // many thanks!
  // Note: The comments are mostly mine made while refactoring!
  namespace tbeu {

    using V = std::array<int64_t, 3>; // 3D vector
    using Line = std::array<V, 2>; // start,velocity vectors pair
    using Lines = std::vector<Line>;

    V operator-(const V& lhs, const V& rhs) {
      return {lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]};
    }

    Line operator-(const Line& lhs, const Line& rhs) {
      Line result;
      result[0] = lhs[0] - rhs[0];
      result[1] = lhs[1] - rhs[1];
      return result;
    }

    // Glu my model with tbeu Lines
    Lines to_lines(Model const& model) {
      Lines lines;
      for (auto const& entry : model) {
        V v1{entry[0], entry[1], entry[2]};
        V v2{entry[3], entry[4], entry[5]};
        Line line{v1,v2};
        lines.push_back(line);
      }
      return lines;
    }

    // Returns the two times t and s.
    // The time t when object on Line a intersects with line b, 
    // and time s when object on line b intersects with line a.
    template <typename INTERSECT_TYPE>
    static std::optional<std::array<INTERSECT_TYPE, 2>> intersect(const Line &a, const Line &b) {
      const auto &p1 = a[0];
      const auto &v1 = a[1];
      const auto &p2 = b[0];
      const auto &v2 = b[1];
      INTERSECT_TYPE s, t;
      const auto den1 = v2[0] * v1[1]; // v2x*v1y
      const auto den2 = v2[1] * v1[0]; // v2y*v1x
      if (den1 == den2) {
        // parallel or identical (v2x*v1y - v2y*v1x == 0)
        return std::nullopt;
      } 
      else {
        const auto nom1 = ((p2[1] - p1[1]) * v1[0] + (p1[0] - p2[0]) * v1[1]);
        if constexpr (std::is_integral_v<INTERSECT_TYPE>) {
          if (0 != nom1 % (den1 - den2)) {
            // not an integer solution
            return std::nullopt;
          }
        }
        s = nom1 / static_cast<INTERSECT_TYPE>(den1 - den2);
        const auto nom2 = (p2[0] - p1[0]) + s * v2[0];
        t = nom2 / static_cast<INTERSECT_TYPE>(v1[0]);
      }
      return std::array<INTERSECT_TYPE, 2>{t, s};
    }

    // Check objects on line a and b intersect each others trajectory at some positive time.
    // Note: They do not have to collide, just intersect each trajectories as an integral time value.
    template <typename INTERSECT_TYPE>
    static bool checkIntersect(const Line &a,const Line &b,int64_t start,int64_t end, bool ignoreZ) {

      const auto ts = intersect<INTERSECT_TYPE>(a, b);
      const auto &p1 = a[0];
      const auto &v1 = a[1];
      const auto &p2 = b[0];
      const auto &v2 = b[1];
      if (!ts.has_value()) {
        const auto isParallel =
            (p2[0] - p1[0]) * v1[1] == (p2[1] - p1[1]) * v1[0]; // xy-projection is parallel
        if (ignoreZ) {
          return isParallel;
        } else {
          return isParallel ||
                 (p2[0] - p1[0]) * v1[2] == (p2[2] - p1[2]) * v1[0]; // Parallel in xyz space
        }
      }
      const auto [t, s] = ts.value();
      bool isInter = s >= 0 && t >= 0; // In the future
      if constexpr (std::is_integral_v<INTERSECT_TYPE>) {
        if (isInter && !ignoreZ) {
          const auto r1 = p1[2] + t * v1[2];
          const auto r2 = p2[2] + s * v2[2];
          isInter = isInter && r1 == r2; // r1 is at the same position at time t as r2 is at time s.
        }
      } else {
        // Floating point comparison
        // Find an overlap in the test area.
        for (uint8_t i = 0; i < 3; ++i) {
          if (!isInter) {
            break;
          }
          if (ignoreZ && 2 == i) {
            break;
          }
          const auto r1 = p1[i] + t * v1[i];
          const auto r2 = p2[i] + s * v2[i];
          isInter = isInter && r1 >= start && r2 >= start;
          isInter = isInter && r1 <= end && r2 <= end;
        }
      }
      return isInter; // True if the lines intersect each others trajectories at some positive time
    }

    // V is a velocity vector
    // returns true if all hailstones with a velocity reduced by vDiff will intersect
    template <typename INTERSECT_TYPE>
    static bool checkIntersect(const Lines &lines, const V &vDiff, bool ignoreZ = true)
    {
      for (size_t i = 0; i < lines.size(); ++i)
      {
        for (size_t j = i + 1; j < lines.size(); ++j)
        {
          const auto &pi = lines[i][0];
          const auto &vi = lines[i][1] - vDiff;
          const auto &pj = lines[j][0];
          const auto &vj = lines[j][1] - vDiff;
          if (!checkIntersect<INTERSECT_TYPE>({pi, vi}, {pj, vj}, INT64_MIN, INT64_MAX, ignoreZ))
          {
            return false;
          }
        }
      }
      return true;
    }

    static V findRock(const Lines &lines) {
      constexpr int64_t vRange = 300; // scan range
      for (int64_t v1 = -vRange; v1 <= vRange; ++v1) {
        // vx velocity
        for (int64_t v2 = -vRange; v2 <= vRange; ++v2) {
          // vy velocity
          if (const auto v = V{v1, v2, 0};!checkIntersect<int64_t>(lines, v, true)) {
            // Don't even overlap in 2D (x,y plane)
            continue;
          }
          for (int64_t v3 = -vRange; v3 <= vRange; ++v3) {
            // vz velocity
            // Check if all lines reduced with velocity v (vx,vy,vz) (rock reference frame) intersects each other
            if (const auto v = V{v1, v2, v3};checkIntersect<int64_t>(lines, v, false)) {
              // All lines intersect each other in the rocks reference frame (when their velocity is reduced by v))
              // get the actual intersection point (from any two lines) as seen in global reference frame
              const auto pi = lines[0][0];
              const auto vi = lines[0][1] - v; // vi is the hailstone i velocity as seen within rocks reference frame
              const auto pj = lines[1][0];
              const auto vj = lines[1][1] - v; // vj is the hailstone j velocity as seen within rocks reference frame
              const auto [t, s] =
                  intersect<int64_t>({pi, vi}, {pj, vj}).value();
              // t and s are times such that pi + t * vi = pj + s * vj
              // The velocity v is the rocks velocity, and the hailstone velocity is vi and vj in rocks reference frame.
              // Hm... Dear tbeu, I do not get why the rock starting position is pi + t+vi?
              // Maybe t and s are the starting times in the past so that the two hailstones collide at rocks time=0?
              // I leave this be for now and just accept the solution as it has proven to be correct!
              Line rock{V{pi[0]+vi[0]*t,pi[1]+vi[1]*t,pi[2]+vi[2]*t},v};
              std::cout << NL << "tbeu found rock: ";
              std::cout << NT << "x0:" << rock[0][0] << " y0:" << rock[0][1] << " z0:" << rock[0][2];
              std::cout << NT << "vx:" << rock[1][0] << " vy:" << rock[1][1] << " vz:" << rock[1][2];

              // Lets check that the solution works.
              for (auto const& line : lines) {
                const auto &p = line[0];
                const auto &v = line[1];
                Integer tx{},ty{},tz{};
                if (rock[1][0] - v[0] != 0) {
                  tx = (p[0] - rock[0][0]) / (rock[1][0] - v[0]);
                }
                if (rock[1][1] - v[1] != 0) {
                  ty = (p[1] - rock[0][1]) / (rock[1][1] - v[1]);
                }
                if (rock[1][2] - v[2] != 0) {
                  tz = (p[2] - rock[0][2]) / (rock[1][2] - v[2]);
                }
                if (tx == ty and ty == tz) {
                  std::cout << NT << "at time:" << tx << " rock collides with hailstone at position:" << p[0] + tx * v[0] << " " << p[1] + tx * v[1] << " " << p[2] + tx * v[2];
                }
                else {
                  std::cout << NT << "ERROR: time check failed for hailstone:" << p[0] << " " << p[1] << " " << p[2] << " velocity:" << v[0] << " " << v[1] << " " << v[2];
                  std::cout << NT << "tx:" << tx << " ty:" << ty << " tz:" << tz << " NOT same time?";
                }
              }

              // return V{pi[0] + t * vi[0], pi[1] + t * vi[1], pi[2] + t * vi[2]}; // 557743507346379
              return rock[0]; // 557743507346379

              // part2
              // tbeu found rock: 
              // 	x0:159153037374407 y0:228139153674672 z0:170451316297300
              // 	vx:245 vy:75 vz:221
              // tbeu says solution is:  159153037374407 , 228139153674672 , 170451316297300

              // ------------ REPORT----------------
              // Part 2 answers
              // 	answer[puzzle.txt] 557743507346379

              // Lets check the solution for the first hailstone.
              // 308205470708820, 82023714100543, 475164418926765 @ 42, 274, -194
              // x: 159153037374407 + t*245 = 308205470708820 + t*42 ?
              // y: 228139153674672 + t*75 = 82023714100543 + t*274 ?
              // z: 170451316297300 + t*221 = 475164418926765 + t*-194 ?
              // 245t - 42t = 308205470708820 - 159153037374407;
              // 75t - 274t = 82023714100543 - 228139153674672;
              // 221t + 194t = 475164418926765 - 170451316297300;
            }
          }
        }
      }
      return V{0, 0, 0};
    }
  } // namespace tbeu

  Result solve_for(Model &model, auto args) {
      Result result{};
      std::cout << NL << NL << "part2";
      const auto rock = tbeu::findRock(tbeu::to_lines(model));
      result = rock[0] + rock[1] + rock[2]; // 557743507346379
      return result; // 557743507346379
  }
}

int main(int argc, char *argv[])
{
  Solution solution{};
  std::cout << NL << "argc : " << argc;
  for (int i = 0; i < argc; ++i) {
    std::cout << NL << "argv[" << i << "] : " << std::quoted(argv[i]);
  }
  // day24 part file min max
  std::tuple<int,std::string,Integer,Integer> args{1,"example.txt",7,17};
  auto& [part,file,min,max] = args;
  if (argc > 1 ) {
    part = std::stoi(argv[1]);
    if (argc > 2) {
      file = argv[2];
    }
    if (file == "puzzle.txt") {
      min = 200000000000000LL;
      max = 400000000000000LL;
    }
    if (argc > 4) {
      min = std::stoi(argv[3]);
      max = std::stoi(argv[4]);
    }
  }
  std::cout << NL << "Part : " << part << " file : " << file << " min : " << min << " max : " << max;;
  std::ifstream in{ file };
  auto model = parse(in);

  switch (part) {
  case 1: {
    auto answer = part1::solve_for(model,args);
    solution[part].push_back({ file,answer });
  } break;
  case 2: {
    auto answer = part2::solve_for(model,args);
    solution[part].push_back({ file,answer });
  } break;
  default:
    std::cout << NL << "No part " << part << " only part 1 and 2";
  }

  std::cout << NL << NL << "------------ REPORT----------------";
  for (auto const& [part, answers] : solution) {
    std::cout << NL << "Part " << part << " answers";
    for (auto const& [heading, answer] : answers) {
      if (answer != 0) std::cout << NT << "answer[" << heading << "] " << answer;
      else std::cout << NT << "answer[" << heading << "] " << " NO OPERATION ";
    }
  }
  std::cout << NL << NL;
  return 0;
}