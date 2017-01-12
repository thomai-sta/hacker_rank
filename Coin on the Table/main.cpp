#include <iostream>
#include "treeItem.hpp"
#include <deque>


void find_and_move_to_front(char item, TreeItem &node)
{
  /// If item exists in vector, move it to the front
  if (std::find(node.next_cell_vals.begin(), node.next_cell_vals.end(), item) != node.next_cell_vals.end())
  {
    node.next_cell_vals.erase(std::remove(node.next_cell_vals.begin(),
                              node.next_cell_vals.end(), item),
                              node.next_cell_vals.end());
    std::vector<char>::iterator it = node.next_cell_vals.begin();
    node.next_cell_vals.insert(it, item);
    node.contains_board_move = true;
  }
}

std::pair<int, int> get_next_cell(TreeItem curr_node)
{
  std::pair<int, int> next_cell;
  if (curr_node.curr_val == 'T')
  {
    next_cell.first = 0;
    next_cell.second = 0;
    return next_cell;
  }
  switch(curr_node.curr_val)
  {
    case 'U' : { next_cell.first = curr_node.curr_cell.first - 1;
                 next_cell.second = curr_node.curr_cell.second;
               }
    case 'L' : { next_cell.first = curr_node.curr_cell.first;
                 next_cell.second = curr_node.curr_cell.second - 1;
               }
    case 'D' : { next_cell.first = curr_node.curr_cell.first + 1;
                 next_cell.second = curr_node.curr_cell.second;
               }
    case 'R' : { next_cell.first = curr_node.curr_cell.first;
                 next_cell.second = curr_node.curr_cell.second + 1;
               }
  }
  return next_cell;
}


TreeItem make_next_node(TreeItem curr_node, int &ID, int N, int M, char **board)
{
  std::pair<int, int> new_cell = get_next_cell(curr_node);
  TreeItem new_node(curr_node.ID, ID++, curr_node.depth + 1,
                    curr_node.next_cell_vals[0], new_cell, N, M,
                    curr_node.operations);
  if (!curr_node.contains_board_move)
  {
    new_node.operations++;
  }
  else
  {
    curr_node.contains_board_move = false;
  }
  curr_node.next_cell_vals.erase(curr_node.next_cell_vals.begin());

  /// Update new_node, regarding next values
  std::pair<int, int> new_next_cell = get_next_cell(new_node);
  find_and_move_to_front(board[new_next_cell.first][new_next_cell.second],
                         new_node);

  return new_node;
}

int main(int argc, char **argv)
{

  int N, M, K;

  std::cin >> N >> M >> K;

  char **board;
  board = new char*[N];
  for (int i = 0; i < N; i++)
  {
    board[i] = new char[M];
    for (int j = 0; j < M; j++)
    {
      std::cin >> board[i][j];
    }
  }

  /// Implement DFS by always following the path with fewer operations:
  bool finished = false;

  std::vector<TreeItem> tree;
  std::deque<std::pair<int, int> > path;

  /// Create root node =========================================================
  std::pair<int, int> curr_cell (-1, -1);
  int ID = 0;  // Nodes' IDs follow the tree-vector's index
  TreeItem curr_node(-1, ID++, -1, 'T', curr_cell, N, M, 0);
  tree.push_back(curr_node);
  path.push_front(curr_cell);
  std::pair<int, int> next_cell = get_next_cell(curr_node);
  find_and_move_to_front(board[next_cell.first][next_cell.second],
                         curr_node);

  /// Implement DFS
  while (!finished)
  {
    if (curr_node.depth <= K && curr_node.curr_val == '*')
    {
      /// Check current node
      std::cout << curr_node.operations << std::endl;
      finished = true;
      return 0;
    }
    else if (curr_node.next_cell_vals.empty() ||
          std::find(path.begin(), path.end(), curr_node.curr_cell) != path.end()
          || curr_node.depth == K)
    {
      /// No more possible children for this node.
      ///                       OR
      /// Curr_node exists in path. We have reached a loop.
      ///                       OR
      /// Reached maximum number of steps.
      /// ===================================================
      /// Remove curr_node and go to its sibling
      path.pop_front();
      if (path.size() == 0)
      {
        std::cout << -1 << std::endl;
        finished = true;
        return 0;
      }
      curr_node = tree[curr_node.parentID];
    }
    else
    {
      /// There are possible children for this node.
      ///                       AND
      /// Curr_node does not exist in path.
      ///                       AND
      /// There are more steps to make.
      /// ================================================
      /// Proceed to next node. Add it to path and tree.
      /// Check it in the next loop.
      TreeItem new_node = make_next_node(curr_node, ID, N, M, board);
      path.push_front(new_node.curr_cell);
      tree.push_back(new_node);
    }
  }

  return 0;
}

