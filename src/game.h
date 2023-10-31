/**
 * @file game.h
 * @brief The core class of the game
 * @author Gzding (gzding5.yeah.net)
 * @version 0.1
 * @date 2023-10-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <map>


/**
 * @brief 2048小游戏核心类
 * @details 实现2048小游戏的核心逻辑类，使用此类即可实现一个简单的字符界面版的游戏！
 * @details 在此基础上，使用 QT 添加 UI 界面，就可以实现完整的 2048 小游戏了！
 * @paragraph 核心逻辑
 * @par - 游戏初始化：使用随机生成方法，随机生成两个数字 2 在棋盘上；
 * @par - 移动合并操作：根据操作方向（键盘上下左右键），移动、合并所有数字；
 * @par - 随机生成：随机生成数字，概率可根据游戏进度（目前最大数值）调整；
 * @par - 结束判断：棋盘满了，且无法在任何方向继续合并；
 */
class Game{
public:
    /// @brief 移动操作：上下左右移动
    enum Direction{up, down, left, right};
private:
    /// @brief 棋盘：空白块-0；数字块-数值 2^n, n=1,2,...
    std::vector<std::vector<int> > grids;
    int size;           /// @brief 棋盘大小：默认4；可能可以玩更大的
    int target;         /// @brief 目标值：默认2048；
    int maxNum;         /// @brief 当前数字块的最大值
    int blankNum;       /// @brief 空白块的数目：[0-16]
    int score;          /// @brief 得分
    int step;           /// @brief 有效操作步数
    bool state;         /// @brief 游戏状态：true-正常、false-结束
    std::vector<std::vector<std::vector<int> > > paths;
    std::vector<std::vector<bool> > targets;
public:
    /**
     * @brief Construct a new Game
     * @param size 棋盘大小：默认 4，可以更大
     * @param target 目标值：默认2048，可以更大，但须是 2 的幂
     */
    Game(int size=4, int target=2048);
    ~Game();
    /// @brief 初始化游戏
    void init();
    /**
     * @brief 向某个方向执行操作。返回是否是有效操作，可以懒惰更新
     * @param dir 方向，在 Game::Direction 中定义
     * @return - true：有效操作，即此次操作对棋盘有所改变
     * @return - false：无效操作，此次操作对棋盘没有改变
     */
    bool moveTo(Direction dir);
    /**
     * @brief 判断游戏是否胜利，根据当前最大值判断，但前提是 check 返回 false
     * @return - true : check()==false && maxNum==2048
     * @return - false : check()==false && maxNum<2048
     */
    bool isSuccess();
    /// @brief 随机生成新数字块：位置随机、数值随机
    void generate();
    /// @brief 获取 step
    int getStep();
    /// @brief 获取 score
    int getScore();
    /// @brief 获取 state
    bool getState();
    /// @brief 获取 棋盘数值矩阵
    std::vector<std::vector<int> > getGrids();
    std::vector<std::vector<bool> > getTargets();
    std::vector<std::vector<std::vector<int> > > getPaths();
    /// @brief 在字符界面以字符方式打印棋盘。可以实现字符界面版的游戏。
    void print();
private:
    /// @brief 检查当前棋盘状态: 是否还能继续游戏
    void check();
    /**
     * @brief 随机生成数
     * @param n 指定范围: [0, n)
     * @return - int : [0, n) 中的整数（服从均匀分布）
     */
    int getRandomNum(int n);
    /// @brief 检查 target 是否大于等于 2048 且是 2 的 n 次幂
    bool checkTarget();
    void reset();
};

#endif // GAME_H
