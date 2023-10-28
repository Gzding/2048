#include "To2048.h"


#include <QKeyEvent>
#include <QPushButton>
#include <QPixmap>
#include <QIcon>

To2048::To2048(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_To2048)
{
    ui->setupUi(this);

    // 设置几个 label 的贴图
    ui->label_img->setPixmap(QPixmap("./img/img.jpg"));
    ui->label_victory->setPixmap(QPixmap("./img/victory.png"));
    ui->label_defeat->setPixmap(QPixmap("./img/defeat.png"));

    // 按钮功能：重新开始游戏
    connect(ui->button_newGame, &QPushButton::clicked, &mywidget, [=](){std::cout << "clicked\n"; mywidget.newGame();});
    connect(ui->button_newGame_1, &QPushButton::clicked, &mywidget, [=](){std::cout << "clicked\n"; mywidget.newGame();});
    connect(ui->button_newGame_2, &QPushButton::clicked, &mywidget, [=](){std::cout << "clicked\n"; mywidget.newGame();});


    // 将 方块 label 指针 保存到 mywidget，方便调用
    mywidget.setLabel(ui->label_0_0, 0, 0);
    mywidget.setLabel(ui->label_0_1, 0, 1);
    mywidget.setLabel(ui->label_0_2, 0, 2);
    mywidget.setLabel(ui->label_0_3, 0, 3);

    mywidget.setLabel(ui->label_1_0, 1, 0);
    mywidget.setLabel(ui->label_1_1, 1, 1);
    mywidget.setLabel(ui->label_1_2, 1, 2);
    mywidget.setLabel(ui->label_1_3, 1, 3);

    mywidget.setLabel(ui->label_2_0, 2, 0);
    mywidget.setLabel(ui->label_2_1, 2, 1);
    mywidget.setLabel(ui->label_2_2, 2, 2);
    mywidget.setLabel(ui->label_2_3, 2, 3);

    mywidget.setLabel(ui->label_3_0, 3, 0);
    mywidget.setLabel(ui->label_3_1, 3, 1);
    mywidget.setLabel(ui->label_3_2, 3, 2);
    mywidget.setLabel(ui->label_3_3, 3, 3);

    // 同 上
    mywidget.setScoreLabel(ui->label_score_v);
    mywidget.setStepLabel(ui->label_step_v);
    mywidget.setVictoryFrame(ui->frame_victory);
    mywidget.setDefeatFrame(ui->frame_defeat);

    // 最后，初始化游戏
    mywidget.initGame();
}

To2048::~To2048()
{
    delete ui; 
}



void To2048::keyReleaseEvent(QKeyEvent *event){
    // 上下左右
    if(event->key() == Qt::Key_Up){
        mywidget.move(0);
    }
    if(event->key() == Qt::Key_Down){
        mywidget.move(1);
    }
    if(event->key() == Qt::Key_Left){
        mywidget.move(2);
    }
    if(event->key() == Qt::Key_Right){
        mywidget.move(3);
    }
}
