#include "tree_item.hpp"


Tree_item::Tree_item(int ID, int parent_ID, std::pair<int, int> position,
 char value, char **current_board, int rows, int cols, int depth,
 int operations)
{
  this->ID = ID;
  this->parent_ID = parent_ID;
  this->position = position;
  this->value = value;
  this->depth = depth;
  this->operations = operations;

  this->current_board = new char*[rows];
  for (int i = 0; i < rows; i++)
  {
    this->current_board[i] = new char[cols];
    for (int j = 0; j < cols; j++)
    {
      this->current_board[i][j] = current_board[i][j];
    }
  }

  if (value == 'T')
  {
    // Root node
    children_vals = {'D', 'R'};
    children_visited = {false, false};
  }
  else
  {
    if (position.first > 0 && value != 'D')
    {
      children_vals.push_back('U');
      children_visited.push_back(false);
    }
    if (position.first < rows - 1 && value != 'U')
    {
      children_vals.push_back('D');
      children_visited.push_back(false);
    }
    if (position.second > 0 && value != 'R')
    {
      children_vals.push_back('L');
      children_visited.push_back(false);
    }
    if (position.second < cols - 1 && value != 'L')
    {
      children_vals.push_back('R');
      children_visited.push_back(false);
    }
  }
}


