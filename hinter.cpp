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
 * 或者是暂时还无法判断，需要玩家自行点击
 */

// 棋盘状态定义
#define S_UNCLICKED ('#')
#define S_MARKED    ('M')
#define S_MINE      ('X')
#define S_SAFE      ('S') // 非输入字符
#define COUNT(cnt)  ('0' + cnt)
#define IN_DANGER(ch) (ch == S_MARKED || ch == S_MINE)
#define VALID_CH(ch) (isdigit(ch) || ch == S_UNCLICKED || IN_DANGER(ch))

using pii = pair<int,int>;
const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
constexpr int dSize = 8;

vector<string> board;
int rows = 0, cols = 0;

static bool ValidMove(int x, int y) {
    return 0 <= x && x < rows && 0 <= y && y < cols;
}

static void UpdateStateAround(int x, int y, char old_state, char new_state, unordered_set<int> &update_list) {
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

static bool AroundHasDigit(int x, int y) {
    int nx, ny;
    for (int k = 0; k < dSize; ++k) {
        nx = x + dx[k];
        ny = y + dy[k];
        if (ValidMove(nx, ny) && isdigit(board[nx][ny])) {
            return true;
        }
    }
    return false;
}

static bool HinterStart(unordered_set<int> safe_list, unordered_set<int> mine_list) {
    bool finish = false;
    int nx, ny;
    while (!finish) {
        finish = true;
        for (int x = 0; x < rows; ++x) {
            for (int y = 0; y < cols; ++y) {
                if (!isdigit(board[x][y])) {
                    continue;
                }
                int cnt = board[x][y] - '0', dangers = 0, unknowns = 0;
                // 数字格，检查四周是否已经填充完成
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
                //cout << cnt << " " << dangers << " " << unknowns << endl;
                if (cnt == dangers) {
                    // 全部雷都已经扫出来了，剩下的方块都是安全的，标记一下
                    UpdateStateAround(x, y, S_UNCLICKED, S_SAFE, safe_list);
                } else if (unknowns == cnt - dangers) {
                    // 剩下的都是雷，需要标记一下
                    finish = false; // 下轮继续迭代更新
                    UpdateStateAround(x, y, S_UNCLICKED, S_MARKED, mine_list);
                } else if (unknowns == 0) {
                    // 错误的棋盘
                    return false;
                }
            }
        }
    }
    // 更新安全格子状态
    for (auto &val : safe_list) {
        int x = val / cols, y = val % cols;
        bool can_update = true;
        int cnt = 0;
        for (int k = 0; k < dSize; ++k) {
            nx = x + dx[k];
            ny = y + dy[k];
            if (!ValidMove(nx, ny)) {
                continue;
            } else if (board[nx][ny] == S_UNCLICKED) {
                can_update = false;
                break;
            } else if (IN_DANGER(board[nx][ny])) {
                ++cnt;
            }
        }
        if (can_update) {
            board[x][y] = COUNT(cnt);
        }
    }
    // 猜格子
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            if (board[x][y] == S_UNCLICKED && AroundHasDigit(x, y)) {
                // TODO
            }
        }
    }
    return true;
}

// 输出提示
void OutputResult(unordered_set<int> safe_list, unordered_set<int> mine_list) {
    for (int x = 0, idx = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y, ++idx) {
            if (safe_list.find(idx) != safe_list.end()) {
                print(string(1, board[x][y]), color_black, color_green);
            } else if (mine_list.find(idx) != mine_list.end()) {
                print(string(1, board[x][y]), color_black, color_red);
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
        cout << "invalid board input with rows = 0 !" << endl;
        return 0;
    }
    cols = board[0].size();
    // 执行算法并输出提示
    unordered_set<int> safe_list, mine_list;
    HinterStart(safe_list, mine_list);
    OutputResult(safe_list, mine_list);
    return 0;
}

