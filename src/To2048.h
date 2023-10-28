#pragma once
#include "ui_To2048.h"
#include <QMainWindow>
#include <QKeyEvent>

#include "mywidget.h"

class To2048 : public QMainWindow {
    Q_OBJECT
    
public:
    To2048(QWidget* parent = nullptr);
    ~To2048();
    /// @brief 接收方向键消息，调用 mywidget 的方法执行移动操作。
    /// @param event 
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui_To2048* ui;
    /// @brief 游戏内核和ui界面的结合类对象
    MyWidget mywidget;
};