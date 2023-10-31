/**
 * @file mywidget.h
 * @brief Add ui and interface for the game
 * @author Gzding (gzding5@yeah.net)
 * @version 0.1
 * @date 2023-10-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "game.h"

#include <QWidget>
#include <QPaintEvent>
#include <QLabel>
#include <QMap>
#include <QTimer>
#include <QRect>
#include <QVector>

#include <vector>

/**
 * @brief 给游戏创建 ui 界面和交互控制
 * @paragraph 核心功能
 * @par - ui 界面的绘制：
 * @par   使用 qpainter 绘制背景和棋盘
 * @par   所有数字块通过 drawText 实现
 * @par - 移动和合并的动画实现：
 * @par   通过操作前后，根据 game.getGrids() 得到的棋盘数据在操作方向上的变化，实现动画过程
 * @par - 游戏的交互控制：
 * @par   在 mainwindow 的键盘事件响应函数中对交互键的触发调用相应的操作
 * @par - 游戏结束界面：
 * @par   绘制图片即可
 */
class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget();
public:
    /**
     * @brief 重写父类 QWidget 的 paintEvent，绘制棋盘，显示当前棋局
     * @paragraph 详细操作：
     * @par 1. 获取棋盘当前数据
     * @par 2. 设置画家等参数后绘制棋盘
     * @par 3. TODO：加入 animation
     */
    void paintEvent(QPaintEvent *e);
    /// @brief 初始化游戏：重置各项变量
    void initGame();
    /// @brief 重新游戏所需要的重置操作
    void newGame();
    void close();
    /// @brief 核心操作：接收 键盘指令，调用相应的移动操作方法
    /// @param dir 移动的方向：0-上、1-下、2-左、3-右
    void move(int dir);
    /// @brief 让 mainwindow 调用的，传递 ui label 接口给本类使用
    void setScoreLabel(QLabel *label);
    /// @brief 让 mainwindow 调用的，传递 ui label 接口给本类使用
    void setStepLabel(QLabel *label);

private:
    int n;              // 棋盘大小
    double winRatio;    // 窗口缩放比例
    double fontRatio;   // 字体大小比例
    int gridSize;       // 绘制的棋盘格子大小
    int gap;            // 格子间隔
    int o;              // 棋盘左上角在本 widget 中的开始坐标
    QMap<int, QColor> bg; // 格子的底色
    Game::Direction direct;
    int nstep;
    int nscore;
    std::vector<std::vector<int> > oldGrids;
    std::vector<std::vector<int> > newGrids;
    std::vector<std::vector<bool> > targets;
    std::vector<std::vector<std::vector<int> > > paths;
    QVector<QVector<QRect> > rects;
    QTimer *timer;
    int frameNum;
    int curf;
    int mode;

private:
    void timerTimeout();

private:
    /// @brief 游戏结束时置为 true
    bool over;
    /// @brief 游戏核心类对象
    Game game;
    /// @brief 显示得分的 label
    QLabel *score;
    /// @brief 显示步数的 label
    QLabel *step;
};

