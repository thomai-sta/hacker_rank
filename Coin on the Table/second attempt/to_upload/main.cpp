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
  bool board_child_checked = false;

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
int rows, cols;

void make_next_node()
{
  int new_op = tree.front().operations;
  std::pair<int, int> next_cell = tree.front().get_next_cell();

  if (board[next_cell.first][next_cell.second] == '*'
      && !tree.front().board_child_checked)
  {
    Tree_item new_node(next_cell, '*', rows, cols, tree.front().depth + 1,
                       new_op, tree.front().private_path);

    /// All node's children visited, remove from path!
    tree.pop_front();
    tree.push_back(new_node);

    return;
  }

  /// Get new node's value
  std::vector<bool>::iterator it;

  char new_val = tree.front().children.front();
  tree.front().children.pop_front();

  if (board[next_cell.first][next_cell.second] != new_val)
  {
    new_op++;
  }
  else
  {
    tree.front().board_child_checked = true;
  }

  /// Check if new_node makes loop
  bool loop = false;
  for (size_t i = 0; i < tree.front().private_path.size(); i++)
  {
    if (tree.front().private_path[i] == next_cell)
    {
      loop = true;
      break;
    }
  }

  if (!loop)
  {
    Tree_item new_node(next_cell, new_val, rows,
                       cols, tree.front().depth + 1, new_op, tree.front().private_path);

    tree.push_back(new_node);
  }

  if (tree.front().children.empty())
  {
    /// All node's children visited, remove from tree
    tree.pop_front();
  }
  return;
}


int main(int argc, char **argv)
{
  /// Read in data
  int max_steps;

  std::cin >> rows >> cols >> max_steps;

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

  int max_operations = target.first + target.second;  // max_operations is the minimum number of steps needed

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

  /// Implement BFS for tree to get all possible paths =========================
  // std::deque<Tree_item> tree;
  /// Make ROOT node
  std::pair<int, int> curr_cell = {-1, -1};
  std::vector<std::pair<int, int> > parent_path;
  parent_path.push_back(curr_cell);
  Tree_item root_node(curr_cell, 'T', rows, cols, -1, 0, parent_path);
  tree.push_back(root_node);

  bool finished = false, found = false;
  int operations = -1;

  while(!finished)
  {
    /// Check if we reached the goal
    if (tree.front().value == '*' && tree.front().depth <= max_steps)
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
    }

    if (tree.front().depth == max_steps ||
        tree.front().value == '*' ||
        tree.front().operations > max_operations ||
        (tree.front().operations > operations && found))
    {
      /// Remove node and use next valid
      tree.pop_front();
    }

    while(!tree.empty() && tree.front().depth == max_steps)
    {
      tree.pop_front();
    }
    if (tree.empty())
    {
      std::cout << operations << std::endl;
      return 0;
    }

    /// Make next node
    make_next_node();
  }
  return 0;
}