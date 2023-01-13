#include <bits/stdc++.h>
using namespace std;

// 跨平台终端颜色显示：https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
// ----------------------------------------------------------------------------------------------------
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h> // for displaying colors
#endif // Windows

#define color_black      0
#define color_dark_blue  1
#define color_dark_green 2
#define color_light_blue 3
#define color_dark_red   4
#define color_magenta    5
#define color_orange     6
#define color_light_gray 7
#define color_gray       8
#define color_blue       9
#define color_green     10
#define color_cyan      11
#define color_red       12
#define color_pink      13
#define color_yellow    14
#define color_white     15

string get_textcolor_code(const int textcolor) { // Linux only
    switch(textcolor) {
        case  0: return "30"; // color_black      0
        case  1: return "34"; // color_dark_blue  1
        case  2: return "32"; // color_dark_green 2
        case  3: return "36"; // color_light_blue 3
        case  4: return "31"; // color_dark_red   4
        case  5: return "35"; // color_magenta    5
        case  6: return "33"; // color_orange     6
        case  7: return "37"; // color_light_gray 7
        case  8: return "90"; // color_gray       8
        case  9: return "94"; // color_blue       9
        case 10: return "92"; // color_green     10
        case 11: return "96"; // color_cyan      11
        case 12: return "91"; // color_red       12
        case 13: return "95"; // color_pink      13
        case 14: return "93"; // color_yellow    14
        case 15: return "97"; // color_white     15
        default: return "37";
    }
}
string get_backgroundcolor_code(const int backgroundcolor) { // Linux only
    switch(backgroundcolor) {
        case  0: return  "40"; // color_black      0
        case  1: return  "44"; // color_dark_blue  1
        case  2: return  "42"; // color_dark_green 2
        case  3: return  "46"; // color_light_blue 3
        case  4: return  "41"; // color_dark_red   4
        case  5: return  "45"; // color_magenta    5
        case  6: return  "43"; // color_orange     6
        case  7: return  "47"; // color_light_gray 7
        case  8: return "100"; // color_gray       8
        case  9: return "104"; // color_blue       9
        case 10: return "102"; // color_green     10
        case 11: return "106"; // color_cyan      11
        case 12: return "101"; // color_red       12
        case 13: return "105"; // color_pink      13
        case 14: return "103"; // color_yellow    14
        case 15: return "107"; // color_white     15
        default: return  "40";
    }
}
string get_print_color(const int textcolor) { // Linux only
    return "\033["+get_textcolor_code(textcolor)+"m";
}
string get_print_color(const int textcolor, const int backgroundcolor) { // Linux only
    return "\033["+get_textcolor_code(textcolor)+";"+get_backgroundcolor_code(backgroundcolor)+"m";
}
void print_color(const int textcolor) {
#if defined(_WIN32)
    static const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, textcolor);
#elif defined(__linux__)
    cout << get_print_color(textcolor);
#endif // Windows/Linux
}
void print_color(const int textcolor, const int backgroundcolor) {
#if defined(_WIN32)
    static const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, backgroundcolor<<4|textcolor);
#elif defined(__linux__)
    cout << get_print_color(textcolor, backgroundcolor);
#endif // Windows/Linux
}
void print_color_reset() {
#if defined(_WIN32)
    static const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, 7); // reset color
#elif defined(__linux__)
    cout << "\033[0m"; // reset color
#endif // Windows/Linux
}

void println(const string& s="") {
    cout << s << endl;
}
void print(const string& s="") {
    cout << s;
}
void print(const string& s, const int textcolor) {
    print_color(textcolor);
    cout << s;
    print_color_reset();
}
void print(const string& s, const int textcolor, const int backgroundcolor) {
    print_color(textcolor, backgroundcolor);
    cout << s;
    print_color_reset();
}
void print_no_reset(const string& s, const int textcolor) { // print with color, but don't reset color afterwards (faster)
    print_color(textcolor);
    cout << s;
}
void print_no_reset(const string& s, const int textcolor, const int backgroundcolor) { // print with color, but don't reset color afterwards (faster)
    print_color(textcolor, backgroundcolor);
    cout << s;
}
// ----------------------------------------------------------------------------------------------------


