#include "plateau.h"
#include <algorithm>
#include <stdexcept>

using namespace rv;

#include <gtest/gtest.h>

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
}  

TEST(PlateauTest, can_enter_tiles) {
  PlateauPtr p = make_plateau(0,0);
  EXPECT_TRUE(p->enter(0,0));
  EXPECT_FALSE(p->enter(1,0));
  EXPECT_FALSE(p->enter(0,1));
  EXPECT_FALSE(make_plateau(3,3)->enter(-2,1));
  EXPECT_FALSE(make_plateau(3,3)->enter(2,-1));
  EXPECT_TRUE(make_plateau(3,7)->enter(2, 5));
}

TEST(PlateauTest, single_occupancy_per_tile) {
  PlateauPtr p = make_plateau(2,2);
  p->enter(1,1);
  EXPECT_FALSE(p->enter(1,1));
  EXPECT_TRUE(p->enter(1,2));
}

TEST(PlateauTest, can_leave_tiles) {

  // TDB - Can always leave empty tiles.  But 'should' we allow this?
  EXPECT_TRUE(make_plateau(2,2)->leave(1,1));

  EXPECT_FALSE(make_plateau(2,2)->leave(-1,1));
  EXPECT_FALSE(make_plateau(2,2)->leave(1,-1));

  PlateauPtr p = make_plateau(2,2);
  p->enter(1,1);
  EXPECT_TRUE(p->leave(1,1));
  EXPECT_TRUE(p->enter(1,1));
}

