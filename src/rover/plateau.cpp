#include "plateau.h"
#include <utility>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace rv {

  Plateau::Plateau(int upper_right_x, int upper_right_y):

    // Boost MultiArray documentation isn't clear on how initial values are 
    // constructed.  However, the source in 1.51 initialises the contents 
    // using the element's default constructor. Plus the docs state
    // resize ops use it too, so this 'should' be safe.
  _occupied(boost::extents[upper_right_y+1][upper_right_x+1])
  { }

  bool Plateau::out_of_bounds(int x, int y)const {
    return x < 0 || y < 0 || x >= x_size() || y >= y_size();
  }

  bool Plateau::can_enter(int x, int y)const {
    return !out_of_bounds(x, y) && !occupied(x, y);
  }

  bool Plateau::enter(int x, int y) {
    if (!can_enter(x, y)) return false;
    _occupied[y][x] = true;
    return true;
  }

  bool Plateau::leave(int x, int y) {
    // Leaving a in-bounds tile always succeeds.
    if (out_of_bounds(x, y)) return false;
    _occupied[y][x] = false;
    return true;
  }

  bool Plateau::occupied(int x, int y)const {
    return _occupied[y][x];
  }

  bool Plateau::descend(int x, int y) {
    return enter(x, y);
  }

  bool Plateau::move(int x, int y, int dx, int dy) {

    // A legal move is at most one tile, either horizontally or vertically.
    if (dx * dy != 0 || abs(dx) > 1 || abs(dy) > 1) return false;

    const int new_x = x + dx, new_y = y + dy;
    if (!can_enter(new_x, new_y) || !leave(x, y)) return false;
    return enter(new_x, new_y);
  }

  PlateauPtr make_plateau(int upper_right_x, int upper_right_y) {
    if (upper_right_x < 0) { 
      throw std::invalid_argument("Plateau x-coordinate is negative");
    }
    if (upper_right_y < 0) { 
      throw std::invalid_argument("Plateau y-coordinate is negative");
    }
    return PlateauPtr(new Plateau(upper_right_x, upper_right_y));
  }

  // A plateau coordinate is an X Y pair.
  static const boost::regex coordinate_exp("(\\d+)\\s+(\\d+)");

  PlateauPtr make_plateau(const std::string& upper_right) {

    // Not necessary to trim whitespace, but makes it a little easier to use.
    std::string s = boost::trim_copy(upper_right);
    boost::smatch sm;
    if (boost::regex_match(s, sm, coordinate_exp)) {
      const int x = boost::lexical_cast<int>(sm[1]);
      const int y = boost::lexical_cast<int>(sm[2]);
      return make_plateau(x, y);
    } else {
      throw std::invalid_argument("Invalid plateau dimensions");
    }
  }
}

