#include "plateau.h"
#include <stdexcept>
#include <gtest/gtest.h>
using namespace rv;

int count_empty(PlateauPtr p) {
  // Multi-Array doesn't support a whole container iterator.
  int n = 0;
  for (int y = 0; y < p->y_size() ; ++y) {
    for (int x = 0; x < p->x_size(); ++x) {
      if (!p->occupied(x, y)) ++n;
    }
  }
  return n;
}

                                                                                
// An unexplored plateau of w*h size must consist of w*h unoccupied tiles.
TEST(PlateauTest, new_plateau_valid) {
  EXPECT_THROW(make_plateau(-3, 2), std::invalid_argument);
  EXPECT_THROW(make_plateau(3, -2), std::invalid_argument);

  EXPECT_EQ(1, count_empty(make_plateau(0, 0)));
  EXPECT_EQ(4, count_empty(make_plateau(1, 1)));
  EXPECT_EQ(308, count_empty(make_plateau(21, 13)));

  EXPECT_EQ(13, make_plateau(9, 12)->y_size());
  EXPECT_EQ(13, make_plateau(12, 9)->x_size());

  EXPECT_THROW(make_plateau("33 x"), std::invalid_argument);
  EXPECT_EQ(12, count_empty(make_plateau("3 2")));
  EXPECT_EQ(6, count_empty(make_plateau("  2   1 ")));
}  

TEST(PlateauTest, can_descend) {

  // Check off plateau
  EXPECT_FALSE(make_plateau(2,2)->descend(3,1));
  EXPECT_FALSE(make_plateau(2,2)->descend(1,3));
  EXPECT_FALSE(make_plateau(2,2)->descend(-1,1));
  EXPECT_FALSE(make_plateau(2,2)->descend(1,-1));

  // Inside plateau
  EXPECT_TRUE(make_plateau(2,2)->descend(1,1));

  // Rovers squashing rovers, uh oh!
  PlateauPtr p = make_plateau(2,2);
  p->descend(1,1);
  EXPECT_FALSE(p->descend(1,1));
  EXPECT_FALSE(p->descend(1,1)); // Multiple descents to tile
  EXPECT_TRUE(p->descend(1,2));
}

TEST(PlateauTest, can_move) {
  PlateauPtr p = make_plateau(2,2);

  p->descend(0,0);
  EXPECT_FALSE(p->move(0, 0, 0, 0));

  // Verify still in (0,0)
  EXPECT_FALSE(p->descend(0,0));

  // Check movements across plateau boundaries. 
  EXPECT_FALSE(p->move(0, 0, -1, 0));
  EXPECT_FALSE(p->descend(0,0));
  EXPECT_FALSE(p->move(0, 0, 0, -1));
  EXPECT_FALSE(p->descend(0,0));
  EXPECT_FALSE(p->move(-1, 0, 0, 0));
  EXPECT_FALSE(p->move(0, 7, 0, 0));

  EXPECT_TRUE(p->descend(2,2));
  EXPECT_FALSE(p->move(2,2,1,0));
  EXPECT_FALSE(p->move(2,2,0,1));
  EXPECT_FALSE(p->descend(2,2));

  // Check in-bound moves are ok
  EXPECT_TRUE(p->move(2,2, -1, 0));
  EXPECT_FALSE(p->descend(1, 2));
  EXPECT_TRUE(p->descend(2, 2));
  EXPECT_TRUE(p->move(2,2, 0, -1));
  EXPECT_FALSE(p->descend(2, 1));

  // Check we're limited to 0 to 1 tile moves.
  PlateauPtr q = make_plateau(5,5);
  q->descend(2,2);
  EXPECT_FALSE(q->move(2,2,2,0));
  EXPECT_FALSE(q->descend(2, 2));
  EXPECT_FALSE(q->move(2,2,0,2));
  EXPECT_FALSE(q->move(2,2,1,1));
  EXPECT_FALSE(q->move(2,2,-1,-1));
}

TEST(PlateauTest, can_move_multiple) {
  PlateauPtr q = make_plateau(5,5);
  q->descend(1,1);
  q->descend(3,3);
  q->descend(2,2);

  EXPECT_FALSE(q->occupied(3,2));
  EXPECT_TRUE(q->move(1,1,1,0));
  EXPECT_TRUE(q->move(2,2,0,1));
  EXPECT_TRUE(q->move(2,1,0,1));
  EXPECT_TRUE(q->move(2,2,1,0));
  EXPECT_FALSE(q->move(3,2,0,1));
  EXPECT_FALSE(q->descend(3,2));
}


