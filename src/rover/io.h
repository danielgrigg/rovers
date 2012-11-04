#ifndef RV_IO_H
#define RV_IO_H

#include <iosfwd>
#include <string>
#include <vector>
#include <tr1/tuple>

namespace rv {

  // What a mouthful.
  typedef std::tr1::tuple<std::string, std::string> RoverCommand;

// Parse the plateau, and pairs of rover initial-positions with
// their command lists.
bool parse_rover_world(std::istream& is, 
    std::string& upper_right,
    std::vector<RoverCommand>& rover_commands);

}

#endif

