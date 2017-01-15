#include <vector>
#include <deque>


class Tree_item
{
public:
  int depth, operations = 0;
  std::vector<std::pair<int, int> > private_path;
  std::deque<char> children;
  std::pair<int, int> position;
  char value;
  bool board_child_checked = false, checked = false;

  Tree_item(std::pair<int, int> position, char value, int rows, int cols,
            int depth, int operations,
            std::vector<std::pair<int, int> > parents_path);
  std::pair<int, int> get_next_cell();
};