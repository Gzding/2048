#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QFrame>


#include <map>
#include <vector>

#include "game.h"


class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);

private:
    /// @brief 游戏结束时置为 true
    bool over;
    /// @brief 游戏核心类对象
    Game game;
    /// @brief 数字块的贴图
    std::map<int, QPixmap> num2figures;
    /// @brief 4x4 的 label 阵列，作为游戏 4x4 棋盘的数字块的载体
    std::vector<std::vector<QLabel*> > labels;
    /// @brief 显示得分的 label
    QLabel *score;
    /// @brief 显示步数的 label
    QLabel *step;
    /// @brief 游戏通过结算画面
    QFrame *victory;
    /// @brief 游戏失败结算画面
    QFrame *defeat;
public:
    /// @brief 初始化游戏：重置各项变量
    void initGame();
    /// @brief 重新游戏所需要的重置操作
    void newGame();
    /// @brief 核心操作：接收 键盘指令，调用相应的移动操作方法
    /// @param dir 移动的方向：0-上、1-下、2-左、3-右
    void move(int dir);
    /// @brief 让 mainwindow 调用的，传递 ui label 接口给本类使用
    /// @param label 4x4 棋盘中的 label，所在位置 (i, j)，从 0 开始
    /// @param i 
    /// @param j 
    void setLabel(QLabel *label, int i, int j);
    /// @brief 让 mainwindow 调用的，传递 ui label 接口给本类使用
    void setScoreLabel(QLabel *label);
    /// @brief 让 mainwindow 调用的，传递 ui label 接口给本类使用
    void setStepLabel(QLabel *label);
    /// @brief 让 mainwindow 调用的，传递 ui frame 接口给本类使用
    void setVictoryFrame(QFrame *frame);
    /// @brief 让 mainwindow 调用的，传递 ui frame 接口给本类使用
    void setDefeatFrame(QFrame *frame);
private:
    /// @brief 更新显示：从 game 获取当前棋盘的数值矩阵，根据数值给 label 设置对应的 pixmap
    void updateGrids();
    /// @brief 加载 pixmap：路径写死了
    void loadFigures();
};

