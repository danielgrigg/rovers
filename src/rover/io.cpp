#include "io.h"
#include <istream>
#include <iterator>

namespace rv {

  bool parse_rover_world(std::istream& is, 
      std::string& upper_right,
      std::vector<RoverCommand>& rover_commands) {

    rover_commands.clear();

    // First (necessary) line is upper-right coordinate
    if (!std::getline(is, upper_right)) {
      return false;
    }

    // Read line pairs of (rover-position, command-list).
    // We leave content validation to the client.
    std::string r, cs;
    while (getline(is, r) && getline(is, cs)) {
      rover_commands.push_back(std::tr1::make_tuple(r, cs));
    }

    // We need at least one valid rover/command list.
    if (rover_commands.empty()) return false;

    return true;
  }
}

