#include <vector>

class Tree_item
{
public:
  int ID, parent_ID, depth, operations = 0;
  std::vector<char> children_vals;
  std::vector<bool> children_visited;
  std::pair<int, int> position;
  char value;
  char **current_board;

  Tree_item(int ID, int parent_ID, std::pair<int, int> position, char value,
            char **current_board, int rows, int cols, int depth,
            int operations);
};