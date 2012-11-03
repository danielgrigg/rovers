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

  bool Plateau::enter(int x, int y) {
    if (x < 0 || y < 0 || x >= x_size() || y >= y_size()) return false;

    if (_occupied[y][x]) return false;

    _occupied[y][x] = true;
    return true;
  }

  bool Plateau::leave(int x, int y) {
    if (x < 0 || y < 0 || x >= x_size() || y >= y_size()) return false;

    _occupied[y][x] = false;
    return true;
  }

  bool Plateau::occupied(int x, int y)const {
    return _occupied[y][x];
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
