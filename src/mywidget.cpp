#include "mywidget.h"

#include <QLabel>

#include <QPainter>
#include <QFont>
#include <QString>
#include <QPoint>
#include <QColor>
#include <QRect>

#include <vector>
#include <cmath>


using namespace std;




// 先 n 后 game（这是在类声明的顺序）
MyWidget::MyWidget(QWidget *parent) : QWidget(parent), n(4), game(n, 2048)
{
    // 计算一些绘制参数
    winRatio = 500 / 500.0;
    fontRatio = winRatio*(4.0/n);
    gridSize = int(winRatio * 500*0.9 / n);
    gap = int(gridSize*0.1);
    o = int(winRatio * 500*0.05 + gap*0.5);

    bg = QMap<int, QColor>({
        {0, QColor(205, 193, 180)},
        {2, QColor(45, 132, 196)}, // #2d84c4
        {4, QColor(60, 169, 238)},
        {8, QColor(97, 189, 238)},
        {16, QColor(165, 211, 235)},
        {32, QColor(255, 255, 255)},
        {64, QColor(223, 166, 147)},
        {128, QColor(220, 110, 85)},
        {256, QColor(225, 75, 50)},
        {512, QColor(195, 55, 38)},
        {1024, QColor(192, 152, 255)},
        {2048, QColor(168, 115, 255)},
        {4096, QColor(128, 62, 227)},
        {8192, QColor(104, 45, 187)}
    });
}

void MyWidget::initGame(){
    // 结束标志
    over = false;
    // 调用游戏核心的初始化
    game.init();
    update();
}

void MyWidget::newGame(){
    // 和初始化一样，但后续可能有不同
    initGame();
}

void MyWidget::close(){
    // 可能需要的操作
    // cout << "Click EXIT" << endl;
}

void MyWidget::setScoreLabel(QLabel *label){
    score = label;
}

void MyWidget::setStepLabel(QLabel *label){
    step = label;
}



void MyWidget::move(int dir){
    // 结束后，不执行操作
    if(over || dir<0 || dir>3) return;

    // 调用核心方法-移动操作，接收返回值：是否是有效的移动操作
    if(game.moveTo(Game::Direction(dir))) update();
    
    // 判断是否结束
    if(game.getState()==false){
        over = true;
        update();
    }
}



void MyWidget::paintEvent(QPaintEvent *e){
    vector<vector<int> > newGrids = game.getGrids();

    // cout << "paint" << endl;
    QPainter painter(this);
    QRect r;


    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            r.setRect(o + j*gridSize, o + i*gridSize, gridSize-gap, gridSize-gap);
            // 外框颜色：透明
            painter.setPen(Qt::transparent);
            // 底色
            // painter.setBrush(QColor(255,250,222,255));
            painter.setBrush(bg[newGrids[i][j]]);
            painter.drawRect(r); // 这里注意坐标对应问题，不然会画反
            if(newGrids[i][j]==0) continue;
            // 数字颜色、字体
            int fontSize = int((60-10*int(log10(newGrids[i][j])))*fontRatio);
            // cout << "fontsize: " << fontSize << endl;
            painter.setPen(Qt::black);
            painter.setFont(QFont("Consolas", fontSize)); // 6-20, 5-25, 4-30, 3-40, 2-50, 1-60
            painter.drawText(QRectF(r), QString::number(newGrids[i][j]), QTextOption(Qt::AlignCenter));
        }
    }

    score->setText(QString::number(game.getScore()));
    step->setText(QString::number(game.getStep()));

    if(over){
        r.setRect(int(winRatio*500*0.1), int(winRatio*500*0.35), int(winRatio*500*0.8), int(winRatio*500*0.3));
        // 外框颜色：透明
        painter.setPen(Qt::transparent);
        // 底色
        painter.setBrush(QColor(255,250,222,255));
        painter.drawRect(r); // 这里注意坐标对应问题，不然会画反
        painter.setFont(QFont("Consolas", int(60*fontRatio)));
        if(game.isSuccess()){
            // 颜色、字体
            painter.setPen(Qt::blue);
            painter.drawText(QRectF(r), QString("VICTORY!"), QTextOption(Qt::AlignCenter));
        }else{
            painter.setPen(Qt::red);
            painter.drawText(QRectF(r), QString("GAME OVER"), QTextOption(Qt::AlignCenter));
        }
    }
}

