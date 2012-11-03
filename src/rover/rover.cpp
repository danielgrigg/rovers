#include "rover.h"
#include <iostream>
#include <string>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>


namespace rv {


  using namespace boost::assign;

 std::map<const char, const char> RIGHT_TURNS = map_list_of 
  ('N', 'E')
  ('E', 'S')
  ('S', 'W')
  ('W', 'N');

 std::map<const char, const char> LEFT_TURNS = map_list_of
  ('N', 'W')
  ('W', 'S')
  ('S', 'E')
  ('E', 'N');

 
const Rover& Rover::left() {
  return *this;
}

const Rover& Rover::right() {
  return *this;
}

const Rover& Rover::move() {
  return *this;
}


bool Rover::operator==( const Rover& b)const {
  return x() == b.x() && y() == b.y() && facing() == b.facing();
}

std::ostream& operator<<(std::ostream& os, const Rover& r) {
  os << r.x() << ' ' << r.y() << ' ' << r.facing();
  return os;
}

static const boost::regex position_exp("(\\d+) (\\d+) ([NESW])");

RoverPtr make_rover(const std::string& position) {
  std::string pos_upper = boost::to_upper_copy(position);
  boost::smatch sm;
  if (boost::regex_match(pos_upper, sm, position_exp)) {
    int x = boost::lexical_cast<int>(sm[1]);
    int y = boost::lexical_cast<int>(sm[2]);
    std::string facing_str = sm[3];
    return RoverPtr(new Rover(x, y, facing_str[0]));
  } else { 
    return RoverPtr();
  }
}



}

