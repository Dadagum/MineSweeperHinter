# HOW TO USE
## Compile
```
g++ hinter.cpp -std=c++11 -o hinter
```

## Prepare a file
Input a board containing the following characters：
- '#': represents a unclicked(unrevealed) grid.
- '[0 - 8]': represents how many mines are adjacent to this revealed grid.
- 'X' or 'M': represents a mine, while 'X' stands for a revealed mine and 'M' only stands for a mark that the user add. They are all treated the same to the program.

For example, prepare a file('input.txt') which contains:
```
########2#
1111113#2#
##########
```

## Run
```
./hinter < input.txt
```

you can config the dfs search depth by modifying the variable "search_dep" in hinter.cpp.

## Output
A board will output which tell you which grids are safe or not for sure.
- 'S' with green blackground: represents a safe grid, but the program doesn't know how many mines are around it. You can click it to find out later.
- '[0 - 8]' with green blackground: represents a safe grid and the program has already calculated how many mines are around the grid.
- 'M' with red blackground: represents a mine grid so you can mark it down later.

If nothing changes, the program fails to work because it isn't smart enough or more information should be provided.

# EXAMPLE
![示例图片](https://raw.githubusercontent.com/Dadagum/MineSweeperHinter/master/minesweeper.png)

# NOTE
- Make sure that your input is correct and you should input the whole board instead of some parts of it. If you want to just solve part of your board, please use '#' around it to avoid potential errors.
- The main idea in this program: intuitive filling + simple basic rule + dfs
- This program is simple and just for fun. 
