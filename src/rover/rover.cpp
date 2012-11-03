#include "rover.h"
#include <iostream>
#include <string>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <tr1/tuple>
#include <stdexcept>
#include <tr1/functional>
#include <numeric>


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

  const std::map<const char, function<Rover(Rover&)> > COMMANDS = 
    map_list_of 
    ('M', &Rover::move)
    ('L', &Rover::left)
    ('R', &Rover::right);


  // Simplify access of immutable maps.
  template <typename K, typename V>
    V constmap_get(const K key, const std::map<const K, V>& m) {
      typename std::map<K, V>::const_iterator x = m.find(key);
      if (x == m.end()) throw std::invalid_argument("key");
      return x->second;
    }

  Rover Rover::left()const {
    return Rover(x(), y(), constmap_get(facing(), LEFT_TURNS));
  }

  Rover Rover::right()const {
    return Rover(x(), y(), constmap_get(facing(), RIGHT_TURNS));
  }

  Rover Rover::move()const {
    int dx, dy;
    tie(dx, dy) = constmap_get(facing(), MOVES);
    return Rover(std::max(0, x() + dx), std::max(0, y() + dy), facing());
  }

  bool Rover::operator==( const Rover& b)const {
    return x() == b.x() && y() == b.y() && facing() == b.facing();
  }

  std::ostream& operator<<(std::ostream& os, const Rover& r) {
    os << r.x() << ' ' << r.y() << ' ' << r.facing();
    return os;
  }

  static const boost::regex position_exp("^(\\d+) (\\d+) ([NESW])$");
  static const boost::regex actions_exp("^([LMR]*)");

  Rover make_rover(const std::string& position) {
    std::string pos_up = boost::to_upper_copy(position);
    boost::smatch sm;
    if (boost::regex_match(pos_up, sm, position_exp)) {
      std::string facing_str = sm[3];

      return Rover(boost::lexical_cast<int>(sm[1]), 
          boost::lexical_cast<int>(sm[2]), 
          facing_str[0]);
    } else { 
      throw std::invalid_argument("position");
    }
  }

  Rover command_once(Rover r, const char a) {
    return constmap_get(a, COMMANDS)(r);
  }

  Rover command(Rover r, const std::string& actions) {
    std::string s = boost::trim_copy(actions);
    boost::to_upper(s);

    boost::smatch sm;

    // Match any subsequence so we invoke all leading, valid commands.
    if (boost::regex_search(s, sm, actions_exp)) {
      std::string cmds(sm[1].first, sm[1].second);
      return std::accumulate(cmds.begin(), cmds.end(), r, command_once); 
    }
    return r;
  }

}

