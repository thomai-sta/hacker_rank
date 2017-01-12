#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
#include <cstdlib>


/// CLASS ======================================================================
class TreeItem
{
public:
  std::pair<int, int> curr_cell;
  char curr_val, parent_val;
  int ID, parentID, depth, operations;
  std::vector<char> possible_children = {'U', 'L', 'D', 'R'};
  bool contains_board_move = false, visited = false;

  TreeItem(int parentID, int ID, int depth, char curr_val,
           std::pair<int, int> curr_cell, int N, int M, int operations);
  std::pair<int, int> get_next_cell();
};

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
  // if ((curr_val == 'U') || (next_cell.first == N - 1))
  // {
  //   possible_children.erase(std::remove(possible_children.begin(),
  //                                    possible_children.end(),
  //                                    'D'), possible_children.end());
  // }
  // if ((curr_val == 'L') || (next_cell.second == M - 1))
  // {
  //   possible_children.erase(std::remove(possible_children.begin(),
  //                                    possible_children.end(),
  //                                    'R'), possible_children.end());
  // }
  // if ((curr_val == 'D') || (next_cell.first == 0))
  // {
  //   possible_children.erase(std::remove(possible_children.begin(),
  //                                    possible_children.end(),
  //                                    'U'), possible_children.end());
  // }
  // if ((curr_val == 'R') || (next_cell.second == 0))
  // {
  //   possible_children.erase(std::remove(possible_children.begin(),
  //                                    possible_children.end(),
  //                                    'L'), possible_children.end());
  // }
  if ((next_cell.first == N - 1))
  {
    possible_children.erase(std::remove(possible_children.begin(),
                                     possible_children.end(),
                                     'D'), possible_children.end());
  }
  if ((next_cell.second == M - 1))
  {
    possible_children.erase(std::remove(possible_children.begin(),
                                     possible_children.end(),
                                     'R'), possible_children.end());
  }
  if ((next_cell.first == 0))
  {
    possible_children.erase(std::remove(possible_children.begin(),
                                     possible_children.end(),
                                     'U'), possible_children.end());
  }
  if ((next_cell.second == 0))
  {
    possible_children.erase(std::remove(possible_children.begin(),
                                     possible_children.end(),
                                     'L'), possible_children.end());
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

/// OTHER FUNCTIONS ============================================================
void find_and_move_to_front(char item, TreeItem &node)
{
  /// If item exists in vector, move it to the front
  if (std::find(node.possible_children.begin(), node.possible_children.end(), item) != node.possible_children.end()
      || item == '*')
  {
    node.possible_children.erase(std::remove(node.possible_children.begin(),
                              node.possible_children.end(), item),
                              node.possible_children.end());
    node.possible_children.insert(node.possible_children.begin(), item);
    node.contains_board_move = true;
  }
}

TreeItem make_next_node(TreeItem &curr_node, int ID, int N, int M, char **board)
{
  std::pair<int, int> new_cell = curr_node.get_next_cell();

  TreeItem new_node(curr_node.ID, ID, curr_node.depth + 1,
                    curr_node.possible_children[0], new_cell, N, M,
                    curr_node.operations);
  if (!curr_node.contains_board_move)
  {
    new_node.operations++;
  }
  else
  {
    curr_node.contains_board_move = false;
  }
  curr_node.possible_children.erase(curr_node.possible_children.begin());

  /// Update new_node, regarding next values
  std::pair<int, int> new_next_cell = new_node.get_next_cell();
  find_and_move_to_front(board[new_next_cell.first][new_next_cell.second],
                         new_node);

  return new_node;
}

bool is_duplicate(std::vector<TreeItem> tree, TreeItem node)
{
  for (size_t i = 0; i < tree.size(); i++)
  {
    if (tree[i].curr_cell == node.curr_cell && tree[i].curr_val == node.curr_val && tree[i].visited)
    {
      return true;
    }
  }
  return false;
}


int main(int argc, char **argv)
{
  int N, M, K;

  std::cin >> N >> M >> K;

  std::cout << N << " " << M << " " << K << std::endl;

  char **board;
  board = new char*[N];
  std::pair<int, int> target;
  for (int i = 0; i < N; i++)
  {
    board[i] = new char[M];
    for (int j = 0; j < M; j++)
    {
      std::cin >> board[i][j];
      if (board[i][j] == '*')
      {
        target.first = i;
        target.second = j;
      }
    }
  }

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < M; j++)
    {
      std::cout << board[i][j] << " ";
    }
    std::cout << std::endl;
  }

  /// Implement DFS by always following the path with fewer operations:
  bool finished = false;

  std::vector<TreeItem> tree;
  std::deque<std::pair<int, int> > path;

  /// Create root node =========================================================
  std::pair<int, int> curr_cell (-1, -1);
  int ID = 0;  // Nodes' IDs follow the tree-vector's index
  TreeItem curr_node(-1, ID, -1, 'T', curr_cell, N, M, 0);
  std::pair<int, int> next_cell = curr_node.get_next_cell();
  curr_node.possible_children = {'D', 'R'};
  find_and_move_to_front(board[next_cell.first][next_cell.second],
                         curr_node);
  tree.push_back(curr_node);
  path.push_front(curr_cell);

  int operations = 0, max_operations = target.first + target.second;
  if (max_operations > K)
  {
    std::cout << -1 << std::endl;
    return 0;
  }
  bool found = false;
  int f = 0;

  /// Implement DFS
  while (!finished)
  {
    // std::cout << path.size() << std::endl;
    // std::cout << "Current cell: (" << curr_node.curr_cell.first << ", " << curr_node.curr_cell.second << ")" << std::endl;
    // std::cout << "Current val: " << curr_node.curr_val << std::endl;
    // std::cout << "Current depth: " << curr_node.depth << std::endl;
    // std::cout << "Current operations: " << curr_node.operations << std::endl;
    // std::cout << "Current ID: " << curr_node.ID << std::endl;
    // for (size_t i = 0; i < curr_node.possible_children.size(); i++)
    // {
    //   std::cout << curr_node.possible_children[i] << " ";
    // }
    // std::cout << std::endl;

    if (curr_node.curr_val == '*' && curr_node.depth <= K)
    {
      std::cout << "FOUND " << f++  << " " << curr_node.operations << std::endl;
      /// Found goal
      if (!found || curr_node.operations < operations)
      {
        /// 1st time we find it
        operations = curr_node.operations;
      }
      found = true;
      if (operations == 0)
      {
        std::cout << 0 << std::endl;
        return 0;
      }
    }

    /// Proceed with tree
    if (curr_node.possible_children.empty()  // No more children available.
        || curr_node.depth >= K  // No more possible steps.
        || curr_node.curr_val == '*'  // Found target.
        || (curr_node.operations > max_operations && curr_node.curr_cell.first > target.first && curr_node.curr_cell.second > target.second)
        || std::find(path.begin() + 1, path.end(), curr_node.curr_cell) != path.end())  // already visited in this path
    {

      // std::cout << "here" << std::endl;
      /// Go back and try another way
      path.pop_front();
      if (path.size() == 0)
      {
        // Path is empty. Entire tree travesed. Exit loop.
        finished = true;
        break;
      }
      curr_node = tree[curr_node.parentID];

      while (curr_node.possible_children.empty())
      {
        curr_node.visited = true;
        path.pop_front();
        if (path.size() == 0)
        {
          // Path is empty. Entire tree travesed. Exit loop.
          finished = true;
          break;
        }
        curr_node = tree[curr_node.parentID];
      }
      if (path.size() == 0)
      {
        // Path is empty. Entire tree travesed. Exit loop.
        finished = true;
        break;
      }

      /// Found a node that has possible new children.
      TreeItem new_node = make_next_node(curr_node, ID, N, M, board);
      tree[curr_node.ID] = curr_node;  // Update current node in tree
      while(is_duplicate(tree, new_node))
      {
        /// Go back and try another way
        path.pop_front();
        if (path.size() == 0)
        {
          // Path is empty. Entire tree travesed. Exit loop.
          finished = true;
          break;
        }
        curr_node = tree[curr_node.parentID];

        while (curr_node.possible_children.empty())
        {
          curr_node.visited = true;
          path.pop_front();
          if (path.size() == 0)
          {
            // Path is empty. Entire tree travesed. Exit loop.
            finished = true;
            break;
          }
          curr_node = tree[curr_node.parentID];
        }
        if (path.size() == 0)
        {
          // Path is empty. Entire tree travesed. Exit loop.
          finished = true;
          break;
        }
        /// Found a node that has possible new children.
        TreeItem new_node = make_next_node(curr_node, ID, N, M, board);
        tree[curr_node.ID] = curr_node;  // Update current node in tree
      }

      path.push_front(new_node.curr_cell);
      tree.push_back(new_node);
      ID++;
      curr_node = new_node;
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
      tree[curr_node.ID] = curr_node;  // Update current node in tree
      while(is_duplicate(tree, new_node))
      {
        /// Go back and try another way
        path.pop_front();
        if (path.size() == 0)
        {
          // Path is empty. Entire tree travesed. Exit loop.
          finished = true;
          break;
        }
        curr_node = tree[curr_node.parentID];

        while (curr_node.possible_children.empty())
        {
          curr_node.visited = true;
          path.pop_front();
          if (path.size() == 0)
          {
            // Path is empty. Entire tree travesed. Exit loop.
            finished = true;
            break;
          }
          curr_node = tree[curr_node.parentID];
        }
        if (path.size() == 0)
        {
          // Path is empty. Entire tree travesed. Exit loop.
          finished = true;
          break;
        }
        /// Found a node that has possible new children.
        TreeItem new_node = make_next_node(curr_node, ID, N, M, board);
        tree[curr_node.ID] = curr_node;  // Update current node in tree
      }

      path.push_front(new_node.curr_cell);
      tree.push_back(new_node);
      ID++;
      curr_node = new_node;
    }
  }
  if (found)
  {
    std::cout << operations << std::endl;
  }
  else
  {
    std::cout << -1 << std::endl;
  }
  return 0;
}

