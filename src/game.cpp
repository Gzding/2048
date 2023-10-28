#include "game.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>

using namespace std;


Game::Game(int size, int target){
    if(size<4){
        cout << "ERROR: the board size is too small" << endl;
        exit(1);
    }
    this->size = size;
    this->target = target;
    if(checkTarget()==false){
        cout << "ERROR: the target is too small or invaild" << endl;
        exit(1);
    }
    grids = vector<vector<int> >(size, vector<int>(size));
}

Game::~Game(){}

void Game::init(){
    // 初始化 统计变量
    step = 0;
    score = 0;
    maxNum = 0;
    state = true;
    blankNum = size*size;
    // 初始化 棋盘
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            grids[i][j] = 0;
        }
    }
    // 随机选择两个格子填充 2
    generate();
    generate();
}

bool Game::moveTo(Direction dir){
    if(state == false) return false;

    // 是否移动、合并的标志
    bool move = false;
    bool merge = false;

    // 根据操作方向，先移动，后合并
    switch(dir){
        case left:{
            // 向左移动
            // 对于每一行分别将非零值都移动到最左边
            for(int i=0; i<size; i++){
                int zero = 0; // 记录上次开始寻找 0 值的位置
                for(int j=1; j<size; j++){
                    if(grids[i][j] == 0) continue;
                    // 将当前非零值 grids[i][j] 移动到从头开始的第一个 0 的位置
                    for(int pre=zero; pre<j; pre++){
                        if(grids[i][pre]==0){
                            move = true; // 设置标记
                            // 移动
                            grids[i][pre] = grids[i][j];
                            grids[i][j] = 0;
                            // 记住这个位置，以便下次加速
                            zero = pre+1;
                            break;
                        }
                    }
                }
            }
            // 向左合并
            // 规则：每个数最多参与一次合并
            // 比如：2 2 2 2 合并结果是 4 4 0 0 而不是 8 0 0 0
            for(int i=0; i<size; i++){
                for(int j=1; j<size; j++){
                    if(grids[i][j]==0) break;
                    if(grids[i][j-1]==grids[i][j]){
                        merge = true; // 设置标记
                        // 合并
                        grids[i][j-1] *= 2;
                        grids[i][j] = 0;
                        // 更新 blankNum
                        blankNum++;
                        // 更新 score
                        score += grids[i][j-1];
                        // 更新 maxNum
                        maxNum = (maxNum > grids[i][j-1]) ? maxNum : grids[i][j-1];
                        // 跳过这个 0
                        j++;
                    }
                }
            }
            // 再次移动
            for(int i=0; i<size; i++){
                int zero = 0;
                for(int j=1; j<size; j++){
                    if(grids[i][j] == 0) continue;
                    for(int pre=zero; pre<j; pre++){
                        if(grids[i][pre]==0){
                            grids[i][pre] = grids[i][j];
                            grids[i][j] = 0;
                            zero = pre+1;
                            break;
                        }
                    }
                }
            }
        }break;
        // 其他三个方向一样的步骤
        case right:{
            for(int i=0; i<size; i++){
                int zero = size-1;
                for(int j=size-2; j>=0; j--){
                    if(grids[i][j] == 0) continue;
                    for(int pre=zero; pre>j; pre--){
                        if(grids[i][pre]==0){
                            move = true;
                            grids[i][pre] = grids[i][j];
                            grids[i][j] = 0;
                            zero = pre-1;
                            break;
                        }
                    }
                }
            }
            for(int i=0; i<size; i++){
                for(int j=size-2; j>=0; j--){
                    if(grids[i][j]==0) break;
                    if(grids[i][j+1]==grids[i][j]){
                        merge = true;
                        grids[i][j+1] *= 2;
                        grids[i][j] = 0;
                        blankNum++;
                        score += grids[i][j+1];
                        maxNum = (maxNum > grids[i][j+1]) ? maxNum : grids[i][j+1];
                        j--;
                    }
                }
            }
            for(int i=0; i<size; i++){
                int zero = size-1;
                for(int j=size-2; j>=0; j--){
                    if(grids[i][j] == 0) continue;
                    for(int pre=zero; pre>j; pre--){
                        if(grids[i][pre]==0){
                            move = true;
                            grids[i][pre] = grids[i][j];
                            grids[i][j] = 0;
                            zero = pre-1;
                            break;
                        }
                    }
                }
            }
        }break;
        case up:{
            for(int j=0; j<size; j++){
                int zero = 0;
                for(int i=1; i<size; i++){
                    if(grids[i][j] == 0) continue;
                    for(int pre=zero; pre<i; pre++){
                        if(grids[pre][j]==0){
                            move = true;
                            grids[pre][j] = grids[i][j];
                            grids[i][j] = 0;
                            zero = pre+1;
                            break;
                        }
                    }
                }
            }
            for(int j=0; j<size; j++){
                for(int i=1; i<size; i++){
                    if(grids[i][j]==0) break;
                    if(grids[i-1][j]==grids[i][j]){
                        merge = true;
                        grids[i-1][j] *= 2;
                        grids[i][j] = 0;
                        blankNum++;
                        score += grids[i-1][j];
                        maxNum = (maxNum > grids[i-1][j]) ? maxNum : grids[i-1][j];
                        i++;
                    }
                }
            }
            for(int j=0; j<size; j++){
                int zero = 0;
                for(int i=1; i<size; i++){
                    if(grids[i][j] == 0) continue;
                    for(int pre=zero; pre<i; pre++){
                        if(grids[pre][j]==0){
                            move = true;
                            grids[pre][j] = grids[i][j];
                            grids[i][j] = 0;
                            zero = pre+1;
                            break;
                        }
                    }
                }
            }
        }break;
        case down:{
            for(int j=0; j<size; j++){
                int zero = size-1;
                for(int i=size-2; i>=0; i--){
                    if(grids[i][j] == 0) continue;
                    for(int pre=zero; pre>i; pre--){
                        if(grids[pre][j]==0){
                            move = true;
                            grids[pre][j] = grids[i][j];
                            grids[i][j] = 0;
                            zero = pre-1;
                            break;
                        }
                    }
                }
            }
            for(int j=0; j<size; j++){
                for(int i=size-2; i>=0; i--){
                    if(grids[i][j]==0) break;
                    if(grids[i+1][j]==grids[i][j]){
                        merge = true;
                        grids[i+1][j] *= 2;
                        grids[i][j] = 0;
                        blankNum++;
                        score += grids[i+1][j];
                        maxNum = (maxNum > grids[i+1][j]) ? maxNum : grids[i+1][j];
                        i--;
                    }
                }
            }
            for(int j=0; j<size; j++){
                int zero = size-1;
                for(int i=size-2; i>=0; i--){
                    if(grids[i][j] == 0) continue;
                    for(int pre=zero; pre>i; pre--){
                        if(grids[pre][j]==0){
                            move = true;
                            grids[pre][j] = grids[i][j];
                            grids[i][j] = 0;
                            zero = pre-1;
                            break;
                        }
                    }
                }
            }
        }break;
        default:{
            cout << "ERROR: error direction" << endl;
        }
    }

    // 有效操作则：
    if(move || merge){
        // 更新 step
        step++;
        if(isSuccess()==false){
            // 生成一个新的 
            generate();
            // 更新游戏状态
            check();
        }
    }
    
    return move || merge;
}

