#ifndef RV_ROVER_H
#define RV_ROVER_H

#include <iosfwd>
#include <boost/operators.hpp>

namespace rv {

/**
 * @brief A cute little Rover, imagine Wall-E.
 * Can rotate around move along his current heading.
 */
class Rover : public boost::equality_comparable<Rover> {
  public:
    Rover(int x, int y, char facing):
      _x(x),
      _y(y),
      _facing(facing)
  {}
    int x()const { return _x; }
    int y()const { return _y; }
    char facing()const { return _facing; }

    Rover left()const;
    Rover right()const;
    Rover move()const;

    bool operator==(const Rover& b)const;

  private:
    int _x;
    int _y;
    char _facing;
};


std::ostream& operator<<(std::ostream& os, const Rover& r);

 Rover make_rover(const std::string& position);

 /*
  * @brief Execute a list of actions.
  * @param actions - a string of [LMR] characters.
  */
 Rover command(Rover r, const std::string& actions);

}

#endif

