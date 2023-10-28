#include "mywidget.h"

#include <QKeyEvent>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QFrame>

#include <windows.h>
#include <string>
#include <vector>


using namespace std;


/// @brief 构造：构造时要初始化 labels，大小 4x4，值默认
/// @param parent 
MyWidget::MyWidget(QWidget *parent) : QWidget(parent), labels(4, vector<QLabel*>(4))
{
    // 加载一次 pixmap
    loadFigures();
}

void MyWidget::loadFigures(){
    string path("./img/");
    string type(".png");
    // 总共 12 张图，0 是空白块的贴图
    vector<string> names({"0", "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048"});
    num2figures[0] = QPixmap(string(path+names[0]+type).c_str());
    for(unsigned int i=1, num=2; i<names.size(); i++, num*=2){
        num2figures[num] = QPixmap(string(path+names[i]+type).c_str());
    }
}

void MyWidget::initGame(){
    // 隐藏两个结算界面，结束标志置位
    victory->setVisible(false);
    defeat->setVisible(false);
    over = false;
    // 调用游戏核心的初始化
    game.init();
    // 刷新显示界面
    updateGrids();
}

void MyWidget::newGame(){
    // 和初始化一样，但后续可能有不同
    initGame();
}

void MyWidget::setLabel(QLabel *label, int i, int j){
    labels[i][j] = label;
}

void MyWidget::setScoreLabel(QLabel *label){
    score = label;
}

void MyWidget::setStepLabel(QLabel *label){
    step = label;
}

void MyWidget::setVictoryFrame(QFrame *frame){
    victory = frame;
}

void MyWidget::setDefeatFrame(QFrame *frame){
    defeat = frame;
}

void MyWidget::move(int dir){
    // 结束后，不执行操作
    if(over) return;

    bool res;
    // 调用核心方法-移动操作，接收返回值：是否是有效的移动操作
    if(0<=dir && dir<=3) res = game.checkAndMove(Game::Direction(dir));
    else return;
    // 是，则调用核心的随机生成方法；否则，直接返回
    if(res) game.generate();
    else return;
    
    // 更新显示
    updateGrids();
    
    // 更新显示之后，再判断游戏状态
    if(game.defeat()){
        // 失败，则显示失败结算界面，并置 结束标志 为 true
        over = true;
        defeat->setVisible(true);
    }
    if(game.victory()){
        // 成功，则同上
        over = true;
        victory->setVisible(true);
    }
}


// TODO:
// - 操作时，操作过快，会导致更新很慢，卡顿；
// - 原因可能是，不断的复制和加载图片导致的；
// - 解决方法：。。。
// TODO:
// 添加动画效果：label->move()
void MyWidget::updateGrids(){
    // get grids value from Game
    auto grids = game.getGrids();
    // set pixmap to each label
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            labels[i][j]->setPixmap(num2figures[grids[i][j]]);
        }
    }
    // update the score and step
    score->setText(QString(to_string(game.getScore()).c_str()));
    step->setText(QString(to_string(game.getStep()).c_str()));

    // sleep a while for update to avoid stucking
    Sleep(200);
}



