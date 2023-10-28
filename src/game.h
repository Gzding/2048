#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <map>


class Game{
private:
    /// @brief 棋盘：空白块-0；数字块-数值
    int grids[4][4];  
    /// @brief 当前数字块的最大值
    int maxNum;
    /// @brief 空白块的数目
    int blankNum;
    /// @brief 得分
    int score;
    /// @brief 有效操作步数
    int step;
public:
    /// @brief 移动操作：上下左右移动
    enum Direction{up, down, left, right};
    Game();
    ~Game();
    /// @brief 初始化游戏
    void init();
    /// @brief 执行移动操作
    /// @param dir 移动方向 
    /// @return 在这个方向上，有有效的移动或合并操作，返回 true；否则，false
    bool checkAndMove(Direction dir);
    /// @brief 随机生成新数字块：位置随机、数值随机
    void generate();
    /// @brief 判断游戏是否胜利
    bool victory();
    /// @brief 判断游戏是否失败。为什么要写两个判断，额！我也不知道！
    bool defeat();
    /// @brief 获取 step
    int getStep();
    /// @brief 获取 score
    int getScore();
    /// @brief 获取 棋盘数值矩阵
    std::vector<std::vector<int> > getGrids();
    /// @brief 在字符界面以字符方式打印棋盘。可以生成，字符界面版的游戏。
    void print();
private:
    /// @brief 检查是否不能再移动了
    /// @return true：我还能操作！ false：欸嘿！又失败了！
    bool check();
    /// @brief 随机生成数
    /// @return [0, n) 中的整数（服从均匀分布）
    int getRandomIndex(int n);
    /// @brief 填充指定的空白块
    /// @param index 从上到下从左到右数，第 index 个空白块（从 0 开始）
    /// @param num 数字块的数值
    void fillBlank(int index, int num);
};

#endif // GAME_H
