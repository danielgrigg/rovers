#ifndef RV_ROVER_H
#define RV_ROVER_H

#include <iosfwd>
#include <boost/operators.hpp>
#include <tr1/memory>
#include "io.h"

namespace rv {

  // Forward declaration
class Plateau;
typedef std::tr1::shared_ptr<Plateau> PlateauPtr;

/**
 * @brief A cute little Rover, imagine Wall-E.
 * Can rotate around move along his current heading.
 */
class Rover : public boost::equality_comparable<Rover> {
  public:
    Rover(int x, int y, char facing, PlateauPtr p= PlateauPtr());
    int x()const { return _x; }
    int y()const { return _y; }
    char facing()const { return _facing; }

    Rover left();
    Rover right();
    Rover move();

    bool operator==(const Rover& b)const;

  private:
    int _x;
    int _y;
    char _facing;
    PlateauPtr _plateau;
};

std::ostream& operator<<(std::ostream& os, const Rover& r);

/*
 * @brief Create a rover given a position string and optional plateau.
 */
 Rover make_rover(const std::string& position, 
     PlateauPtr plateau = PlateauPtr());

 /*
  * @brief Execute a list of actions.
  * @param actions - a string of [LMR] characters.
  */
 Rover command(Rover r, const std::string& actions);

/*
 * @brief Simulate a single rover exploring a plateau.
 *
 * @param rc (Rover-position, command-list) pair.
 * @param p Plateau the Rover will explore.
 */
Rover simulate_rover(RoverCommand& rc, PlateauPtr p);

void simulate_squad( std::vector<RoverCommand>& rcs, 
    const std::string& plateau_upper_right,
    std::vector<Rover>& squad_output);

}

#endif

