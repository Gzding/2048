#include "To2048.h"

#include <QPushButton>
#include <QHBoxLayout>

To2048::To2048(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_To2048)
{
    // 在 ui 设计中：固定了窗口大小，设置了button、label等widget
    ui->setupUi(this);

    // 将 mywidget 插入到 ui 中
    mywidget.setParent(this);
    ((QHBoxLayout*)ui->centralWidget->layout())->insertWidget(0, &mywidget);

    // 固定棋盘大小
    mywidget.setMaximumWidth(500);
    mywidget.setMaximumHeight(500);
    mywidget.setMinimumWidth(500);
    mywidget.setMinimumHeight(500);

    // 按钮功能：重新开始游戏；关闭游戏调用本类的close，触发closeEvent
    connect(ui->pushButton_new, &QPushButton::clicked, &mywidget, [=](){mywidget.newGame();});
    connect(ui->pushButton_exit, &QPushButton::clicked, this, [=](){close();});

    // 给 mywidget label 接口
    mywidget.setStepLabel(ui->label_step);
    mywidget.setScoreLabel(ui->label_score);

    // 最后，初始化游戏
    mywidget.initGame();
}

To2048::~To2048()
{
    delete ui; 
}


void To2048::keyPressEvent(QKeyEvent *e){
    // cout << "Debug: key="  << endl;
    if(e->key()==Qt::Key_Up || e->key()==Qt::Key_W){
        mywidget.move(0);
    }
    if(e->key()==Qt::Key_Down || e->key()==Qt::Key_S){
        mywidget.move(1);
    }
    if(e->key()==Qt::Key_Left || e->key()==Qt::Key_A){
        mywidget.move(2);
    }
    if(e->key()==Qt::Key_Right || e->key()==Qt::Key_D){
        mywidget.move(3);
    }
}


void To2048::closeEvent(QCloseEvent *e){
    // std::cout << "close" << std::endl;
    mywidget.close();
}