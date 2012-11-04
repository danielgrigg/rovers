/*
 * The mighty rovers app.  
 * The app manages all the awkward but necessary controller business.
 * Most of the logic is delegated to the Rover API though.
 */

#include "rover.h"
#include "plateau.h"
#include "config.h"
#include "io.h"

#include <iostream>
#include <sstream>
#include <string>
#include <boost/program_options.hpp>
#include <fstream>

using namespace std::tr1;

// Version pulled from cmake                                                    
std::string version_string() {
  std::stringstream ss;
  ss << "rovers version " << ROVERS_VERSION_MAJOR << "." 
    << ROVERS_VERSION_MINOR << "\n";
  return ss.str();
}

const char* EXAMPLE_INPUT = "    5 5\n"
                            "    1 2 N\n"
                            "    LMLMLMLMM\n"
                            "    3 3 E\n"
                            "    MMRMMRMRRM\n"
                            "    ^D\n";

std::string usage_string() {
  std::stringstream ss;
  ss << "Usage\n\n"
    << "  rovers [options]\n\n"
    << "Running\n\n"
    << "  With no arguments specified, rovers reads simulation input from"
    << " stdin.\n"
    << "  An example session of 2 rovers on a 6x6 plateau:\n"
    << EXAMPLE_INPUT
    << "  Where ^D signals end-of-input.\n";
    return ss.str();
}

// For those time when no other exception matches.
const std::string UNKNOWN_ERROR = 
"rovers has encountered an unknown error and must exit";

const std::string INVALID_INPUT_ERROR = 
"Invalid input format.  Read problem_description.txt for "
"specs or run with --help for an example.";


int main(int argc, char* argv[]) {
  using namespace rv;
  using std::cout;
  using std::endl;

  namespace po = boost::program_options;

  std::string infile;
  try {
    po::options_description desc("Available options");
    desc.add_options()
      ("help", "display help message")
      ("file", po::value<std::string>(), "read simulation input from a file");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
      cout << version_string() << "\n" << usage_string() << "\n" << desc 
        << endl;
      return 0;
    } else if (vm.count("file")) {
      infile = vm["file"].as<std::string>();
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << endl;
    return 1;
  } catch (...) {
    std::cerr << UNKNOWN_ERROR << endl;
    return 1;
  }

  std::ifstream ifs;
  std::streambuf* cin_buf = NULL;

  // Redirect std::cin to read from a file if the user wants it.
  if (!infile.empty()) {
    ifs.open(infile.c_str());
    if (!ifs.good()) {
      std::cerr << "Unable to read input file\n";
      return 1;
    }
    cin_buf = std::cin.rdbuf();
    std::cin.rdbuf(ifs.rdbuf());
  }

  std::string upper_right;
  std::vector<RoverCommand> rover_commands;
  if (parse_rover_world(std::cin, upper_right, rover_commands)) {
    try {

      // Run the simulation!
      std::vector<rv::Rover> squad;
      simulate_squad(rover_commands, upper_right, squad);

      // Write out each rover's position
      std::copy(squad.begin(), squad.end(), 
          std::ostream_iterator<rv::Rover>(std::cout, "\n"));
    }
    catch (std::invalid_argument e) {
      std::cerr << "Simulation aborted: " << e.what() << endl;
    }
    catch (...) {
      std::cerr << UNKNOWN_ERROR << endl;
    }

  } else {
    std::cerr << INVALID_INPUT_ERROR << endl;
  }

  // Restore cin's rdbuf if we overwrote it.
  if (cin_buf) std::cin.rdbuf(cin_buf);
  ifs.close();
  return 0;
}

