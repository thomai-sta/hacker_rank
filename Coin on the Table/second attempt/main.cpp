#include <iostream>
#include <deque>
#include <algorithm>

#include "tree_item.hpp"

std::pair<int, int> get_next_cell(std::pair<int, int> curr_cell, char move)
{
  std::pair<int, int> next_cell;

  switch(move)
  {
    case 'U':
      next_cell = {curr_cell.first - 1, curr_cell.second};
      return next_cell;
      break;
    case 'D':
      next_cell = {curr_cell.first + 1, curr_cell.second};
      return next_cell;
      break;
    case 'R':
      next_cell = {curr_cell.first, curr_cell.second + 1};
      return next_cell;
      break;
    case 'L':
      next_cell = {curr_cell.first, curr_cell.second - 1};
      return next_cell;
      break;
    case 'T':
      next_cell = {0, 0};
      return next_cell;
      break;
  }
  return next_cell;
}

Tree_item make_next_node(Tree_item &curr_node, char **board, int rows, int cols,
 std::vector<Tree_item> &tree, std::deque<int> &path)
{
  std::pair<int, int> next_cell = get_next_cell(curr_node.position,
                                                curr_node.value);

  int new_op = curr_node.operations;
  if (board[next_cell.first][next_cell.second] == '*')
  {
    Tree_item new_node(tree.size(), curr_node.ID, next_cell, '*', board, rows,
                       cols, curr_node.depth + 1, new_op);
    /// Set all new node's children as visited
    for (size_t i = 0; i < new_node.children_visited.size(); i++)
    {
      new_node.children_visited[i] = true;
    }

    /// Set all current node's children as visited
    for (size_t i = 0; i < curr_node.children_visited.size(); i++)
    {
      curr_node.children_visited[i] = true;
    }

    tree[curr_node.ID] = curr_node;

    path.push_back(new_node.ID);
    tree.push_back(new_node);

    return new_node;
  }

  /// Get new node's value
  std::vector<bool>::iterator it;

  it = std::find(curr_node.children_visited.begin(),
                 curr_node.children_visited.end(), false);
  char new_val;
  if (it != curr_node.children_visited.end())
  {
    int index = std::distance(curr_node.children_visited.begin(), it);
    new_val = curr_node.children_vals[index];
    curr_node.children_visited[index] = true;
  }
  tree[curr_node.ID] = curr_node;

  if (board[next_cell.first][next_cell.second] != new_val)
  {
    new_op++;
  }

  Tree_item new_node(tree.size(), curr_node.ID, next_cell, new_val, board, rows,
                     cols, curr_node.depth + 1, new_op);

  path.push_back(new_node.ID);
  tree.push_back(new_node);
  return new_node;
}


int main(int argc, char **argv)
{
  /// Read in data
  int rows, cols, max_steps;

  std::cin >> rows >> cols >> max_steps;

  std::cout << rows << " " << cols << " " << max_steps << std::endl;

  char **board;
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
  }

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      std::cout << board[i][j] << " ";
    }
    std::cout << std::endl;
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

  /// Implement BFS for tree to get all possible paths
  std::vector<Tree_item> tree;
  std::deque<int> path;
  /// Make ROOT node
  std::pair<int, int> curr_cell = {-1, -1};
  Tree_item curr_node(0, -1, curr_cell, 'T', board, rows, cols, -1, 0);
  tree.push_back(curr_node);
  path.push_back(curr_node.ID);

  bool finished = false, found = false;
  int operations = -1;


  while(!finished)
  {
    /// Check if we reached the goal
    if (curr_node.value == '*' && curr_node.depth <= max_steps)
    {
      if(!found)
      {
        found = true;
        operations = curr_node.operations;
      }
      else if (curr_node.operations < operations)
      {
        operations = curr_node.operations;
      }
    }

    /// Update path
    if (curr_node.depth >= max_steps ||
        curr_node.value == '*')
    {
      /// Remove node and use next valid
      path.pop_front();

      while(find(tree[path.front()].children_visited.begin(),
                 tree[path.front()].children_visited.end(), false) ==
             tree[path.front()].children_visited.end()
             && !tree.empty())
      {
        // First node of path is "use up". Remove from path and try next
        path.pop_front();
      }
      if (tree.empty())
      {
        finished = true;
        break;
      }
      curr_node = tree[path.front()];
    }

    /// Make next node
    curr_node = make_next_node(curr_node, board, rows, cols, tree, path);

  }

  std::cout << operations << std::endl;
  return 0;
}