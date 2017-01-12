#include "treeItem.hpp"
#include <cstdlib>

TreeItem::TreeItem(int parentID, int ID, int depth, char curr_val,
 std::pair<int, int> curr_cell, int N, int M, int operations)
{
  /// Initialize variables
  this->curr_cell = curr_cell;
  this->parentID = parentID;
  this->ID = ID;
  this->depth = depth;
  this->curr_val = curr_val;
  this->operations = operations;

  /// Make list of valid next cell values from this cell
  if ((curr_val == 'U') || (curr_cell.first == N - 1))
  {
    next_cell_vals.erase(std::remove(next_cell_vals.begin(),
                                     next_cell_vals.end(),
                                     'D'), next_cell_vals.end());
  }
  else if ((curr_val == 'L') || (curr_cell.second == M - 1))
  {
    next_cell_vals.erase(std::remove(next_cell_vals.begin(),
                                     next_cell_vals.end(),
                                     'R'), next_cell_vals.end());
  }
  else if ((curr_val == 'D') || (curr_cell.first == 0))
  {
    next_cell_vals.erase(std::remove(next_cell_vals.begin(),
                                     next_cell_vals.end(),
                                     'U'), next_cell_vals.end());
  }
  else if ((curr_val == 'R') || (curr_cell.second == 0))
  {
    next_cell_vals.erase(std::remove(next_cell_vals.begin(),
                                     next_cell_vals.end(),
                                     'L'), next_cell_vals.end());
  }
}