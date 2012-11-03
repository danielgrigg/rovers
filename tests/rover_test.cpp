#include "rover.h"
#include <sstream>

using namespace rv;

#include <gtest/gtest.h>
                                                                                
TEST(RoverTest, can_position_rover) {
  EXPECT_EQ(7, Rover(7,3,'E').x());
  EXPECT_EQ(3, Rover(7,3,'E').y());
  EXPECT_EQ('E',Rover(7,3,'E').facing()); 
}  

TEST(RoverTest, can_compare_rovers) {
  EXPECT_EQ(Rover(3, 4, 'N'), Rover(3, 4, 'N'));
  EXPECT_NE(Rover(3, 4, 'W'), Rover(3, 4, 'N'));
}

TEST(RoverTest, can_serialize_rover) {
  std::ostringstream ss;
  ss << Rover(3, 7, 'S');
  EXPECT_EQ("3 7 S", ss.str());
}

TEST(RoverTest, can_parse_position) {
  EXPECT_EQ(Rover(14, 323, 'W'), *make_rover("14 323 w"));  
  EXPECT_EQ(Rover(1, 1, 'N'), *make_rover("1 1 n"));  
  EXPECT_EQ(RoverPtr(), make_rover("N N N"));
  EXPECT_EQ(RoverPtr(), make_rover("-3 4 N"));
  EXPECT_EQ(RoverPtr(), make_rover("3 4 J"));
  EXPECT_EQ(RoverPtr(), make_rover("3.3 4.2 N"));
}

TEST(RoverTest, can_turn_left) {
  EXPECT_EQ(Rover(1, 1, 'S'), make_rover("1 1 w")->left());
  EXPECT_EQ(Rover(0, 0, 'E'), make_rover("0 0 s")->left());
  EXPECT_EQ(Rover(0, 0, 'N'), make_rover("0 0 e")->left());
  EXPECT_EQ(Rover(3, 8, 'S'), make_rover("3 8 W")->left());
}

TEST(RoverTest, can_turn_right) {
  EXPECT_EQ(Rover(0, 0, 'E'), make_rover("0 0 n")->right());
  EXPECT_EQ(Rover(0, 0, 'S'), make_rover("0 0 e")->right());
  EXPECT_EQ(Rover(0, 0, 'W'), make_rover("0 0 s")->right());
  EXPECT_EQ(Rover(0, 0, 'N'), make_rover("0 0 w")->right());
  EXPECT_EQ(Rover(3, 8, 'N'), make_rover("3 8 w")->right());
}

TEST(RoverTest, can_move_forward) {
  EXPECT_EQ(Rover(5, 6, 'N'), make_rover("5 5 N")->move());
  EXPECT_EQ(Rover(6, 5, 'E'), make_rover("5 5 e")->move());
  EXPECT_EQ(Rover(5, 4, 'S'), make_rover("5 5 s")->move());
  EXPECT_EQ(Rover(4, 5, 'W'), make_rover("5 5 w")->move());
}

TEST(RoverTest, can_sequence_turns) {
  EXPECT_EQ(Rover(5, 5, 'S'), 
      make_rover("5 5 n")->left()
      .left()
      .left()
      .right());

  RoverPtr q = make_rover("5 5 e");
  for (int i = 0; i < 10; ++i) {
    q->left();
    q->right();
  }
  EXPECT_EQ(Rover(5, 5, 'E'), *q);
}

TEST(RoverTest, can_sequence_actions) {
  RoverPtr r = make_rover("5 5 n");
  r->move(); r->move(); r->move();
  EXPECT_EQ(Rover(5, 8, 'N'), *r);

  r->left(); r->move(); r->move();
  EXPECT_EQ(Rover(3, 8, 'W'), *r);

  r->left(); r->move(); r->move(); r->move();
  EXPECT_EQ(Rover(3, 5, 'S'), *r);

  r->left(); r->move(); r->move();
  EXPECT_EQ(Rover(5, 5, 'E'), *r);
}

// plateau edges

TEST(RoverTest, stays_on_plateau) {
  //TODO - this test will require refactoring for plateau extents
  EXPECT_EQ(Rover(0, 0, 'W'), make_rover("0 0 W")->move().move());
}

