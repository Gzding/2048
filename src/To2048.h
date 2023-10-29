/**
 * @file To2048.h
 * @brief MainWindow Class
 * @author Gzding (gzding5@yeah.net)
 * @version 0.1
 * @date 2023-10-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "ui_To2048.h"
#include <QMainWindow>

#include <QCloseEvent>
#include <QKeyEvent>


#include "mywidget.h"

class To2048 : public QMainWindow {
    Q_OBJECT
    
public:
    To2048(QWidget* parent = nullptr);
    ~To2048();
    /// @brief 重写此事件，给 mywidget 发送消息，让其执行必要的操作
    void closeEvent(QCloseEvent *event);
    /// @brief 监听键盘事件，调用 mywidget 的方法，这是游戏的交互方式
    void keyPressEvent(QKeyEvent *event);

private:
    Ui_To2048* ui;
    /// @brief 游戏内核和ui界面的结合类对象
    MyWidget mywidget;
};