/**
 * @brief 扫雷提示器
 * 1. 输入格式棋盘格式：
 * '#' 代表还没有点击的方块
 * '[0 - 8]' 代表方块四周的雷个数
 * 'M' 表示玩家自己标记的雷(备注：程序会默认这就是一个雷，不会检查正确性)
 * 'X' 表示已经挖出的雷
 * 
 * 2. 输出：
 * 提示玩家哪些空格一定是安全(即可点击的)，输出在棋盘中为 'S'
 * 一定是有雷的，输出在棋盘中为 'M'
 * 或者是暂时还无法判断，需要玩家自行随机点击
 */

// 棋盘状态定义以及相关判断
#define S_UNCLICKED ('#')
#define S_MARKED    ('M')
#define S_MINE      ('X')
#define S_SAFE      ('S') // 非输入字符
#define COUNT(cnt)  ('0' + cnt)
#define GET_COUNT(ch) (ch - '0')
#define IN_DANGER(ch) (ch == S_MARKED || ch == S_MINE) // 雷区 
#define VALID_CH(ch) (isdigit(ch) || ch == S_UNCLICKED || IN_DANGER(ch))

// 相关常量和全局变量
using pii = pair<int,int>;
const int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
constexpr int dSize = 8;
const int vdx[] = {1, 1, 1, -1, -1, -1};
const int vdy[] = {-1, 0, 1, -1, 0, 1};
const int hdx[] = {-1, 0, 1, -1, 0, 1};
const int hdy[] = {-1, -1, -1, 1, 1, 1};
constexpr int aSize = 3; // 减法公式独区域方格个数 
constexpr int search_dep = 5; // dfs 暴力边界搜索深度

int rows = 0, cols = 0;

static bool ValidMove(int x, int y) {
    return 0 <= x && x < rows && 0 <= y && y < cols;
}

static void UpdateStateAround(vector<string> &board, int x, int y, char old_state, char new_state, unordered_set<int> &update_list) {
    int nx, ny;
    for (int k = 0; k < dSize; ++k) {
        nx = x + dx[k];
        ny = y + dy[k];
        if (ValidMove(nx, ny) && board[nx][ny] == old_state) {
            board[nx][ny] = new_state;
            update_list.insert(nx * cols + ny);
        }
    }
}

static void UpdateStateAround(vector<string> &board, int x, int y, char old_state, char new_state) {
    int nx, ny;
    for (int k = 0; k < dSize; ++k) {
        nx = x + dx[k];
        ny = y + dy[k];
        if (ValidMove(nx, ny) && board[nx][ny] == old_state) {
            board[nx][ny] = new_state;
            //cout << "x = " << nx << " , y = " << ny << " is changing from " << old_state << " to " << new_state << endl;
        }
    }
}

// 得到格子四周的格子信息(雷数、未开数、安全数)
static void GetGridInfo(vector<string> &board, int x, int y, int &dangers, int &unknowns) {
    int nx, ny;    
    for (int k = 0; k < dSize; ++k) {
        nx = x + dx[k];
        ny = y + dy[k];
        if (!ValidMove(nx, ny)) {
            continue;
        }
        if (IN_DANGER(board[nx][ny])) {
            ++dangers;
        } else if (board[nx][ny] == S_UNCLICKED) {
            ++unknowns;
        }
    }
}

static void MarkAsSafeOrMine(vector<string> &board, vector<pii> &vt, unordered_set<int> &st, char state) {
    for (auto &p : vt) {
        board[p.first][p.second] = state;
        st.insert(p.first * cols + p.second);
    }
}

/**
 * @brief 运用减法公式再进行填充
 * 参考：https://zhuanlan.zhihu.com/p/27439584
 */