bool Game::isSuccess(){
    return maxNum==target;
}

void Game::generate(){
    // 在 [0, blankNum) 中随机选择一个数 index
    // 以 maxNum 控制的概率分布，随机选择 2、4、8 填充这个空白块

    cout << "Debug: blankNum=" << blankNum << endl;
    if(blankNum <= 0) return;

    // 随机位置 index
    int index = getRandomNum(blankNum);

    // 填充值
    int num = 1;
    // 分三个等级设置 2、4、8 三个数值的概率分布
    int x = getRandomNum(100);
    if(maxNum <= 256){
        // 256 之前：0.98, 0.02, 0
        num = (x<98) ? 2 : 4;
    }else if(maxNum <= 1024){
        // 1024 之前：0.9, 0.08, 0.02
        num = (x<90) ? 2 : ((x<98) ? 4 : 8);
    }else{
        // 最后：0.8, 0.15, 0.05
        num = (x<80) ? 2 : ((x<95) ? 4 : 8);
    }

    cout << "Debug: index=" << index << endl;
    cout << "Debug: num=" << num << endl;

    // 更新 maxNum
    maxNum = maxNum > num ? maxNum : num;

    // 遍历到第 index 个空白处
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            // 找到第 index 个 0
            if(grids[i][j] == 0){
                if(index==0){
                    grids[i][j] = num;
                    // 更新 blankNum
                    blankNum--;
                    return;
                }
                index--;
            }
        }
    }
}

