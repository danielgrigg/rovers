#include "io.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace rv;

TEST(RoversParser, can_parse_input) {
// Check we can parse an input stream

  std::string input("5 5\n"
      "1 2 N\n"
      "LMLMLMLMM\n"
      "3 3 E\n"
      "MMRMMRMRRM");

  std::istringstream is(input);
  std::string xy;
  std::vector<RoverCommand> rc;
  EXPECT_TRUE(parse_rover_world(is, xy, rc));

  EXPECT_EQ("5 5", xy);
  EXPECT_EQ("1 2 N", std::tr1::get<0>(rc[0]));
  EXPECT_EQ("LMLMLMLMM", std::tr1::get<1>(rc[0]));
  EXPECT_EQ("3 3 E", std::tr1::get<0>(rc[1]));
  EXPECT_EQ("MMRMMRMRRM", std::tr1::get<1>(rc[1]));
 }

TEST(RoversParser, can_parse_empty) {
  std::string input("");
  std::istringstream is(input);
  std::string xy;
  std::vector<RoverCommand> rc;
  EXPECT_FALSE(parse_rover_world(is, xy, rc));
}

TEST(RoversParser, can_parse_partial) {
  {
    std::string input("3 7\n"
        "2 3 N\n"
        "LMLMLMLMM\n"
        "3 5 E\n");

    std::istringstream is(input);
    std::string xy;
    std::vector<RoverCommand> rc;
    EXPECT_TRUE(parse_rover_world(is, xy, rc));

    // Ignore incomplete trailing rover-commands.
    EXPECT_EQ(1, rc.size());

    EXPECT_EQ("3 7", xy);
    EXPECT_EQ("2 3 N", std::tr1::get<0>(rc[0]));
    EXPECT_EQ("LMLMLMLMM", std::tr1::get<1>(rc[0]));
  }
  {
    std::string input("3 7\n"
        "2 3 N\n");
    std::istringstream is(input);
    std::string xy;
    std::vector<RoverCommand> rc;
    EXPECT_FALSE(parse_rover_world(is, xy, rc));
  }
}
