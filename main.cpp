#include <iostream>
#include <queue>
#include <stack>
#define maxm 1005       // max # of rows of the board
#define maxn 1005       // max # of columns of the board
#define manb 2147483647 // max # of battery
#define maxl 1000005    // max # of level
#define maxc 1000005    // max # of cells

using namespace std;

// enum of type
enum
{
  OBSTACLE = 1,
  FREE = 0,
  CHARGER = 2
};
const int dx[4] = {1, 0, -1, 0}; // right, down, left, up
const int dy[4] = {0, -1, 0, 1};

struct Cell
{
  // id of cell = i * n + j;
  short row, col;
  short type;
  int level;
  // # of paths from the high level can go into the cell
  unsigned long long in;
  // # of paths under the cell which it con go out to visit
  unsigned long long out;
  bool visited;
} cells[maxm][maxn];

/*
  Be used to sort cells by level (using counting sort).
*/
int level_cnt[maxl] = {0};
/*
  Sort by level ASC using counting sort.
*/
Cell *sorted_ptrs[maxc] = {nullptr};
int total_free_cells;

int m, n, b;

void cal_basic_info(Cell *);
void update_in(Cell *);
int find_path(Cell *, int, deque<Cell *> &, int &);
void find_paths();
void show_all();

int main()
{
  cin >> m >> n >> b;

  // Add wall around the cells
  m += 2;
  n += 2;

  // Initialize cells
  char tmp;
  Cell *start;
  for (int i = 0; i < m; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      cells[i][j].row = i;
      cells[i][j].col = j;
      cells[i][j].level = maxl;
      cells[i][j].in = 0;
      cells[i][j].out = 0;
      cells[i][j].visited = false;

      // Setup type of cell and get start cell
      if (i == 0 || j == 0 || i == m - 1 || j == n - 1)
        // Add wall around the cells
        cells[i][j].type = OBSTACLE;
      else
      {
        cin >> tmp;
        if (tmp == '1')
          cells[i][j].type = OBSTACLE;
        else if (tmp == '0')
          cells[i][j].type = FREE;
        else if (tmp == 'R')
        {
          cells[i][j].type = CHARGER;
          start = &(cells[i][j]);
        }
        else
          cout << "[ERROR] unknown type of cell" << '\n';
      }
    }
  }

  // Calculate level, in, out of cells
  cal_basic_info(start);

  show_all();

  find_paths();
  // TODO: substract back index of the walls

  return 0;
}

void cal_basic_info(Cell *start)
{
  queue<Cell *> q;
  Cell *cur;
  Cell *next;

  // init
  q.push(start);
  start->level = 0;
  start->out = 1;

  // BFS: update cell.level & cell.in (path can go throught the cell)
  while (!q.empty())
  {
    cur = q.front();
    q.pop();

    for (int i = 0; i < 4; ++i)
    {
      next = &(cells[cur->row + dx[i]][cur->col + dy[i]]);

      if (next->type == OBSTACLE)
        continue;
      if (cur->level > next->level)
        continue;

      else if (cur->level + 1 < next->level)
      {
        if (next->level != maxl)
          // Prevent count one cell twice
          level_cnt[next->level] -= 1;

        next->level = cur->level + 1;

        // Increment level counter
        level_cnt[next->level] += 1;

        // next->out = (next->out == 0) ? cur->out : next->out + 1;
        next->out += cur->out;
        q.push(next);
      }
      else if (cur->level + 1 == next->level)
        // next->out += 1;
        next->out += cur->out;
      else if (cur->level == next->level)
        // next->out += 1;
        next->out += cur->out;
    }
  }

  // DFS: update cell.out (path the cell can go to)
  // FIXME: very slow & cause out of time limit!!!
  update_in(start);

  // Sort cells by level
  // TODO: can count total nubmer of free cells when input
  int sum = 0; // also equal total number of free cells
  int tmp = 0;
  int idx;
  for (int i = 0; i < m * n; ++i)
  {
    tmp = level_cnt[i];
    level_cnt[i] = sum;
    sum += tmp;
  }
  total_free_cells = sum;
  for (int i = 0; i < m; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      if (cells[i][j].type == OBSTACLE)
        continue;
      idx = level_cnt[cells[i][j].level];
      sorted_ptrs[idx] = &(cells[i][j]);
      level_cnt[cells[i][j].level] += 1;
    }
  }
}

// Calculate # of path the cell can go into
void update_in(Cell *start)
{
  stack<Cell *> s;
  Cell *cur;
  Cell *next = nullptr;

  // init
  s.push(start);

  while (!s.empty())
  {
    cur = s.top();

    bool haveNext = false;
    for (int i = 0; i < 4; ++i)
    {
      next = &(cells[cur->row + dx[i]][cur->col + dy[i]]);

      if (next->type == OBSTACLE)
        continue;
      if (cur->level > next->level)
        continue;

      if (next->in != 0)
        cur->in += next->in;
      else
      {
        s.push(next);
        haveNext = true;
        break;
      }
    }

    if (!haveNext)
    {
      cur->in = 1;
      s.pop();
    }
  }
}