static bool SubtractionFormula(vector<string> &board, pii p1, pii p2, bool &finish, unordered_set<int> &safe_list, unordered_set<int> &mine_list, const int dx_[aSize], const int dy_[aSize]) {
    // 需要统计的变量有：独自区域的雷数、未开个数、安全个数 
    vector<pii> lb, rb, lu, ru;
    int nx, ny;
    // 统计左区域
    for (int k = 0; k < aSize; ++k) {
        nx = p1.first + dx_[k];
        ny = p1.second + dy_[k];
        if (!ValidMove(nx, ny)) {
            continue;
        } else if (IN_DANGER(board[nx][ny])) {
            lb.push_back({nx, ny});
        } else if (board[nx][ny] == S_UNCLICKED) {
            lu.push_back({nx, ny});
        }	
    } 
    // 统计右区域
    for (int k = 0; k < aSize; ++k) {
        nx = p2.first + dx_[aSize + k];
        ny = p2.second + dy_[aSize + k];
        if (!ValidMove(nx, ny)) {
            continue;
        } else if (IN_DANGER(board[nx][ny])) {
            rb.push_back({nx, ny});
        } else if (board[nx][ny] == S_UNCLICKED) {
            ru.push_back({nx, ny});
        }
    } 
    int should = board[p1.first][p1.second] - board[p2.first][p2.second];
    int now = lb.size() - rb.size();
    int delta = should - now;
    if (delta == 0) {
        // 如果一方没有空闲格子，那么另一方全部都应该是安全
        if (lu.size() == 0 && ru.size() > 0) {
            MarkAsSafeOrMine(board, ru, safe_list, S_SAFE);
            finish = false;
        } else if (ru.size() == 0 && lu.size() > 0) {
            MarkAsSafeOrMine(board, lu, safe_list, S_SAFE);
            finish = false;
        }
        // 双方都有空闲格子，暂时无法判断
    } else if (delta > 0) {
        // 左方的空闲格子需要提供 delta 个雷
        if (lu.size() < delta) {
            cout << "warning: invalid board input, line " << __LINE__ << endl;
            cout << "context -> (x = " << p1.first << ", y = " << p1.second << ") , lu.size() = " << lu.size() << " , delta = " << delta << endl;
            finish = true;
            return false;
        } else if (lu.size() == delta) {
            // 左方的空闲格子全部都是雷，右方的空闲格子全部都是安全
            MarkAsSafeOrMine(board, lu, mine_list, S_MARKED);
            MarkAsSafeOrMine(board, ru, safe_list, S_SAFE);
            finish = false;
        }
        // 左方的空闲格子比需要的雷个数多，不确定哪一个是雷
    } else {
        delta = -delta;
        // 右方的空闲格子需要提供 delta 个雷
        if (ru.size() < delta) {
            cout << "warning: invalid board input, line " << __LINE__ << endl;
            cout << "context -> (x = " << p1.first << ", y = " << p2.second << ") , ru.size() = " << ru.size() << " , delta = " << delta << endl;
            finish = true;
            return false;
        } else if (ru.size() == delta) {
            // 右方的空闲格子全部都是雷，左方的空闲格子全部都是安全
            MarkAsSafeOrMine(board, ru, mine_list, S_MARKED);
            MarkAsSafeOrMine(board, lu, safe_list, S_SAFE);
            finish = false;
        }
        // 右方的空闲格子比需要的雷个数多，不确定哪一个是雷
    }
    return true;
}

// 检查 (x, y) 四周是否有数字格
static bool isDigitEdgeGrid(vector<string> &board, int x, int y) {
    int nx, ny;
    for (int i = 0; i < dSize; ++i) {
        nx = x + dx[i], ny = y + dy[i];
        if (ValidMove(nx, ny) && isdigit(board[nx][ny])) {
            return true;
        }
    }
    return false;
}

static void UpdateGridAroundInfo(vector<string> &board, vector<vector<int>> &vt, int x, int y, int op) {
    int nx, ny;
    for (int i = 0; i < dSize; ++i) {
        nx = x + dx[i], ny = y + dy[i];
        if (ValidMove(nx, ny) && isdigit(board[nx][ny])) {
            vt[nx][ny] += op;
        }
    }
}

// 判断一个节点是否合法, cnt 为节点四周应该有的雷数，dangers 为剩余未开雷数，unknowns 为未开盒格子数量
static bool IsValidGrid(int cnt, int dangers, int unknowns) {
    if (dangers < 0 || unknowns < 0 || dangers > cnt || dangers > unknowns) {
        return false;
    }
    return true;
}

// 检查节点四周的全部数字节点是否为有效节点
static bool CheckIfGridAroundIsValid(vector<string> &board, int x, int y, vector<vector<int>> &g_unknowns, vector<vector<int>> &g_dangers) {
    int nx, ny;
    for (int i = 0; i < dSize; ++i) {
        nx = x + dx[i], ny = y + dy[i];
        if (!ValidMove(nx, ny) || !isdigit(board[nx][ny])) {
            continue;
        }
        if (!IsValidGrid(GET_COUNT(board[nx][ny]), g_dangers[nx][ny], g_unknowns[nx][ny])) {
            return false;
        }
    }
    return true;
}

// 在节点四周寻找特定状态的节点
static vector<pii> FindGridAround(vector<string> &board, int x, int y, char state) {
    vector<pii> ans;
    int nx, ny;
    for (int i = 0; i < dSize; ++i) {
        nx = x + dx[i], ny = y + dy[i];
        if (ValidMove(nx, ny) && board[nx][ny] == state) {
            ans.push_back({nx, ny});
        }
    }
    return ans;
} 

