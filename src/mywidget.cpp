#include "mywidget.h"

#include <QLabel>

#include <QPainter>
#include <QFont>
#include <QString>
#include <QPoint>
#include <QColor>
#include <QRect>
#include <QSize>

#include <vector>
#include <cmath>

#include <windows.h>

using namespace std;




// 先 n 后 game（这是在类声明的顺序）
MyWidget::MyWidget(QWidget *parent) : QWidget(parent), n(4), game(n, 2048)
{
    frameNum = 20; // 每次移动、缩放动画的总帧数
    timer = new QTimer(this);

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

    rects = QVector<QVector<QRect> >(n, QVector<QRect>(n));

    // 将定时器计时结束信号连接到槽（计时结束事件处理函数）
    connect(timer, &QTimer::timeout, this, &MyWidget::timerTimeout);
}


MyWidget::~MyWidget()
{
    //释放
    if(timer){
        delete  timer;
    }
}

void MyWidget::initGame(){
    mode = 0;
    // 结束标志
    over = false;
    // 调用游戏核心的初始化
    game.init();
    // 初始化棋盘
    oldGrids = vector<vector<int> >(n, vector<int>(n, 0));
    newGrids = game.getGrids();
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
    if(game.moveTo(Game::Direction(dir))){
        // 操作有效，则需要更新画面
        // 获取棋盘数据
        oldGrids = newGrids;
        newGrids = game.getGrids();
        score->setText(QString::number(game.getScore()));
        step->setText(QString::number(game.getStep()));
        // 获取动画过程需要的数据
        paths = game.getPaths();
        targets = game.getTargets();
        // 设置参数
        mode = 1;
        curf =0;
        // 开启定时器
        timer->start(5);
    }
    
    // 判断是否结束
    if(game.getState()==false){
        over = true;
    }
}


// void 

/**
 * @brief animation method
 * 
 * 再次更改 game.cpp - moveTo()：
 * 改变移动合并方法，使得不仅计算出操作后的结果，还能得出过程
 * 对于操作方向上的每一列，按序遍历一遍，寻零值开始位置开始置为零，且遍历从 1 开始
 * 对于每个非零值，从寻零值的开始位置，寻找第一个零，或相同值
 * 要是相同值，则合并；否则，是移动
 * 记录目标位置在一个新的矩阵 paths 中的原位置中
 * 若是合并，再在另一个新矩阵 targets 中的目标位置标记，表示此处是合并后结果出现位置
 * 另外，有效操作后的新生成的方块，只在合并标记矩阵中标记
 * 保存两个矩阵，作为此次有效操作的过程记录
 * 另外，不要忘记更新最终结果
 * 
 * 根据 oldGrids paths 即可插值计算出每个 old grid 的位置动画了
 * 根据 targets newGrids 即可插值计算出每个合并后的或新生成的一个 grid 的大小动画了
 * 
 * 上述插值计算还需要参数 curf，即动画的第 curf 帧
 * 动画实现需要定时器，定时器设置一个固定时间（或者可以根据 curf 改变时间间隔，实现有加速度的动画）
 * 每次 move 时是有效操作，就开启定时器，并重置 curf=0
 * 定时器每次 timeout，就根据 curf 按照上述方法插值计算出当前帧（各个 grid 的位置）
 * 先是移动动画（位置移动），结束再是生成动画（大小变化、透明度）
 * 然后，update
 * paintEvent 中根据当前帧，进行绘制
 * 到 curf == frameNum 时的 timeout 关闭定时器
 */

