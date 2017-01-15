#include <iostream>
#include <deque>
#include <algorithm>
#include <vector>

class Tree_item
{
public:
  int depth, operations = 0;
  std::vector<std::pair<int, int> > private_path;
  std::deque<char> children;
  std::pair<int, int> position;
  char value;
  bool checked = false;

  Tree_item(std::pair<int, int> position, char value, int rows, int cols,
            int depth, int operations,
            std::vector<std::pair<int, int> > parents_path);
  std::pair<int, int> get_next_cell();
};

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


char **board;
std::deque<Tree_item> tree;
int rows, cols, max_operations, operations = -1, max_steps;
bool found = false;

void make_next_node()
{
  int new_op = tree.front().operations;
  std::pair<int, int> next_cell = tree.front().get_next_cell();

  /// Get new node's value
  char new_val = tree.front().children.front();
  tree.front().children.pop_front();

  if (board[next_cell.first][next_cell.second] != new_val)
  {
    new_op++;
    if (new_op > max_operations ||
       (new_op >= operations && found))
    {
      return;
    }
  }

  /// Check if new_node makes loop
  for (size_t i = 0; i < tree.front().private_path.size(); i++)
  {
    if (tree.front().private_path[i] == next_cell)
    {
      return;
    }
  }

  Tree_item new_node(next_cell, new_val, rows, cols, tree.front().depth + 1,
                     new_op, tree.front().private_path);
  tree.push_front(new_node);

  return;
}


int main(int argc, char **argv)
{
  /// Read in data
  std::cin >> rows >> cols >> max_steps;
  if (rows > max_steps)
  {
    rows = max_steps + 1;
  }
  if (cols > max_steps)
  {
    cols = max_steps + 1;
  }

  board = new char*[rows];
  std::pair<int, int> target;

  for (int i = 0; i < rows; i++)
  {
    board[i] = new char[cols];
    for (int j = 0; j < cols; j++)
    {
      std::cin >> board[i][j];
      if (board[i][j] == '*')
      {
        target.first = i;
        target.second = j;
      }
    }
  }

  max_operations = target.first + target.second;  // max_operations is the minimum number of steps needed

  if (max_operations > max_steps)
  {
    std::cout << -1 << std::endl;
    return 0;
  }

  if (board[0][0] == '*')
  {
    std::cout << 0 << std::endl;
    return 0;
  }
  if (max_steps < 0)
  {
    std::cout << -1 << std::endl;
    return 0;
  }

  /// Implement DFS for tree to get all possible paths =========================
  /// Make ROOT node
  std::pair<int, int> curr_cell = {-1, -1};
  std::vector<std::pair<int, int> > parent_path;
  parent_path.push_back(curr_cell);
  Tree_item root_node(curr_cell, 'T', rows, cols, -1, 0, parent_path);
  tree.push_front(root_node);

  while(!tree.empty())
  {
    if (!tree.front().checked)
    {
      tree.front().checked = true;
      std::pair<int, int> next_cell = tree.front().get_next_cell();
      /// Check if we reached the goal
      if (board[next_cell.first][next_cell.second] == '*')
      {
        if(!found)
        {
          found = true;
          operations = tree.front().operations;
        }
        else if (tree.front().operations < operations)
        {
          operations = tree.front().operations;
        }
        if (operations == 0)
        {
          std::cout << operations << std::endl;
          return 0;
        }
        tree.pop_front();
      }
    }
    /// Check for empty children
    while ((tree.front().children.empty() ||
            tree.front().depth == max_steps - 1) && !tree.empty())
    {
      tree.pop_front();
    }

    if (!tree.empty())
    {
      make_next_node();
    }
  }

  std::cout << operations << std::endl;
  return 0;
}