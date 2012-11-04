#include "rover.h"
#include "plateau.h"
#include "config.h"

#include <iostream>
#include <sstream>
#include <string>
#include <boost/program_options.hpp>

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
    std::cerr << "rovers has encountered an unknown error " 
      << " and must exit." << std::endl;
    return 1;
  }

  return 0;
}

