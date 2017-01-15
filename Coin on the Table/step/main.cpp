#include <iostream>
#include <deque>
#include <algorithm>

#include "tree_item.hpp"

char **board;
std::deque<Tree_item> tree;
int rows, cols, max_operations, operations = -1;
bool found = false;

void make_next_node()
{
  int new_op = tree.front().operations;
  std::pair<int, int> next_cell = tree.front().get_next_cell();

  /// Get new node's value
  std::vector<bool>::iterator it;

  char new_val = tree.front().children.front();
  tree.front().children.pop_front();

  if (board[next_cell.first][next_cell.second] != new_val)
  {
    new_op++;
    if (new_op > max_operations ||
       (new_op > operations && found))
    {
      if (tree.front().children.empty())
      {
        /// All node's children visited, remove from tree
        tree.pop_front();
      }
      return;
    }
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
    Tree_item new_node(next_cell, new_val, rows, cols, tree.front().depth + 1,
                       new_op, tree.front().private_path);

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

  /// Implement BFS for tree to get all possible paths =========================
  /// Make ROOT node
  std::pair<int, int> curr_cell = {-1, -1};
  std::vector<std::pair<int, int> > parent_path;
  parent_path.push_back(curr_cell);
  Tree_item root_node(curr_cell, 'T', rows, cols, -1, 0, parent_path);
  tree.push_back(root_node);


  for (int d = -1; d < max_steps; d++)
  {
    while(tree.front().depth == d)
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

      while (tree.front().checked)
      {
        make_next_node();
      }
    }
  }

  std::cout << operations << std::endl;
  return 0;
}