// 普通填充算法, (x, y) 必须为数字格
static bool NaiveFill(vector<string> &board, int x, int y, unordered_set<int> &safe_list, unordered_set<int> &mine_list, bool &finish) {
    int nx, ny;
    int cnt = GET_COUNT(board[x][y]), dangers = 0, unknowns = 0;
    GetGridInfo(board, x, y, dangers, unknowns);
    //cout << cnt << " " << dangers << " " << unknowns << endl;
    if (cnt == dangers) {
        // 全部雷都已经扫出来了，剩下的方块都是安全的，标记一下
        UpdateStateAround(board, x, y, S_UNCLICKED, S_SAFE, safe_list);
    } else if (unknowns == cnt - dangers) {
        // 剩下的都是雷，需要标记一下
        finish = false; // 下轮继续迭代更新
        UpdateStateAround(board, x, y, S_UNCLICKED, S_MARKED, mine_list);
    } else if (unknowns == 0) {
        // 错误的棋盘
        cout << "warning: invalid board input, line " << __LINE__ << endl;
        cout << "context -> (x = " << x << ", y = " << y << ") , cnt = " << cnt << " , dangers = " << dangers << " , unknowns = " << unknowns << endl;
        return false;
    }
    // 其它情况：剩余的空格大于雷数，暂时不知道怎么填
    return true;
}

static bool CheckIfValidBoardDfs(vector<string> &board, int x, int y, int search_dep, vector<vector<int>> &g_unknowns, vector<vector<int>> &g_dangers);

// 尝试给一个数字格子四周填雷
// (x, y) 为数字节点
static bool TryFillMineAround(vector<string> &board, int x, int y, int search_dep, vector<vector<int>> &g_unknowns, vector<vector<int>> &g_dangers) {
    //cout << "TryFillMineAround start with x = " << x << " , y = " << y << endl;
    // 填充已经完成
    if (g_dangers[x][y] == 0 && g_unknowns[x][y] == 0) {
        //cout << "filling alreay complete with x = " << x << " , y = " << y << endl;
        return true;
    }
    // 检查当前节点是否有效
    if (!IsValidGrid(GET_COUNT(board[x][y]), g_dangers[x][y], g_unknowns[x][y])) {
       // cout << "invalid gird x = " << x << ", y = " << y << " with dangers = " << g_dangers[x][y] << " , unknowns = " << g_unknowns[x][y] << endl;
        return false; // 无效节点，直接返回填充失败
    }
    // 如果剩余的雷数大于 1 且已经到达暴力搜索深度，可以立刻返回，直接认为排面正确
    if (g_dangers[x][y] > 1 && search_dep == 0) {
        //cout << "search end " << x << ", y = " << y << " with dangers = " << g_dangers[x][y] << " , search_dep = " << search_dep << endl;
        return true;
    }
    // 填雷是否为唯一解，如果是唯一解则本轮不算暴力搜索轮次，否则搜索轮次 -1
    if (g_unknowns[x][y] > g_dangers[x][y]) {
        --search_dep;
    }
    bool ok = true;
    // 如果已经没有剩余的雷了(此时必定有空位置)
    if (g_dangers[x][y] == 0) {
       // cout << "no mines rest with x = " << x << ", y = " << y << " with dangers = " << g_dangers[x][y] << " , unknowns = " << g_unknowns[x][y] << endl;
        // 说明剩下的位置都是安全的
        vector<pii> safe_list = FindGridAround(board, x, y, S_UNCLICKED);
        UpdateStateAround(board, x, y, S_UNCLICKED, S_SAFE);
        for (auto &p : safe_list) {
            UpdateGridAroundInfo(board, g_unknowns, p.first, p.second, -1);
        }
        // 先对所有标记为安全的节点进行进一步的 dfs
        for (auto &p : safe_list) {
            // 有一个安全节点是错误的，因此这个排面都是错的，因为没有剩余的雷了
            ok = CheckIfValidBoardDfs(board, p.first, p.second, search_dep, g_unknowns, g_dangers);
            if (!ok) {
               // cout << "incorrect marking with safe x = " << p.first << ", y = " << p.second << endl;
                break;
            }
        }
        // 回溯
        for (auto &p : safe_list) {
           // cout << "x = " << p.first << " , y = " << p.second << " is changing from " << board[p.first][p.second] << " to " << S_UNCLICKED << endl;
            board[p.first][p.second] = S_UNCLICKED;
            UpdateGridAroundInfo(board, g_unknowns, p.first, p.second, 1);
        }
        // if (ok) {
        //     cout << "fill ok! x = " << x << " , y = " << y << endl;
        // }
        return ok;
    }
    //cout << "try to fill mine around x = " << x << " , y = " << y << endl; 
    // 找出数字节点附近的未开格子，只要找出一种填法是有效的，则该节点就有效
    int nx, ny;
    for (int j = 0; j < dSize; ++j) {
        nx = x + dx[j], ny = y + dy[j];
        if (!ValidMove(nx, ny) || board[nx][ny] != S_UNCLICKED) {
            continue;
        }
        // 设置为雷并更新统计信息
        UpdateGridAroundInfo(board, g_unknowns, nx, ny, -1); 
        UpdateGridAroundInfo(board, g_dangers, nx, ny, -1); 
        board[nx][ny] = S_MINE;
       // cout << "marking x = " << nx << " , y = " << ny << " as mine..." << endl;
        ok = CheckIfValidBoardDfs(board, nx, ny, search_dep - 1, g_unknowns, g_dangers);
        // 回溯
        board[nx][ny] = S_UNCLICKED;
       // cout << "cleaning x = " << nx << " , y = " << ny << " ..." << endl;
        UpdateGridAroundInfo(board, g_unknowns, nx, ny, 1); 
        UpdateGridAroundInfo(board, g_dangers, nx, ny, 1); 
        // 判断是否找到一种有效填法
        if (ok) {
           // cout << "fill ok! x = " << x << " , y = " << y << endl;
            return true;
        }
    }
    return false;
}