int find_path(Cell *cur, int battery, deque<Cell *> &dq, int &duplicated_cnt)
{
  // 1. Add current cell
  dq.push_back(cur);
  cur->visited = true;

  if (battery == 0)
    return 0;

  // 2. Find next cell
  Cell *next = nullptr;
  Cell *cand = nullptr;
  for (int i = 0; i < 4; ++i)
  {
    cand = &(cells[cur->row + dx[i]][cur->col + dy[i]]);

    if (cand->type == OBSTACLE)
      continue;
    // no enough battery
    if (cand->level > battery)
      continue;

    // FIXME: may need to do back tracing
    // don't go deeper level
    if (cand->level > cur->level)
      continue;

    // Pick the first valid next
    if (!next)
      next = cand;
    // Prefer non-visited
    else if (!cand->visited && next->visited)
      next = cand;
    else if (!cand->visited && !next->visited)
    {
      // FIXME: Will this happen? Won't it be updated by BFS?
      if (cand->level > next->level)
        // prefer high level cell
        next = cand;
      else if (cand->level == next->level)
      {
        if (cand->in < next->in)
          // prefer hard to visit cell
          next = cand;
        // FIXME: if 'in' is the same, does which path to visit first matter?
        // else if (cand->in == next->in)
        // prefer less further path cell
        // if (cand->out < next->out)
        // next = cand;
      }
    }
    else if (cand->visited && next->visited)
    {
      // FIXME: Will this happen? Won't it be updated by BFS?
      if (cand->level < next->level)
        // prefer use less battery
        next = cand;
      else if (cand->level == next->level)
        if (cand->out > next->out)
          // prefer more potential paths
          next = cand;
    }
  }

  if (next && next->visited)
    duplicated_cnt += 1;

  if (next)
    return find_path(next, battery - 1, dq, duplicated_cnt) + 1;
  return 0;
}

void find_paths()
{
  Cell *start, *tmp;
  int battery, used_battery;
  int battery_usage = 0;
  deque<Cell *> come, back;

  // debug
  int path_idx = 0;
  int duplicated_cnt = 0;
  for (int i = total_free_cells - 1; i >= 0; --i)
  {
    start = sorted_ptrs[i];
    if (start->visited)
      continue;

    cout << path_idx++ << "th path: " << '\n';

    // Find path from R to the start cell
    battery = (b + 1) / 2;
    used_battery = find_path(start, battery, come, duplicated_cnt);
    battery_usage += used_battery;
    cout << "come battery usage: " << used_battery << '\n';
    // Print path
    cout << "come: " << '\n';
    while (!come.empty())
    {
      tmp = come.back();
      come.pop_back();
      cout << tmp->row << ", " << tmp->col << '\n';
    }

    // Find path from start back to R
    battery = b - used_battery;
    used_battery = find_path(start, battery, back, duplicated_cnt);
    battery_usage += used_battery;
    cout << "back battery usage: " << used_battery << '\n';
    // pop duplicated start cell
    back.pop_front();
    // Print path
    cout << "back: " << '\n';
    while (!back.empty())
    {
      tmp = back.front();
      back.pop_front();
      cout << tmp->row << ", " << tmp->col << '\n';
    }
  }
  cout << "# of path: " << path_idx << '\n';
  cout << "total battery usage: " << battery_usage << '\n';
  // Duplicated cell cnt is slight inaccurate, since come and back path count R twice times
  // However, the inaccurate doesn't affect the measurement, so haven't fix it.
  cout << "duplicated cell cnt: " << duplicated_cnt << '\n';
}

void show_all()
{
  // show level by map
  cout << "Level:\n";
  for (int i = 1; i < m - 1; ++i)
  {
    for (int j = 1; j < n - 1; ++j)
    {
      if (cells[i][j].type == OBSTACLE)
        cout << ' ' << '\t';
      else if (cells[i][j].level == maxl)
        cout << 'x' << '\t';
      else
        cout << cells[i][j].level << '\t';
    }
    cout << '\n';
  }

  cout << "In:\n";
  for (int i = 1; i < m - 1; ++i)
  {
    for (int j = 1; j < n - 1; ++j)
    {
      if (cells[i][j].type == OBSTACLE)
        cout << ' ' << '\t';
      else
        cout << cells[i][j].in << '\t';
    }
    cout << '\n';
  }
  cout << "Out:\n";
  for (int i = 1; i < m - 1; ++i)
  {
    for (int j = 1; j < n - 1; ++j)
    {
      if (cells[i][j].type == OBSTACLE)
        cout << ' ' << '\t';
      else
        cout << cells[i][j].out << '\t';
    }
    cout << '\n';
  }

  cout << "Sorted cells:\n";
  for (int i = 0; sorted_ptrs[i] != nullptr; ++i)
  {
    cout << sorted_ptrs[i]->row << ',' << sorted_ptrs[i]->col << ": " << sorted_ptrs[i]->level << '\n';
  }
}