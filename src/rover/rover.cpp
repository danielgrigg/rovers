#include "rover.h"
#include "plateau.h"
#include <iostream>
#include <string>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp> 
#include <stdexcept>
#include <numeric>
#include <iterator>
#include <tr1/tuple>
#include <tr1/functional>

namespace rv {
  using namespace boost::assign;
  using namespace std::tr1;

  // Cache right-turn combinations
  const std::map<const char, const char> RIGHT_TURNS = map_list_of 
    ('N', 'E')
    ('E', 'S')
    ('S', 'W')
    ('W', 'N');

  // Cache left-turn combinations
  const std::map<const char, const char> LEFT_TURNS = map_list_of
    ('N', 'W')
    ('W', 'S')
    ('S', 'E')
    ('E', 'N');

  // Cache movement deltas
  const std::map<const char, const tuple<int, int> > MOVES = map_list_of
    ('N', make_tuple(0, 1))
    ('E', make_tuple(1, 0))
    ('S', make_tuple(0, -1))
    ('W', make_tuple(-1, 0));

  // Map commands to rover actions.
  const std::map<const char, function<Rover(Rover&)> > COMMANDS = 
    map_list_of 
    ('M', &Rover::move)
    ('L', &Rover::left)
    ('R', &Rover::right);


  // Simplify access of immutable maps.
  template <typename K, typename V>
    V constmap_get(const K key, const std::map<const K, V>& m) {
      typename std::map<K, V>::const_iterator x = m.find(key);
      if (x == m.end()) throw std::invalid_argument("Key does not exist!");
      return x->second;
    }

  Rover::Rover(int x, int y, char facing, PlateauPtr p):
      _x(x),
      _y(y),
      _facing(facing),
      _plateau(p) {}

  Rover Rover::left() {
    return Rover(x(), y(), constmap_get(facing(), LEFT_TURNS), _plateau);
  }

  Rover Rover::right() {
    return Rover(x(), y(), constmap_get(facing(), RIGHT_TURNS), _plateau);
  }

  Rover Rover::move() {
    int dx, dy;
    tie(dx, dy) = constmap_get(facing(), MOVES);

    // For testing purposes we allow a Rover to exist independently
    // of a plateau.  Imagine Wall-E floating in a vacuum...
    if (!_plateau || _plateau->move(x(), y(), dx, dy)) {
      return Rover(x() + dx, y() + dy, facing(), _plateau);
    }
    return *this;
  }

  bool Rover::operator==( const Rover& b)const {
    return x() == b.x() && y() == b.y() && facing() == b.facing();
  }

  std::ostream& operator<<(std::ostream& os, const Rover& r) {
    os << r.x() << ' ' << r.y() << ' ' << r.facing();
    return os;
  }

  // All positions must be a "X Y F" triple.
  static const boost::regex position_exp("^(\\d+) (\\d+) ([NESW])$");

  // All command lists must be a sequence of {L M R} characters.
  static const boost::regex actions_exp("^([LMR]*)");

  Rover make_rover(const std::string& position, 
      PlateauPtr plateau ) {
    std::string pos_up = boost::to_upper_copy(position);
    boost::smatch sm;
    if (boost::regex_match(pos_up, sm, position_exp)) {
      const char facing = std::string(sm[3])[0];
      const int x = boost::lexical_cast<int>(sm[1]);
      const int y = boost::lexical_cast<int>(sm[2]);

      if (plateau && !plateau->descend(x, y)) {
          throw std::invalid_argument(
              "Rover crashed into a rover or outside the plateau!");
      }
      return Rover(x, y, facing, plateau);
    } else { 
      throw std::invalid_argument("Rover position is invalid");
    }
  }

  // Execute a single action.
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

Rover simulate_rover(RoverCommand& rc, PlateauPtr p) {
  std::string position, actions;
  tie(position, actions) = rc;
  return command(make_rover(position, p), actions);
}


void simulate_squad(std::vector<RoverCommand>& rcs, 
    const std::string& plateau_upper_right,
    std::vector<Rover>& squad_output) {

  PlateauPtr plateau = make_plateau(plateau_upper_right);

  squad_output.clear();
  squad_output.reserve(rcs.size());

  // Fly my pretties!
  using std::tr1::placeholders::_1;
  std::transform(rcs.begin(), rcs.end(), std::back_inserter(squad_output),
      bind(simulate_rover, _1, plateau));
}

}

