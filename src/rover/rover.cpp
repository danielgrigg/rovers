#include "rover.h"
#include <iostream>
#include <string>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <tr1/tuple>
#include <stdexcept>


namespace rv {
  using namespace boost::assign;
  using namespace std::tr1;

 const std::map<const char, const char> RIGHT_TURNS = map_list_of 
  ('N', 'E')
  ('E', 'S')
  ('S', 'W')
  ('W', 'N');

 const std::map<const char, const char> LEFT_TURNS = map_list_of
  ('N', 'W')
  ('W', 'S')
  ('S', 'E')
  ('E', 'N');

 const std::map<const char, const tuple<int, int> > MOVES = map_list_of
  ('N', make_tuple(0, 1))
  ('E', make_tuple(1, 0))
  ('S', make_tuple(0, -1))
  ('W', make_tuple(-1, 0));

 // Simplify access of immutable maps.
 template <typename K, typename V>
V constmap_get(const K key, const std::map<const K, V>& m) {
  typename std::map<K, V>::const_iterator x = m.find(key);
  if (x == m.end()) throw std::invalid_argument("key");
  return x->second;
}
 
Rover& Rover::left() {
  _facing = constmap_get(facing(), LEFT_TURNS);
  return *this;
}

Rover& Rover::right() {
  _facing = constmap_get(facing(), RIGHT_TURNS);
  return *this;
}

Rover& Rover::move() {
  int dx, dy;
  tie(dx, dy) = constmap_get(facing(), MOVES);
  _x = std::max(0, x() + dx);
  _y = std::max(0, y() + dy);
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