bool Game::getState(){
    return state;
}

int Game::getStep(){
    return step;
}

int Game::getScore(){
    return score;
}

// 不知这里有没有必要给返回值加上 const 修饰
vector<vector<int> > Game::getGrids(){
    return grids;
}

// 打印棋盘格子
void Game::print(){
    int width = 2;
    int tar = target;
    while(tar > 0){
        width++;
        tar /= 10;
    }
    cout << setfill('-');
    cout << setw((width+2)*(size/2+size%2)+2) << "Game" << setw((width+2)*(size/2)-1) << '-' << endl;
    cout << setfill(' ');
    for(int i=0; i<size; i++){
        cout << "|";
        for(int j=0; j<size; j++){
            if(grids[i][j]>0) cout << setw(width) << grids[i][j];
            else cout << setw(width) << " ";
            cout << " |";
        }
        cout << endl;
        cout << setfill('-');
        cout << setw((width+2)*size+1) << '-' << endl;
        cout << setfill(' ');
    }
}


void Game::check(){
    // 有空白，肯定还能操作
    if(blankNum > 0) return;
    // 行、列 上有相邻的相同数值，则还能合并
    for(int i=0; i<size; i++){
        for(int j=1; j<size; j++){
            if(grids[i][j-1] == grids[i][j]) return;
            if(grids[j-1][i] == grids[j][i]) return;
        }
    }
    // 遍历完没有可合并的，那就是over了
    state = false;
}

int Game::getRandomNum(int n){
    // 这里若使用 C++ 的 random 库，似乎每次都是同样的随机数
    srand(time(NULL));
    return (rand() % n);
}

bool Game::checkTarget(){
    cout << "Debug: target=" << target << endl;
    if(target<2048 || target%1024!=0) return false;
    int tar = target / 1024;
    while(tar%2 == 0) tar /= 2;
    return tar==1;
}





// #define __MAIN__
#ifdef __MAIN__
// 字符版
int main(){
    const string exit("exit");
    const map<string, Game::Direction> cmds({
        {"up", Game::up}, {"down", Game::down}, {"left", Game::left}, {"right", Game::right},
        {"w", Game::up}, {"s", Game::down}, {"a", Game::left}, {"d", Game::right}
    });

    // 初始化
    Game game;
    game.init();
    cout << "Game Init Complete!" << endl;

    // 主循环
    while(true){
        // 打印数据
        cout << "Stat:  score:" << game.getScore() << ", " << "step:" << game.getStep() << endl;
        // 打印棋盘
        game.print();
        // 判断是否结束
        if(game.getState()==false){
            if(game.isSuccess()){
                cout << "Congratulations!" << endl;
                cout << "Step: " << game.getStep() << endl;
                cout << "Score: " << game.getScore() << endl;
            }else{
                cout << "Game Over !!!" << endl;
            }
            break;
        }
        // 输入命令
        string dir;
        cout << "Please input direction (up, down, left, right, exit):";
        cin >> dir;
        if(dir == exit) break;
        else if(cmds.count(dir)==0){
            cout << "Key error! Please input correct dirction!" << endl;
        }else{
            // 执行操作，有效执行则生成新数字块
            game.moveTo(cmds.at(dir));
        }
    }
    return 0;
}
#endif // __MAIN__
