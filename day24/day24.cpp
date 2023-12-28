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
    using Vector = std::array<Integer, 3>;

    std::string to_string(Vector const& v) {
      std::string result = "{";
      for (int i = 0; i < 3; ++i) {
        if (i > 0) result += ",";
        result += std::to_string(v[i]);
      }
      result += "}";
      return result;
    }
    
    Vector operator-(const Vector& lhs, const Vector& rhs) {
      return {lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]};
    }

    Vector operator+(const Vector& lhs, const Vector& rhs) {
      return {lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]};
    }

    // scalar multiplication
    Vector operator*(const Vector& lhs, Integer n) {
      return {lhs[0]*n, lhs[1]*n, lhs[2]*n};
    }

    // // scalar division
    // std::array<double,3> operator/(const Vector& lhs, double n) {
    //   return {lhs[0]/n, lhs[1]/n, lhs[2]/n};
    // }

    Integer dot(const Vector& a, const Vector& b) {
        return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
    }

    Vector cross(const Vector& a, const Vector& b) {
        return Vector{
            a[1]*b[2] - a[2]*b[1],
            a[2]*b[0] - a[0]*b[2],
            a[0]*b[1] - a[1]*b[0]
        };
    }    

    Integer determinant(std::array<Vector, 3> matrix) {
        Vector crossProduct = cross(matrix[1], matrix[2]);
        return dot(matrix[0], crossProduct);
    }

    double norm(const Vector& v) {
        return std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    }

    Integer manhattan_norm(const Vector& v) {
        return std::abs(v[0]) + std::abs(v[1]) + std::abs(v[2]);
    }

    bool is_parallel(const Vector& a, const Vector& b) {
        return cross(a, b) == Vector{0, 0, 0};
    }

    class Trajectory {
    public:
      Vector start; // x0,y0,z0
      Vector orientation; // dx,dy,dz
      bool operator<(const Trajectory& other) const {
        return std::tie(start, orientation) < std::tie(other.start, other.orientation);
      }
    };

    // Example solution rock x0:24 y0:13 z0:10 vx:-3 vy:1 vz:2
    Trajectory example_rock{24,13,10,-3,1,2};

    // Hailstone: 20, 19, 15 @ 1, -5, -3
    // Collision time: 1
    // Collision position: 21, 14, 12      

    // Hailstone: 18, 19, 22 @ -1, -1, -2
    // Collision time: 3
    // Collision position: 15, 16, 16

    // Hailstone: 20, 25, 34 @ -2, -2, -4
    // Collision time: 4
    // Collision position: 12, 17, 18

    // Hailstone: 19, 13, 30 @ -2, 1, -2
    // Collision time: 5
    // Collision position: 9, 18, 20

    // Hailstone: 12, 31, 28 @ -1, -2, -1
    // Collision time: 6
    // Collision position: 6, 19, 22

    std::map<Trajectory,Integer> example_collisions{
       {Trajectory{21,14,12,1,-5,-3},1}
      ,{Trajectory{15,16,16,-1,-1,-2},3}
      ,{Trajectory{12,17,18,-2,-2,-4},4}
      ,{Trajectory{9,18,20,-2,1,-2},5}
      ,{Trajectory{6,19,22,-1,-2,-1},6}
    };

    std::string to_string(Trajectory const& t) {
      std::string result = to_string(t.start);
      result += " + t*";
      result += to_string(t.orientation);
      return result;
    }

    // Trajectory to_projected(Trajectory const& h,Vector const& n) {
    //   // project h onto the plane defined by the plane up-vector n (assume not normalized n)
    //   // In effect remove the component of the starting position and velocity that is parallel to the normal vector
    //   auto norm_n = norm(n); // n is integer vector
    //   auto n_norm = n / static_cast<double>(norm_n);

    //   Vector h_position = h.start;
    //   double pos_scale = h_position[0]*n_norm[0] + h_position[1]*n_norm[1] + h_position[2]*n_norm[2];
    //   // Force the projection to integer coordinates
    //   Vector proj_position = h_position - Vector{std::round(n_norm[0]*pos_scale),std::round(n_norm[1]*pos_scale),std::round(n_norm[2]*pos_scale)};
    //   // Now `proj_position` is the projection of the h starting position onto the plane with normal n
    //   Vector h_orientation = h.orientation;
    //   double orientation_scale = h_orientation[0]*n_norm[0] + h_orientation[1]*n_norm[1] + h_orientation[2]*n_norm[2];
    //   // Force the projection to integer coordinates
    //   Vector proj_orientation = h_orientation - Vector{static_cast<Integer>(n_norm[0]*orientation_scale),static_cast<Integer>(n_norm[1]*orientation_scale),static_cast<Integer>(n_norm[2]*orientation_scale)};
    //   return {proj_position, proj_orientation};
    // }

    using Trajectories = std::vector<Trajectory>;
    struct PlaneEquation {
      Integer A, B, C, D; // Ax + By + Cz + D = 0
    };

    PlaneEquation to_plane_equation(Trajectory trajectory1, Trajectory trajectory2) {
      // Calculate the normal vector of the plane (cross product of the orientation vectors)
      auto n = cross(trajectory1.orientation, trajectory2.orientation);

      // Calculate D (dot product of the normal vector and any point on the plane)
      auto D = -dot(n, trajectory1.start);

      // Return the coefficients of the plane equation
      return {n[0], n[1], n[2], D};
    }

    std::string to_string(PlaneEquation const& eq) {
      std::string result = std::to_string(eq.A);
      result += "x + ";
      result += std::to_string(eq.B);
      result += "y + ";
      result += std::to_string(eq.C);
      result += "z + ";
      result += std::to_string(eq.D);
      result += " = 0";
      return result;
    }
    
    Trajectories get_three_random(Trajectories const& trajectories) {
      std::vector<Trajectory> randomTrajectories;  
      // Create a copy of the trajectories vector
      std::vector<Trajectory> shuffledTrajectories = trajectories;  
      // Shuffle the trajectories using std::shuffle
      std::random_device rd;
      std::mt19937 gen(rd());
      std::shuffle(shuffledTrajectories.begin(), shuffledTrajectories.end(), gen);  
      // Get the first three shuffled trajectories
      randomTrajectories.assign(shuffledTrajectories.begin(), shuffledTrajectories.begin() + 3);  
      return randomTrajectories;
    }

    void print_collisions(Trajectory const& rock, Trajectories const& hailstones) {
      std::cout << NL << "print_collisions for rock:" << to_string(rock) << " and " << hailstones.size() << " hailstones.";
      std::map<Integer,Trajectory> collisions{};
      for (auto const& hailstone : hailstones) {
        // Calculate the time of collision
        // rock.start + t * rock.orientation = hailstone.start + t * hailstone.orientation
        // t*(rock.orientation - hailstone.orientation) = hailstone.start - rock.start
        // t = (hailstone.start - rock.start) / (rock.orientation - hailstone.orientation)
        Integer tx,ty,tz;
        auto denom_x = (rock.orientation[0] - hailstone.orientation[0]);
        auto denom_y = (rock.orientation[1] - hailstone.orientation[1]);
        auto denom_z = (rock.orientation[2] - hailstone.orientation[2]);
        if (denom_x != 0) {
          tx = (hailstone.start[0] - rock.start[0]) / denom_x;
        }
        if (denom_y != 0) {
          ty = (hailstone.start[1] - rock.start[1]) / denom_y;
        }
        if (denom_z != 0) {
          tz = (hailstone.start[2] - rock.start[2]) / denom_z;
        }
        if (tx == ty and ty==tz) {
          collisions[tx] = hailstone;
          std::cout << NT << "at time:" << tx << " rock collides with hailstone at position:" << hailstone.start[0] + tx * hailstone.orientation[0] << " " << hailstone.start[1] + tx * hailstone.orientation[1] << " " << hailstone.start[2] + tx * hailstone.orientation[2];
        }
        else {
          std::cout << NT << "ERROR: time check failed for hailstone:" << hailstone.start[0] << " " << hailstone.start[1] << " " << hailstone.start[2] << " velocity:" << hailstone.orientation[0] << " " << hailstone.orientation[1] << " " << hailstone.orientation[2];
          std::cout << NT << "tx:" << tx << " ty:" << ty << " tz:" << tz << " NOT same time?";
          if (tx > 0) collisions[tx] = hailstone;
          else if (ty > 0) collisions[ty] = hailstone;
          else if (tz > 0) collisions[tz] = hailstone;
        }         
      }
    }

    // The function f to get to 0 for rock to collide with hailstone
    Vector f(Trajectory const& a,Trajectory const& b) {
      // If b position relative to a is the same direction as how b is moving towards a. Then b will intersect the trajectory of a somewhere.
      return cross(a.start - b.start,a.orientation - b.orientation);
      // The x-component is (ay0 - by0)*(avz0 - bvz0) - (az0 - bz0)*(avy0 - bvy0) // yz
      // The y-component is (az0 - bz0)*(avx0 - bvx0) - (ax0 - bx0)*(avz0 - bvz0) // zx
      // The z-component is (ax0 - bx0)*(avy0 - bvy0) - (ay0 - by0)*(avx0 - vvx0) // xy
    }

    Trajectory to_hit_all_trajectory(Model const& model, auto args) {
      // Idea: From when I owned a sailing boat I learned the trick to detect if I was on a collision course with another boat.
      //       If the other boats trajectory would cross mine and, its relative orientation relative me was also not changing, then we would collide.
      //       Like, If the other boat was located steady at say 15 degrees to the right of my traveling path!

      // We can express this using vector algebra. If the other boats relative traveling direction is directly at me, then we will collide.
      // For boat a and b traveling by vectors a0 + da and b+ + db. We want b-a and db-da to be parallel.
      // This would mean that the direction a to b, being the b-a vector and the relative orientation of b relative a, being db-da, both points at a.
      // Or yet another way, at some time t t*(db-da) will become b-a and ba would have traveled towards a to the point that they are at the same location.

      if (false) {
        // test / explore that collision detection works

        // 19, 13, 30 @ -2,  1, -2
        // 18, 19, 22 @ -1, -1, -2
        // 20, 25, 34 @ -2, -2, -4
        // 12, 31, 28 @ -1, -2, -1
        // 20, 19, 15 @  1, -5, -3

        // A rock x0:24 y0:13 z0:10 vx:-3 vy:1 vz:2

        // Collisions in time order
        // 	1ns later at time: 1ns rock collides with hailstone at position:21 14 12
        // 	2ns later at time: 3ns rock collides with hailstone at position:15 16 16
        // 	1ns later at time: 4ns rock collides with hailstone at position:12 17 18
        // 	1ns later at time: 5ns rock collides with hailstone at position:9 18 20
        // 	1ns later at time: 6ns rock collides with hailstone at position:6 19 22

        Trajectory rock{24,13,10,-3,1,2};
        Trajectory hailstone{19,13,30,-2,1,-2};
        std::cout << NL << "rock : " << to_string(rock) << " hailstone : " << to_string(hailstone);

        auto relative_position = hailstone.start - rock.start;
        auto relative_orientation = hailstone.orientation - rock.orientation;      
        std::cout << NL << "relative position : " << to_string(relative_position) << " relative orientation : " << to_string(relative_orientation);
        if (is_parallel(relative_position, relative_orientation)) {
          std::cout << NL << "rock and hailstone will collide";
          std::cout << NT << "f is " << to_string(f(hailstone,rock));
        }
        else {
          std::cout << NL << "rock and hailstone will not collide";
        }
      }

      // Ok, so how do we find a rock trajectory such that its relative position with each hailstone is parallel with its relative orientation to this hailstone?      
      // Given one other hailstone a the rock will collide with it if it has any trajectory on any cone around trajectory a.

      // Imagine we pick three hailstones a,b,c that pairwise span three non coplanar planes.

      // We can find three "good" hailstones by random I suppose.
      Trajectories trajectories{};
      for (auto const& entry : model) {
        trajectories.push_back({entry[0], entry[1], entry[2], entry[3], entry[4], entry[5]});
      }
      Trajectories hailstone_candidates = get_three_random(trajectories);
      bool is_three_non_coplanar_planes = false;
      while (!is_three_non_coplanar_planes) {
        // Form the three planes spanned by pairs of the three trajectories
        // Obtain the planes normal vectors
        Vector n0 = cross(hailstone_candidates[0].orientation, hailstone_candidates[1].orientation);
        Vector n1 = cross(hailstone_candidates[1].orientation, hailstone_candidates[2].orientation);
        Vector n2 = cross(hailstone_candidates[2].orientation, hailstone_candidates[0].orientation);
        if (is_parallel(n0, n1) || is_parallel(n1, n2) || is_parallel(n2, n0)) {
          std::cout << NL << "Found three hailstones that DOES NOT span three non coplanar planes";
          for (auto const& trajectory : hailstone_candidates) {
            std::cout << NT << to_string(trajectory);
          }
          // At least two plane normals are parallel, so we need to pick three new hailstones
          // Note: We want the three normals to be independent so we get the most "juice" (information) about a good rock path
          hailstone_candidates = get_three_random(trajectories);
        }
        else {
          is_three_non_coplanar_planes = true;
        }
      }
      std::cout << NL << "Found three hailstones that span three non coplanar planes";
      for (auto const& trajectory : hailstone_candidates) {
        std::cout << NT << to_string(trajectory);
      }
      // We should have three hailstone trajectories sufficiently different to solve for a unique rock path.
      // When the relative position (trajectory.start - rock.start) 
      // is parallel to the relative orientation (hailstone.start - rock.start)
      // then the rock will see the hailstone fixated "in the sky" while it moves. And if the relative velocity is positive towards the rock,
      // then they will sooner or later collide (the velocity towards the rock will eat up the distance between them).

      // Relative position between rock r and a hailstone h: h.start - r.start = (hx,hy,hz) - (rx,ry,rz)
      // relative orientation (velocity) between rock and hailstone: h.orientation - r.orientation = (hvx,hvy,hvz) - (rvx,rvy,rvz)
      // We want the rock oriented so that these two are parallel: The cross product ((hx,hy,hz) - (rx,ry,rz)) x ((hvx,hvy,hvz) - (rvx,rvy,rvz)) = (0,0,0)

      // Split into x,y,z components:
      // The x-component is (hy - ry)*(hvz - rvz) - (hz - rz)*(hvy - rvy) = 0
      // The y-component is (hz - rz)*(hvx - rvx) - (hx - rx)*(hvz - rvz) = 0
      // The z-component is (hx - rx)*(hvy - rvy) - (hy - ry)*(hvx - rvx) = 0

      // Now, the rock can still start anywhere and hit this hailstone anywhere.
      // Hm... Can we use the projection to the xy-plane to get any useful information for this part 2?

      // Like, what if we record all x,y positions where each hailstone "shadow" on the xy-plane intersect?
      // hailstone_candidates.push_back(example_rock); // Add the known example rock trajectory to the three random trajectories
      using ShadowIntersection = std::array<std::optional<double>,3>;
      using ShadowIntersections = std::vector<ShadowIntersection>;
      ShadowIntersections shadow_intersections{};
      ShadowIntersections int_shadow_intersections{};
      hailstone_candidates = trajectories; // Use all hailstones (test)
      // hailstone_candidates.push_back(example_rock); // Add the known example rock trajectory to the candidates
      std::array<std::tuple<Integer,Integer,Integer,Integer>,9> t_equations{}; // a*t1 + b*t2 + c*t3 + d = 0
      int count_ints{};
      for (int i=0;i<hailstone_candidates.size();++i) {
        for (int j=i+1;j<hailstone_candidates.size();++j) {
          auto xi = hailstone_candidates[i];
          auto xj = hailstone_candidates[j];
          std::cout << NL << i << "," << j << " xi:" << to_string(xi) << " xj:" << to_string(xj);
          {
            // Now we can reuse the solution from part 1 to find where the "shadows" on the xy-plane intersect.
            Hailstone hi{xi.start[0],xi.start[1],xi.start[2],xi.orientation[0],xi.orientation[1],xi.orientation[2]};
            Hailstone hj{xj.start[0],xj.start[1],xj.start[2],xj.orientation[0],xj.orientation[1],xj.orientation[2]};
            double ai = hi.a, bi = hi.b, ci = hi.c;
            double aj = hj.a, bj = hj.b, cj = hj.c;
            if (ai * bj == bi * aj) {
              // From part 1: But we have picked i and j carefully and they should NOT be parallel!
              std::cout << NT << "Curious? Parallel lines i:" << i << " j:" << j << " ?";
              continue; // skip parallel lines
            }
            // Note: To ensure the multiplication does not overflow we ensure the arguments are floats and not int64_t
            // Fingers crossed we do not need more than 15 significant digits to find the solution...
            auto x = (ci * bj - cj * bi) / (ai * bj - aj * bi);
            auto y = (cj * ai - ci * aj) / (ai * bj - aj * bi);
            std::cout << NT << "XY-Shadow intersection candidate xyz: " << x << T << y << T << " _ ";
            shadow_intersections.push_back({x,y,std::nullopt});
            if (x == static_cast<Integer>(x) or y == static_cast<Integer>(y)) {
              int_shadow_intersections.push_back({x,y,std::nullopt});               
            }
            if (x == static_cast<Integer>(x)) ++count_ints;
            if (y == static_cast<Integer>(y)) ++count_ints;
          }
          {
            // Lets do the same for the shadow onto the yz-plane
            // If we permute xyz -> yzx we can reuse the Hailstone class ;)
            Hailstone hi{xi.start[1],xi.start[2],xi.start[0],xi.orientation[1],xi.orientation[2],xi.orientation[0]};
            Hailstone hj{xj.start[1],xj.start[2],xj.start[0],xj.orientation[1],xj.orientation[2],xj.orientation[0]};
            double ai = hi.a, bi = hi.b, ci = hi.c;
            double aj = hj.a, bj = hj.b, cj = hj.c;
            if (ai * bj == bi * aj) {
              // From part 1: But we have picked i and j carefully and they should NOT be parallel!
              std::cout << NT << "Curious? Parallel lines i:" << i << " j:" << j << " ?";
              continue; // skip parallel lines
            }
            // Note: To ensure the multiplication does not overflow we ensure the arguments are floats and not int64_t
            // Fingers crossed we do not need more than 15 significant digits to find the solution...
            auto y = (ci * bj - cj * bi) / (ai * bj - aj * bi);
            auto z = (cj * ai - ci * aj) / (ai * bj - aj * bi);
            std::cout << NT << "YZ-Shadow intersection candidate xyz: " << " _ " << T << y << T << z;
            shadow_intersections.push_back({std::nullopt,y,z});
            if (y == static_cast<Integer>(y)) ++count_ints;
            if (z == static_cast<Integer>(z)) ++count_ints;
            if (y == static_cast<Integer>(y) or z == static_cast<Integer>(z)) {
              int_shadow_intersections.push_back({std::nullopt,y,z});               
            }
          }
          {
            // Lets do the same for the shadow onto the zx-plane
            // If we permute yzx -> zxy we can reuse the Hailstone class ;)
            Hailstone hi{xi.start[2],xi.start[0],xi.start[1],xi.orientation[2],xi.orientation[0],xi.orientation[1]};
            Hailstone hj{xj.start[2],xj.start[0],xj.start[1],xj.orientation[2],xj.orientation[0],xj.orientation[1]};
            double ai = hi.a, bi = hi.b, ci = hi.c;
            double aj = hj.a, bj = hj.b, cj = hj.c;
            if (ai * bj == bi * aj) {
              // From part 1: But we have picked i and j carefully and they should NOT be parallel!
              std::cout << NT << "Curious? Parallel lines i:" << i << " j:" << j << " ?";
              continue; // skip parallel lines
            }
            // Note: To ensure the multiplication does not overflow we ensure the arguments are floats and not int64_t
            // Fingers crossed we do not need more than 15 significant digits to find the solution...
            auto z = (ci * bj - cj * bi) / (ai * bj - aj * bi);
            auto x = (cj * ai - ci * aj) / (ai * bj - aj * bi);
            std::cout << NT << "ZX-Shadow intersection candidate xyz: " << x << T << " _ " << T << z;
            shadow_intersections.push_back({x,std::nullopt,z});
            if (x == static_cast<Integer>(x)) ++count_ints;
            if (z == static_cast<Integer>(z)) ++count_ints;
            if (x == static_cast<Integer>(x) or z == static_cast<Integer>(z)) {
              int_shadow_intersections.push_back({x,std::nullopt,z});               
            }
          }
        }        
      }
      /*
      Found three hailstones that span three non coplanar planes
        {19,13,30} + t*{-2,1,-2}
        {20,25,34} + t*{-2,-2,-4}
        {12,31,28} + t*{-1,-2,-1}
      0,1 xi:{19,13,30} + t*{-2,1,-2} xj:{20,25,34} + t*{-2,-2,-4}
        XY-Shadow intersection candidate xyz: 11.6667	16.6667	 _ 
        YZ-Shadow intersection candidate xyz:  _ 	18	20
        ZX-Shadow intersection candidate xyz: 17	 _ 	28
      0,2 xi:{19,13,30} + t*{-2,1,-2} xj:{12,31,28} + t*{-1,-2,-1}
        XY-Shadow intersection candidate xyz: 6.2	19.4	 _ 
        YZ-Shadow intersection candidate xyz:  _ 	17.4	21.2
        Curious? Parallel lines i:0 j:2 ?
      0,3 xi:{19,13,30} + t*{-2,1,-2} xj:{24,13,10} + t*{-3,1,2}
        XY-Shadow intersection candidate xyz: 9	18	 _ 
        YZ-Shadow intersection candidate xyz:  _ 	18	20
        ZX-Shadow intersection candidate xyz: 9	 _ 	20
      1,2 xi:{20,25,34} + t*{-2,-2,-4} xj:{12,31,28} + t*{-1,-2,-1}
        XY-Shadow intersection candidate xyz: -2	3	 _ 
        YZ-Shadow intersection candidate xyz:  _ 	19	22
        ZX-Shadow intersection candidate xyz: 22	 _ 	38
      1,3 xi:{20,25,34} + t*{-2,-2,-4} xj:{24,13,10} + t*{-3,1,2}
        XY-Shadow intersection candidate xyz: 12	17	 _ 
        Curious? Parallel lines i:1 j:3 ?
      2,3 xi:{12,31,28} + t*{-1,-2,-1} xj:{24,13,10} + t*{-3,1,2}
        XY-Shadow intersection candidate xyz: 6	19	 _ 
        YZ-Shadow intersection candidate xyz:  _ 	19	22
        ZX-Shadow intersection candidate xyz: 6	 _ 	22
      */          

      // What can we learn from this information?
      // We know no hailstone intersects another hailstone trajectory.
      // But from the x and y where the XY-shadows intersects we know there is some z distance that would make them intersect in 3D.
      // And for the y and z where the YZ-shadows intersect we know there is some x distance that would make the intersect in 3D.
      // And the same goes for the ZX-shadow where some y distance would make them intersect in 3D.
      // With "hailstone" 3 the known rock trajectory it seems we get a shadow at integer positions?
      // Than makes sense as we know the solution rock will hit each hailstone at integer coordinates at integer times.
      // We also see that the shadows may be parallel although the actual hailstones we selected should not be parallel.
      // Hm... I wonder if the shadow intersections can be our area of interest?
      std::sort(shadow_intersections.begin(),shadow_intersections.end(),[](auto const& a,auto const& b){
        return std::tie(a[0],a[1],a[2]) < std::tie(b[0],b[1],b[2]);}
      );
      std::sort(int_shadow_intersections.begin(),int_shadow_intersections.end(),[](auto const& a,auto const& b){
        return std::tie(a[0],a[1],a[2]) < std::tie(b[0],b[1],b[2]);}
      );
      for (auto const& [ox,oy,oz] : shadow_intersections) {
        std::cout << NT << "shadow_intersection xyz:" << T;
        if (ox.has_value()) std::cout << ox.value();else std::cout << " _ ";
        std::cout << T << " ";
        if (oy.has_value()) std::cout << oy.value();else std::cout << " _ ";
        std::cout << T << " ";
        if (oz.has_value()) std::cout << oz.value();else std::cout << " _ ";
      }
      for (auto const& [ox,oy,oz] : int_shadow_intersections) {
        std::cout << NT << "INTEGER shadow_intersection xyz:" << T;
        if (ox.has_value()) std::cout << ox.value();else std::cout << " _ ";
        std::cout << T << " ";
        if (oy.has_value()) std::cout << oy.value();else std::cout << " _ ";
        std::cout << T << " ";
        if (oz.has_value()) std::cout << oz.value();else std::cout << " _ ";
      }
      std::cout << NL << "shadow_intersections count:" << shadow_intersections.size();
      std::cout << NT << " from hailstone count:" << trajectories.size();
      std::cout << NT << " count_ints:" << count_ints;
      std::cout << NT << " int_shadow_intersections count:" << int_shadow_intersections.size();

      /*

        shadow_intersection xyz:	 _ 	 14	 12
        shadow_intersection xyz:	 _ 	 14	 12
                      {Trajectory{21,  14, 12,1,-5,-3},1}

        shadow_intersection xyz:	 _ 	 17.4	 21.2

        shadow_intersection xyz:	 _ 	 18	 20
        shadow_intersection xyz:	 _ 	 18	 20

        shadow_intersection xyz:	 _ 	 19	 22
        shadow_intersection xyz:	 _ 	 19	 22

        shadow_intersection xyz:	 _ 	 20.1538	 15.6923
        shadow_intersection xyz:	 _ 	 89	 57

        shadow_intersection xyz:	-6	 -5	  _ 
        shadow_intersection xyz:	-2	 3	  _ 
        shadow_intersection xyz:	6.2	 19.4	  _ 
                     ,{Trajectory{6,   19,   22,    -1,-2,-1},6}

                     ,{Trajectory{9,   18,   20,-2,1,-2},5}

        shadow_intersection xyz:	11.6667	 16.6667	  _ 
                     ,{Trajectory{12,      17,       18,-2,-2,-4},4}

        shadow_intersection xyz:	14.3333	 15.3333	  _ 
        shadow_intersection xyz:	14.75	  _ 	 30.75
                     ,{Trajectory{15,    16,   16,-1,-1,-2},3}

        shadow_intersection xyz:	16	  _ 	 27
        shadow_intersection xyz:	16	 39	  _ 
        shadow_intersection xyz:	16.2	  _ 	 26.4

        shadow_intersection xyz:	17	  _ 	 28
        shadow_intersection xyz:	17.8	  _ 	 21.6

        shadow_intersection xyz:	19	 24	  _ 
        shadow_intersection xyz:	19.6667	 20.6667	  _ 

        shadow_intersection xyz:	21.4444	 11.7778	  _ 

        shadow_intersection xyz:	22	  _ 	 38
        shadow_intersection xyz:	25	  _ 	 36
        shadow_intersection xyz:	30	  _ 	 46

        INTEGER shadow_intersection xyz:	 _ 	 14	 12
        INTEGER shadow_intersection xyz:	 _ 	 14	 12
                              {Trajectory{21,  14, 12,1,-5,-3},1}

        INTEGER shadow_intersection xyz:	 _ 	 18	 20
        INTEGER shadow_intersection xyz:	 _ 	 18	 20
                              ,{Trajectory{9,  18, 20,-2,1,-2},5}


        INTEGER shadow_intersection xyz:	 _ 	 19	 22
        INTEGER shadow_intersection xyz:	 _ 	 19	 22

        INTEGER shadow_intersection xyz:	 _ 	 89	 57

        INTEGER shadow_intersection xyz:	-6	 -5	  _ 
        INTEGER shadow_intersection xyz:	-2	 3	  _ 

                             ,{Trajectory{15, 16,  16,   -1,-1,-2},3}

        INTEGER shadow_intersection xyz:	16	  _ 	 27
        INTEGER shadow_intersection xyz:	16	 39	  _ 

        INTEGER shadow_intersection xyz:	17	  _ 	 28
        INTEGER shadow_intersection xyz:	19	 24	  _ 
        INTEGER shadow_intersection xyz:	22	  _ 	 38
        INTEGER shadow_intersection xyz:	25	  _ 	 36
        INTEGER shadow_intersection xyz:	30	  _ 	 46
      shadow_intersections count:26
        from hailstone count:5
        count_ints:32
        int_shadow_intersections count:16

       {Trajectory{21,14,12,1,-5,-3},1}
      ,{Trajectory{15,16,16,-1,-1,-2},3}
      ,{Trajectory{12,17,18,-2,-2,-4},4}
      ,{Trajectory{9,18,20,-2,1,-2},5}
      ,{Trajectory{6,19,22,-1,-2,-1},6}


      */
     // Well, sorting on x is of course to no vane as it does not show the order in time of possible collisions :)
     // Still, the order in time of collisions travels linear through space, p + tdp.

      return {0,0,0,0,0,0}; // Todo, implement ;)
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

    // Glue my model with tbeu Lines
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
            return false; // one pair of lines do not intersect when reoriented by vDiff.
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
              std::map<Integer,Line> collisions{}; 
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
                  collisions[tx] = line;
                  std::cout << NT << "at time:" << tx << " rock collides with hailstone at position:" << p[0] + tx * v[0] << " " << p[1] + tx * v[1] << " " << p[2] + tx * v[2];
                }
                else {
                  // As it turns out one hailstone in my input had the same vy as the rock (so velocity difference is 0 in y-direction and ty is undetermined)
                  // ERROR: time check failed for hailstone:281044650604797 228139153674672 288712115516572 velocity:10 75 -7
                  // 	tx:518687715874 ty:0 tz:518687715874 NOT same time?                  
                  if (tx > 0) collisions[tx] = line;
                  else if (ty > 0) collisions[ty] = line;
                  else if (tz > 0) collisions[tz] = line;
                  else {
                    std::cout << NT << "ERROR: time check failed for hailstone:" << p[0] << " " << p[1] << " " << p[2] << " velocity:" << v[0] << " " << v[1] << " " << v[2];
                    std::cout << NT << "tx:" << tx << " ty:" << ty << " tz:" << tz << " NOT same time?";
                  }
                }
              }
              // print collisions in time order (map will sort on the key being time from low to high)
              std::cout << NL << "Collisions in time order";
              std::pair<Integer,Integer> times{0,0};
              for (auto const& [time,line] : collisions) {
                times.second = time;
                auto dt = times.second - times.first;
                std::cout << NT << dt << "ns later at time: " << time  << "ns rock collides with hailstone";
                std::cout << NT << T << "start: " << line[0][0] << " " << line[0][1] << " " << line[0][2] << " velocity: " << line[1][0] << " " << line[1][1] << " " << line[1][2];
                std::cout << NT << T << "   at:" << line[0][0] + time * line[1][0] << " " << line[0][1] + time * line[1][1] << " " << line[0][2] + time * line[1][2];
                times.first = times.second;
              }

              // return V{pi[0] + t * vi[0], pi[1] + t * vi[1], pi[2] + t * vi[2]}; // 557743507346379
              return rock[0]; // 557743507346379

              // tbeu found rock: (example.txt)
              // 	x0:24 y0:13 z0:10
              // 	vx:-3 vy:1 vz:2

              // part2
              // tbeu found rock:  (puzzle.txt)
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
      if (true) {
        // Developing my own solution from scratch
        const auto rock = mine::to_hit_all_trajectory(model,args);
        result = rock.start[0] + rock.start[1] + rock.start[2];
      }
      // Known working solution from tbeu
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