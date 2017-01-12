#ifndef _TREEITEM_HPP_
#define _TREEITEM_HPP_

#include <vector>
#include <algorithm>
#include <iostream>


class TreeItem
{
public:
  std::pair<int, int> curr_cell;
  char curr_val, parent_val;
  int ID, parentID, depth, operations;
  std::vector<int> childrenID;
  std::vector<char> next_cell_vals = {'U', 'L', 'D', 'R'};
  bool contains_board_move = false, found_goal = false;

public:
  TreeItem(int parentID, int ID, int depth, char curr_val, std::pair<int, int> curr_cell, int N, int M, int operations);
  std::pair<int, int> get_next_cell();
};

#endif