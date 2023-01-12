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

## Output
A board will output which tell you which grids are safe or not for sure.
- 'S' with green blackground: represents a safe grid so you can click it later.
- 'M' with red blackground: represents a mine grid so you can mark it down later.

If nothing changes, the program fails to work because it isn't smart enough or more information should be provided.

# EXAMPLE
![示例图片](https://raw.githubusercontent.com/Dadagum/MineSweeperHinter/master/minesweeper.png)

# NOTE
- Make sure that your input is correct and you should input the whole board instead of some parts of it. If you want to just solve part of your board, please use '#' around it to avoid potential errors.
- This program is simple and just for fun. 

# FAIL TO SOLVE RIGHT NOW
## exp1
```
###
#21
#10
```

## exp2
001#######
111#######
M222######
2M2#######
12########
12########
##0#######

## exp3
001111####
111M22####
M222######
2M22#12###
12M2112###
1212#22###
M101######