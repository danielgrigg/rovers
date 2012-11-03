#include "rover.h"
#include "config.h"

#include <iostream>
#include <sstream>
#include <string>

using std::cout;
using std::endl;

// Version pulled from cmake                                                    
std::string version_string() {
  std::stringstream ss;
  ss << "rovers version " << ROVERS_VERSION_MAJOR << "." 
    << ROVERS_VERSION_MINOR << "\n";
  return ss.str();
}

int main(int argc, char* argv[]) {

  cout << version_string() << endl;
  return 0;
}