// 是暴力搜索查看当前排面的节点是否有效
// (x, y) 为标记为安全节点或者是标记为雷
static bool CheckIfValidBoardDfs(vector<string> &board, int x, int y, int search_dep, vector<vector<int>> &g_unknowns, vector<vector<int>> &g_dangers) {
    //cout << "checkIfValid safe or mine with x = " << x << " , y = " << y << endl;
    // 遍历四周数字节点，枚举它们没有填完的格子进行填充，只要有一个数字节点无法填充则返回 false 说明排面出错
    int nx, ny;
    for (int i = 0; i < dSize; ++i) {
        // (nx, ny) 为数字格子
        nx = x + dx[i], ny = y + dy[i];
        if (!ValidMove(nx, ny) || !isdigit(board[nx][ny])) {
            continue;
        }
        // 该节点怎么也没有办法填充，排面出错，说明 (x, y) 填错了
        if (!TryFillMineAround(board, nx, ny, search_dep, g_unknowns, g_dangers)) {
            //cout << "invalid safe or mine x = " << x << " , y = " << y << endl;
            return false;
        }
    }
    //cout << "valid safe or mine " << x << " " << y << endl;
    return true;
}

// for debug
template<typename T>
static void print_2ray(vector<vector<T>> &vt) {
    int n = vt.size();
    if (n == 0) {
        return;
    }
    int m = vt[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << vt[i][j] << " ";
        }
        cout << endl;
    }
}

// for debug
static void print_board(vector<string> &board) {
    for (int x = 0, idx = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y, ++idx) {
            cout << board[x][y];
        }
        cout << endl;
    }
}

// 暴力深搜
static bool SearchEdgeDigitGrid(vector<string> &board, unordered_set<int> &safe_list, int search_dep) {
    // 分别表示四周未开格子和四周未知格子还需要出多少雷
    vector<vector<int>> g_unknowns(rows, vector<int>(cols)), g_dangers(rows, vector<int>(cols));

    // 统计全图数字格的信息
    int unknowns, dangers;
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            if (isdigit(board[x][y])) {
                unknowns = dangers = 0;
                GetGridInfo(board, x, y, dangers, unknowns);
                g_unknowns[x][y] = unknowns;
                g_dangers[x][y] = GET_COUNT(board[x][y]) - dangers;
            }
        }
    }

    // 针对所有数字边缘的未开启格，尝试填雷
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            if (board[x][y] != S_UNCLICKED || !isDigitEdgeGrid(board, x, y)) {
                continue;
            }
            // 设置为雷并更新统计信息
            UpdateGridAroundInfo(board, g_unknowns, x, y, -1); 
            UpdateGridAroundInfo(board, g_dangers, x, y, -1); 
            board[x][y] = S_MINE;
            //cout << "marking x = " << x << " , y = " << y << " as mine..." << endl;
            bool ok = CheckIfValidBoardDfs(board, x, y, search_dep, g_unknowns, g_dangers);
            // 回溯
            // 说明该格子不能为雷，即肯定是安全节点
            if (!ok) {
                board[x][y] = S_SAFE;
                UpdateGridAroundInfo(board, g_dangers, x, y, 1); 
                safe_list.insert(x * cols + y);
            } else {
                board[x][y] = S_UNCLICKED;
                UpdateGridAroundInfo(board, g_unknowns, x, y, 1); 
                UpdateGridAroundInfo(board, g_dangers, x, y, 1); 
            }
            //cout << "cleaning x = " << x << " , y = " << y << " ..." << endl;
            // debug:
            // cout << "after status:" << endl;
            // print_2ray(g_dangers);
            // cout << endl;
            // print_2ray(g_unknowns);
            // cout << endl;
            // print_board(board);
            // cout << "---------" << endl;
        }
    }
    return true;
}



