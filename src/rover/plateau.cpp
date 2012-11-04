#include "plateau.h"
#include <utility>

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
    
    //TBD - should we fail on leaving an unoccupied tile?
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
      throw std::invalid_argument("upper_right_x");
    }
    if (upper_right_y < 0) { 
      throw std::invalid_argument("upper_right_y");
    }
    return PlateauPtr(new Plateau(upper_right_x, upper_right_y));
  }
}

