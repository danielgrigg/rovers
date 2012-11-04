#include "rover.h"
#include "plateau.h"
#include "config.h"
#include "io.h"

#include <iostream>
#include <sstream>
#include <string>
#include <boost/program_options.hpp>


using namespace std::tr1;

// Version pulled from cmake                                                    
std::string version_string() {
  std::stringstream ss;
  ss << "rovers version " << ROVERS_VERSION_MAJOR << "." 
    << ROVERS_VERSION_MINOR << "\n";
  return ss.str();
}

std::string usage_string() {
  std::stringstream ss;
  ss << "Usage\n\n"
    << "  rovers [options]\n\n"
    << "Running\n\n"
    << "  TODO\n";
  return ss.str();
}

std::string example_input() {
  return "5 5\n"
    "1 2 N\n"
    "LMLMLMLMM\n"
    "3 3 E\n"
    "MMRMMRMRRM";
}

const std::string UNKNOWN_ERROR = 
  "rovers has encountered an unknown error and must exit";

const std::string INVALID_INPUT_ERROR = 
  "Invalid input format.  Read problem_description.txt for "
  "specs or run with --help for an example.";


int main(int argc, char* argv[]) {
  using namespace rv;
  using namespace std;

  namespace po = boost::program_options;

  // We only have one option atm, but '--help' is universally expected for
  // command-line utils.
  try {
    po::options_description desc("Available options");
    desc.add_options()
      ("help", "display help message");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
      cout << version_string() << "\n" << usage_string() << "\n" << desc 
           << endl;
      return 0;
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << endl;
    return 1;
  } catch (...) {
    std::cerr << UNKNOWN_ERROR << endl;
    return 1;
  }

  std::string upper_right;
  std::vector<RoverCommand> rover_commands;

  if (parse_rover_world(std::cin, upper_right, rover_commands)) {

    try {
      std::vector<rv::Rover> squad;
      simulate_squad(rover_commands, upper_right, squad);

      std::copy(squad.begin(), squad.end(), 
          std::ostream_iterator<rv::Rover>(std::cout, "\n"));
    }
    catch (std::invalid_argument e) {
      std::cerr << "Simulation aborted: " << e.what() << endl;
      return 1;
    }
    catch (...) {
      std::cerr << UNKNOWN_ERROR << endl;
      return 1;
    }

  } else {
    std::cerr << INVALID_INPUT_ERROR << endl;
    return 1;
  }

  return 0;
}

