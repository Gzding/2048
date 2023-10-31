/**
 * @file mywidget.h
 * @brief Add ui and interface for the game
 * @author Gzding (gzding5@yeah.net)
 * @version 2.0
 * @date 2023-10-31
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
 * @par   通过得到的棋盘数据在操作方向上的变化，实现动画过程
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
    // 绘制的参数
    int n;              // 棋盘大小
    double winRatio;    // 窗口缩放比例
    double fontRatio;   // 字体大小比例
    int gridSize;       // 绘制的棋盘格子大小
    int gap;            // 格子间隔
    int o;              // 棋盘左上角在本 widget 中的开始坐标
    QMap<int, QColor> bg; // 格子的底色
    // 动画数据
    std::vector<std::vector<int> > oldGrids;    // 本次操作前的棋盘
    std::vector<std::vector<int> > newGrids;    // 本次操作后的棋盘
    std::vector<std::vector<int> > targets;    // 本次操作合并的位置和新生成的位置置为 true
    std::vector<std::vector<std::vector<int> > > paths; // 本次操作前棋盘各个方块在操作后的索引位置
    QVector<QVector<QRect> > rects; // 动画每帧中，各个方块的实际绘制位置
    QTimer *timer;  // 定时器
    int frameNum;   // 每次动画的总帧数
    int curf;   // 当前绘制第 curf 帧
    int mode;   // 当前绘制模式：0-非动画模式、1-移动动画、2-缩放动画

private:
    /// @brief 定时器结束事件处理函数：每次定时器计时结束，计算当前帧的 rects，然后 update
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

