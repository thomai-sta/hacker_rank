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
  std::pair<int, int> next_cell = get_next_cell();
  if ((curr_val == 'U') || (next_cell.first == N - 1))
  {
    next_cell_vals.erase(std::remove(next_cell_vals.begin(),
                                     next_cell_vals.end(),
                                     'D'), next_cell_vals.end());
  }
  if ((curr_val == 'L') || (next_cell.second == M - 1))
  {
    next_cell_vals.erase(std::remove(next_cell_vals.begin(),
                                     next_cell_vals.end(),
                                     'R'), next_cell_vals.end());
  }
  if ((curr_val == 'D') || (next_cell.first == 0))
  {
    next_cell_vals.erase(std::remove(next_cell_vals.begin(),
                                     next_cell_vals.end(),
                                     'U'), next_cell_vals.end());
  }
  if ((curr_val == 'R') || (next_cell.second == 0))
  {
    next_cell_vals.erase(std::remove(next_cell_vals.begin(),
                                     next_cell_vals.end(),
                                     'L'), next_cell_vals.end());
  }
}


std::pair<int, int> TreeItem::get_next_cell()
{
  std::pair<int, int> next_cell;
  if (curr_val == 'T')
  {
    next_cell.first = 0;
    next_cell.second = 0;
    return next_cell;
  }
  switch(curr_val)
  {
    case 'U' :
            next_cell.first = curr_cell.first - 1;
            next_cell.second = curr_cell.second;
            break;
    case 'L' :
            next_cell.first = curr_cell.first;
            next_cell.second = curr_cell.second - 1;
            break;
    case 'D' :
            next_cell.first = curr_cell.first + 1;
            next_cell.second = curr_cell.second;
            break;
    case 'R' :
            next_cell.first = curr_cell.first;
            next_cell.second = curr_cell.second + 1;
            break;
  }
  return next_cell;
}