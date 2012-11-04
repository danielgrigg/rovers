#ifndef RV_PLATEAU_H
#define RV_PLATEAU_H

#include <tr1/memory>
#include <boost/multi_array.hpp>
#include <tr1/tuple>

namespace rv {

  typedef boost::multi_array<bool, 2> OccupancyGrid;

  // A plateau tracks tile occupancy. 
  class Plateau {
    public:
      Plateau(int upper_right_x, int upper_right_y);


      bool occupied(int x, int y)const;

      /*
       * @brief Plateau y extent
       */
      int y_size()const { return _occupied.shape()[0]; }

      /*
       * @brief Plateau x extent
       */
      int x_size()const { return _occupied.shape()[1]; }

      /*
       * @brief Initial rover descent to (x,y).
       * A failed descent implies loss of one or more rovers
       * and the end of exploration.
       */
      bool descend(int x, int y);

      /*
       * @brief Attempt to move from (x,y) -> (x+dx, y+dy).
       * Rovers only move horizontally or vertically one tile per move.
       * Unsuccessful moves leave the plateau state unchanged.
       * @param dx 0 or 1 tiles
       * @param dy 0 or 1 tiles
       * @return true if move if successful. 
       */
      bool move(int x, int y, int dx, int dy);

    private:

      // Is (x,y) is outside the plateau.
      bool out_of_bounds(int x, int y)const;

      bool can_enter(int x, int y)const;

      // Try entering a tile.
      bool enter(int x, int y);

      // Try leaving a tile.
      bool leave(int x, int y);

      boost::multi_array<bool, 2> _occupied;

  };

  typedef std::tr1::shared_ptr<Plateau> PlateauPtr;
  

  /*
   * @brief Create a new plateau.
   * @param upper_right_x x component of upper-right coordinate
   * @param upper_right_y y component of upper-right coordinate
   */
  PlateauPtr make_plateau(int upper_right_x, int upper_right_y);

  /*
   * @brief Create a plateau given its upper-right coordinate.
   * @param upper_right X Y string.
   */
  PlateauPtr make_plateau(const std::string& upper_right);
}

#endif
