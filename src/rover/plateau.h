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

      bool enter(int x, int y);

      bool leave(int x, int y);

      bool occupied(int x, int y)const;

      int y_size()const { return _occupied.shape()[0]; }
      int x_size()const { return _occupied.shape()[1]; }

    private:

      boost::multi_array<bool, 2> _occupied;
  };

  typedef std::tr1::shared_ptr<Plateau> PlateauPtr;
  

  /*
   * @brief Create a new plateau.
   * @param upper_right_x x component of upper-right coordinate
   * @param upper_right_y y component of upper-right coordinate
   */
  PlateauPtr make_plateau(int upper_right_x, int upper_right_y);
}

#endif