void MyWidget::timerTimeout(){
    // 线性插值
    // 根据当前帧序 curf 等参数计算当前帧，即：各个方块的位置
    // 先移动
    if(mode == 1){
        if(curf<0 || curf>frameNum) return;
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                // 对实际绘制位置插值
                double x = (curf*(o+paths[i][j][1]*gridSize) + (frameNum-curf)*(o+j*gridSize))*1.0/frameNum;
                double y = (curf*(o+paths[i][j][0]*gridSize) + (frameNum-curf)*(o+i*gridSize))*1.0/frameNum;
                rects[i][j].setX(x);
                rects[i][j].setY(y);
                // 大小不变
                rects[i][j].setSize(QSize(gridSize-gap, gridSize-gap));
            }
        }
    }
    // 后缩放
    else if(mode == 2){
        if(curf<0 || curf>frameNum) return;
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                // 只对标记的进行缩放动画：即合并产生的新数字块、新生成的一个数字块
                if(targets[i][j]!=0){
                    // 大小插值
                    double wh = (gridSize - gap) * (curf+3.0) / (frameNum-1.0);
                    // 中心位置不变，但左上角位置还得重新计算
                    rects[i][j].setX(int(o + j*gridSize + (gridSize-gap - wh)/2.0));
                    rects[i][j].setY(int(o + i*gridSize + (gridSize-gap - wh)/2.0));
                    rects[i][j].setSize(QSize(int(wh), int(wh)));
                }else{
                    // 其他的不动
                    rects[i][j].setRect(o + j*gridSize, o + i*gridSize, gridSize-gap, gridSize-gap);
                }
            }
        }
    }else{
        // 动画结束则关闭定时器
        timer->stop();
    }
    // 更新
    update();
}



/**
 * @paragraph animation
 * @par 将一次操作造成的变化视为一次单独的动画，
 * @par 一次动画的时间分成两部分，移动时间、生成时间
 * @par 这里有个问题：如果操作过快，是否导致绘制来不及？
 * @par 可以看到：每次 move 操作是有效的，就调用一次 update，即 paint 一次
 * @par 是否有可能：在本次 move 更新了 game 后，上一次的 move 产生的 paint 事件处理还没开始呢？
 * @par 若有可能，则上一次的 paintEvent 可能会获取到本次 move 更新 game 后的 game
 * @par 这对于不加动画没有影响，但是加了动画，由于获得的前后状态不是相邻的，动画可能会出错
 * @par 使用 sleep 测试，发现会丢帧，有的 paintEvent 事件没有执行，
 * @par 但是，执行 paintEvent 时，去 game 获取的数据和 move 时保存的 game 数据是一样的
 * @par 所以结果就是：
 * @par - move 执行时保存的 game 数据，与其触发的 paint 事件处理时获取的 game 数据一样
 * @par - 即：不会有多次 move 之后才执行缓慢的 paint 事件处理，而是直接不处理没来的及处理的 paint 事件
 * @par - 在 move 中，更新前后两次 game 的状态数据，保证 paint 事件处理的是相邻的两帧
 * @par - 问题是：paint 被跳过时，有丢失动画片段的可能，直接就是显示了结果，但显示的动画绝对不会错，结果也不会错
 * @par - 对于 paint 处理慢丢帧的情况，TODO：如何解决？先不管，做好动画再说，说不定 paint 很快呢！
 * @par - 
 * @par oldGrids -> newGrids
 * @par for each line on the operate direction
 * @par - just size's grids become size's grids
 * @par - 在 oldGrids 中检查合并对，
 * @par - 将合并对、不合并的分别作为一个个单元进行动画处理；
 * @par - 对于不合并但要移动的，执行移动效果
 * @par - 对于合并对，先分别执行移动效果（两者目标位置一样，且前面的置顶），再执行生成效果
 * @par - 
 * @par - 先计算每个单元的起末位置，
 * @par - 然后进行移动、合并等动画，
 * @par - 不合并的就平移移动，
 * @par - 合并对，在移动到目标位置后做合并效果
 * @par - 
 * @par - 移动效果
 * @par - 移动速度：完成移动的时间相同
 * @par - 匀速移动
 * @par - 每个移动或合并的都有这个效果
 * @par -
 * @par - 合并效果
 * @par - 可以分为：重叠效果（就是合并对的第二个移动到第一个下面）、生成效果
 * @par -
 * @par - 生成效果 
 * @par - 在目标位置由小变大再变小到正常大小
 * @par - 新生成的、合并结果，这两个都有这个效果
 */
void MyWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    QRect r; 
    double fontSize;

    // 先绘制空白棋盘：都是 0 的底色
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            // 计算位置
            r.setRect(o + j*gridSize, o + i*gridSize, gridSize-gap, gridSize-gap);
            // 外框颜色：透明
            painter.setPen(Qt::transparent);
            // 底色
            painter.setBrush(bg[0]);
            painter.drawRect(r); // 这里注意坐标对应问题，不然会画反
        }
    }


    // 再绘制数字块
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            // 移动帧：绘制的是 oldGrids 的移动
            if(mode==1){
                if(oldGrids[i][j]==0) continue;
                // 底色
                painter.setPen(Qt::transparent);
                painter.setBrush(bg[oldGrids[i][j]]);
                painter.drawRect(rects[i][j]);
                // 数值
                fontSize = (60-10*int(log10(oldGrids[i][j])))*fontRatio;
                painter.setPen(Qt::black);
                painter.setFont(QFont("Consolas", int(fontSize))); // 6-20, 5-25, 4-30, 3-40, 2-50, 1-60
                painter.drawText(QRectF(rects[i][j]), QString::number(oldGrids[i][j]), QTextOption(Qt::AlignCenter));
            }
            // 缩放帧：绘制的是 newGrids 的缩放
            else if(mode==2){
                if(newGrids[i][j]==0) continue;
                r.setRect(o + j*gridSize, o + i*gridSize, gridSize-gap, gridSize-gap);
                // 合并生成的，在缩放动画时，也要绘制 oldGrids，此时其数值是 newGrids 的一半
                if(targets[i][j]==2){
                    // 底色
                    painter.setPen(Qt::transparent);
                    painter.setBrush(bg[int(newGrids[i][j]/2)]);
                    painter.drawRect(r);
                    // 数值
                    fontSize = (60-10*int(log10(newGrids[i][j]/2)))*fontRatio;
                    painter.setPen(Qt::black);
                    painter.setFont(QFont("Consolas", int(fontSize))); // 6-20, 5-25, 4-30, 3-40, 2-50, 1-60
                    painter.drawText(QRectF(r), QString::number(int(newGrids[i][j]/2)), QTextOption(Qt::AlignCenter));
                }
                // 底色
                painter.setPen(Qt::transparent);
                painter.setBrush(bg[newGrids[i][j]]);
                painter.drawRect(rects[i][j]);
                // 数值
                fontSize = (60-10*int(log10(newGrids[i][j])))*fontRatio;
                fontSize = targets[i][j]!=0 ? fontSize*((curf+1.0)/(frameNum-1.0)) : fontSize;
                painter.setPen(Qt::black);
                painter.setFont(QFont("Consolas", int(fontSize))); // 6-20, 5-25, 4-30, 3-40, 2-50, 1-60
                painter.drawText(QRectF(rects[i][j]), QString::number(newGrids[i][j]), QTextOption(Qt::AlignCenter));
            }
            // 再绘制一帧最终结果帧
            else if(mode==0){
                if(newGrids[i][j]==0) continue;
                r.setRect(o + j*gridSize, o + i*gridSize, gridSize-gap, gridSize-gap);
                // 底色
                painter.setPen(Qt::transparent);
                painter.setBrush(bg[newGrids[i][j]]);
                painter.drawRect(r);
                // 数值
                fontSize = (60-10*int(log10(newGrids[i][j])))*fontRatio;
                painter.setPen(Qt::black);
                painter.setFont(QFont("Consolas", int(fontSize))); // 6-20, 5-25, 4-30, 3-40, 2-50, 1-60
                painter.drawText(QRectF(r), QString::number(newGrids[i][j]), QTextOption(Qt::AlignCenter));
            }
        }
    }

    /**
     * 在这里改变变量 curf mode，有一个奇效：
     * 不管操作是否多快，paintEvent 是否丢帧了，
     * 只要执行了，就是连续的，则动画就是完整的。
     * 因为，curf 是在这里改变的，
     * 丢帧，则 curf 没变，下一次 timeout 还是绘制丢弃的帧
     * 所以，不管哪一次 paintEvent 被丢了，都会继续绘制相同的内容，
     * 最终动画是连续的、完整的
     */
    // 继续下一帧
    curf++;
    // 动画结束切换模式
    if(mode==1 && curf==frameNum){
        mode = 2;
        curf = 0;
    }
    if(mode==2 && curf==frameNum){
        mode = 0;
        curf = 0;
    }

    // score->setText(QString::number(game.getScore()));
    // step->setText(QString::number(game.getStep()));

    // 绘制结束界面
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


// #define __NOANIMATION__
#ifdef __NOANIMATION__
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

    oldGrids = newGrids;
}
#endif // __NOANIMATION__