/**
 * @brief 基本思路：朴素填充 + 减法公式 + 暴力深搜
 * 
 * @param safe_list 
 * @param mine_list 
 * @return true 
 * @return false 
 */
static bool HinterStart(vector<string> &board, unordered_set<int> &safe_list, unordered_set<int> &mine_list, int search_dep_) {
    // 1.普通填充
    bool finish = false;
    int nx, ny;
    while (!finish) {
        finish = true;
        for (int x = 0; x < rows; ++x) {
            for (int y = 0; y < cols; ++y) {
                if (isdigit(board[x][y]) && !NaiveFill(board, x, y, safe_list, mine_list, finish)) {
                    return false;
                }
            }
        }
    }

    // 2. 减法公式 
    // 水平方向 
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols - 1; ++y) {
            if (isdigit(board[x][y]) && isdigit(board[x][y+1])) {
                if (!SubtractionFormula(board, make_pair(x, y), make_pair(x, y+1), finish, safe_list, mine_list, hdx, hdy)) {
                    return false;
                }
            } 
        }
    }
    // 竖直方向
    for (int x = 1; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            if (isdigit(board[x][y]) && isdigit(board[x-1][y])) {
                if (!SubtractionFormula(board, make_pair(x, y), make_pair(x-1, y), finish, safe_list, mine_list, vdx, vdy)) {
                    return false;
                }
            }
        }
    }

    // 进行多轮迭代
    if (!finish) {
    	return HinterStart(board, safe_list, mine_list, search_dep);
	}
    return true;
}

// 输出提示
void OutputResult(vector<string> &board, unordered_set<int> &safe_list, unordered_set<int> &mine_list, unordered_set<int> &dfs_list) {
    for (int x = 0, idx = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y, ++idx) {
            if (safe_list.find(idx) != safe_list.end()) {
                print(string(1, board[x][y]), color_black, color_green);
            } else if (mine_list.find(idx) != mine_list.end()) {
                print(string(1, board[x][y]), color_black, color_red);
            } else if (dfs_list.find(idx) != dfs_list.end()) {
                print(string(1, board[x][y]), color_black, color_yellow);
            } else {
                cout << board[x][y];
            }
        }
        cout << endl;
    }
}

int main() {
    // 处理输入以及变量初始化
    string line;
    vector<string> board;
    while (cin >> line) {
        // 简单检查输入是否正确
        for (auto &ch : line) {
            if (!VALID_CH(ch)) {
                cout << "unknown character \'" << ch << "\' in \"" << line << "\"" << endl;
                return 0;
            }
        }
        board.push_back(line);
        ++rows;
    }
    if (rows == 0) {
        cout << "Hinter Fails! Invalid board input with rows = 0 !" << endl;
        return 0;
    }
    cols = board[0].size();
    // 执行算法并输出提示
    // dfs 算法算出来的标注未黄色
    unordered_set<int> safe_list, mine_list, dfs_list;
    bool f;
    // f = HinterStart(board, safe_list, mine_list, search_dep);
    // if (!f) {
    // 	cout << "Hinter Fails! err = 1" << endl;
    // 	return 0;
    // }
    f = SearchEdgeDigitGrid(board, dfs_list, search_dep);
    if (!f) {
    	cout << "Hinter Fails! err = 2" << endl;
    	return 0;
    }
    OutputResult(board, safe_list, mine_list, dfs_list);
    return 0;
}
