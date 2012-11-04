
/*
 * Rover Plateau integration tests.
 */

#include "rover.h"
#include "plateau.h"
#include <gtest/gtest.h>
#include <stdexcept>

using namespace rv;

// Useful for ensuring rover movements don't increase or decrease 
// plateau occupancy.
int count_occupied(PlateauPtr p) {
  // Multi-Array doesn't support a whole container iterator.
  int n = 0;
  for (int y = 0; y < p->y_size() ; ++y) {
    for (int x = 0; x < p->x_size(); ++x) {
      if (p->occupied(x, y)) ++n;
    }
  }
  return n;
}


TEST(RoverPlateauTest, rover_can_descend) {
  PlateauPtr p = make_plateau(3,3);
  Rover r = make_rover("1 1 N", p);
  EXPECT_TRUE(p->occupied(r.x(), r.y()));

  // Toss some rovers over the edge. (Sorry little dudes :( ).
  EXPECT_THROW(make_rover("1 1 E", p), std::invalid_argument);
  EXPECT_THROW(make_rover("0 -2 E", p), std::invalid_argument);
  EXPECT_THROW(make_rover("4 2 E", p), std::invalid_argument);

  // Ensure state of plateau is still valid.
  Rover q = make_rover("1 2 N", p);
  EXPECT_TRUE(p->occupied(q.x(), q.y()));
  EXPECT_EQ(2, count_occupied(p));
}


TEST(RoverPlateauTest, rover_can_spin) {
  // Check the rover stays where it is.

  PlateauPtr p = make_plateau(5,5);
  Rover r = make_rover("2 2 N", p);
  r = r.left().left().left();
  EXPECT_TRUE(p->occupied(r.x(), r.y()));

  EXPECT_EQ(1, count_occupied(p));
}

TEST(RoverPlateauTest, rover_can_move) {
  PlateauPtr p = make_plateau(5,5);
  Rover r = make_rover("3 3 N", p);
  EXPECT_FALSE(p->occupied(5, 5));
  r = r.move().right().move().move().left().move();
  EXPECT_TRUE(p->occupied(5, 5));
  EXPECT_EQ(1, count_occupied(p));

  r = r.move().move().left().move().move();
  EXPECT_TRUE(p->occupied(3, 5));
  
}

TEST(RoverPlateauTest, rover_squad_can_move) {
  PlateauPtr p = make_plateau(5, 5);
  Rover q = make_rover("1 1 E", p).move().move();
  Rover r = make_rover("2 2 E", p).move().move().move();
  Rover s = make_rover("3 3 W", p).move().move();
  EXPECT_TRUE(p->occupied(3, 1));
  EXPECT_TRUE(p->occupied(5, 2));
  EXPECT_TRUE(p->occupied(1, 3));
  EXPECT_EQ(3, count_occupied(p));
}


TEST(RoverPlateauTest, rover_squad_can_overlap_paths) {
  PlateauPtr p = make_plateau(5, 5);
  Rover q = make_rover("1 1 E", p).move().move();
  Rover r = make_rover("2 2 N", p).move().move().move();
  Rover s = make_rover("3 3 W", p).move().left().move().move().move();
  EXPECT_TRUE(p->occupied(3, 1));
  EXPECT_TRUE(p->occupied(2, 5));
  EXPECT_TRUE(p->occupied(2, 0));
  EXPECT_EQ(3, count_occupied(p));

}

TEST(RoverPlateauTest, rovers_respect_personal_space) {
  PlateauPtr p = make_plateau(5, 5);
  Rover q = make_rover("1 1 E", p).move().move(); 
  Rover r = make_rover("3 3 S", p).move().move().move();
  EXPECT_TRUE(p->occupied(3, 2));
  EXPECT_TRUE(p->occupied(3, 1));
  EXPECT_EQ(2, count_occupied(p));
}
 

TEST(RoverPlateauTest, rover_squads_obey_commands) {
  // Integrate rovers, command processing and plateaus.
  PlateauPtr p = make_plateau(5, 5);
  command(make_rover("1 1 E", p), "MM");
  command(make_rover("2 2 N", p), "MMM");
  command(make_rover("3 3 W", p), "MLMMM");
  EXPECT_TRUE(p->occupied(3, 1));
  EXPECT_TRUE(p->occupied(2, 5));
  EXPECT_TRUE(p->occupied(2, 0));

}
