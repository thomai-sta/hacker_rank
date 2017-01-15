#include "tree_item.hpp"


Tree_item::Tree_item(std::pair<int, int> position,
 char value, int rows, int cols, int depth,
 int operations, std::vector<std::pair<int, int> > parents_path)
{
  this->position = position;
  this->value = value;
  this->depth = depth;
  this->operations = operations;

  private_path = parents_path;
  private_path.push_back(position);

  std::pair<int, int> next_cell = get_next_cell();

  if (value == 'T')
  {
    // Root node
    children = {'D', 'R'};
  }
  else
  {
    if (next_cell.first > 0 && value != 'D')
    {
      children.push_back('U');
    }
    if (next_cell.first < rows - 1 && value != 'U')
    {
      children.push_back('D');
    }
    if (next_cell.second > 0 && value != 'R')
    {
      children.push_back('L');
    }
    if (next_cell.second < cols - 1 && value != 'L')
    {
      children.push_back('R');
    }
  }
}


std::pair<int, int> Tree_item::get_next_cell()
{
  std::pair<int, int> next_cell;
  switch(this->value)
  {
    case 'U':
      next_cell = {this->position.first - 1, this->position.second};
      return next_cell;
    case 'D':
      next_cell = {this->position.first + 1, this->position.second};
      return next_cell;
    case 'R':
      next_cell = {this->position.first, this->position.second + 1};
      return next_cell;
    case 'L':
      next_cell = {this->position.first, this->position.second - 1};
      return next_cell;
    case 'T':
      next_cell = {0, 0};
      return next_cell;
  }
  return next_cell;
}