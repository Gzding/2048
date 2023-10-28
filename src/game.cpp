#include "game.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>

using namespace std;


// 构造函数
Game::Game(){}
// 析构函数
Game::~Game(){}

// 初始化游戏
void Game::init(){
    // 统计变量置位
    blankNum = 16;
    maxNum = 0;
    score = 0;
    step = 0;
    // 棋盘清空：先全部归零
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            grids[i][j] = 0;
        }
    }
    // 随机选择两个格子，以大概率填充 2，小概率填充 4
    // 更新棋盘格子
    // 更新统计变量
    int index1 = getRandomIndex(16);
    int index2 = getRandomIndex(15);
    int x = getRandomIndex(100);
    if(x < 98) fillBlank(index1, 2);
    else fillBlank(index1, 4);
    x = getRandomIndex(100);
    if(x < 98) fillBlank(index2, 2);
    else fillBlank(index2, 4);
    blankNum -= 2;
}

/// @brief 执行移动操作
/// @param dir 移动方向 
/// @return 在这个方向上，有有效的移动或合并操作，返回 true；否则，false
bool Game::checkAndMove(Direction dir){
    // 先检查一遍是否有任一方向还能移动
    if(!check()) return false;
    
    // 移动和合并可以移动和合并的所有数字块
    // 更新统计变量：最大数字块的值、得分、步数、空白块数目
    int i, j, k;
    bool moveOrMerge = false; // 有效移动或合并的标志
    int nums[4] = {0}; // 辅助数组：拷贝一列数值中的所有非零数值，在这个数组内合并可以合并的相同数值
    // 合并步骤：各个方向都一样，使用 lambda 更简洁
    auto merge = [&](){
        // 在拷贝所有非零数值后，剩余的置为 0
        while(k < 4) nums[k++] = 0;
        for(k=1; k<4; k++){
            // 可以合并的条件
            if(nums[k]>0 && nums[k-1] == nums[k]){
                // merge
                nums[k-1] *= 2;
                nums[k] = 0;
                // update statistics
                moveOrMerge = true; // 合并数字块的标志
                blankNum++; // bug 记录：真离谱！每天都在写奇奇怪怪的bug！这段代码的进入条件不加 nums[k]>0 条件，则此行更新就是 bug！
                score += nums[k-1];
                maxNum = maxNum > nums[k-1] ? maxNum : nums[k-1];
                // skip
                k++; // 避免重复合并：规则是两个数值合并后，这个数值不能再参与合并
            }
        }
    };
    // 四个方向：没想出好的简洁的方法，干脆都写一遍
    if(dir == up){
        // 对于在这个方向上的每一列数据，独立合并
        for(j=0; j<=3; j++){
            // 拷贝所有数字块：数值大于0的
            for(i=0, k=0; i<=3; i++){
                if(grids[i][j] > 0){
                    nums[k++] = grids[i][j];
                }
            }
            // 判断是否有移动：数字块 -> 空白块
            // 有移动，拷贝的数值肯定和原来的不一样
            for(i=0; i<k; i++){
                if(nums[i] != grids[i][j]) moveOrMerge = true;
            }
            // 合并数字块
            merge();
            // 更新：将辅助数组中的非零值拷贝，然后补0
            for(k=0, i=0; k<=3; k++){
                if(nums[k] > 0) grids[i++][j] = nums[k];
            }
            while(i < 4) grids[i++][j] = 0;
        }
    }else 
    if(dir == down){
        for(j=0; j<=3; j++){
            for(i=3, k=0; i>=0; i--){
                if(grids[i][j] > 0) nums[k++] = grids[i][j];
            }
            for(i=0; i<k; i++){
                if(nums[i] != grids[3-i][j]) moveOrMerge = true;
            }
            merge();
            for(k=0, i=3; k<=3; k++){
                if(nums[k] > 0) grids[i--][j] = nums[k];
            }
            while(i >= 0) grids[i--][j] = 0;
        }
    }else 
    if(dir == left){
        for(i=0; i<=3; i++){
            for(j=0, k=0; j<=3; j++){
                if(grids[i][j] > 0) nums[k++] = grids[i][j];
            }
            for(j=0; j<k; j++){
                if(nums[j] != grids[i][j]) moveOrMerge = true;
            }
            merge();
            for(k=0, j=0; k<=3; k++){
                if(nums[k] > 0) grids[i][j++] = nums[k];
            }
            while(j < 4) grids[i][j++] = 0;
        }
    }else 
    if(dir == right){
        for(i=0; i<=3; i++){
            for(j=3, k=0; j>=0; j--){
                if(grids[i][j] > 0) nums[k++] = grids[i][j];
            }
            for(j=0; j<k; j++){
                if(nums[j] != grids[i][3-j]) moveOrMerge = true;
            }
            merge();
            for(k=0, j=3; k<=3; k++){
                if(nums[k] > 0) grids[i][j--] = nums[k];
            }
            while(j>=0) grids[i][j--] = 0;
        }
    }
    // 有效操作才算步数
    if(moveOrMerge) step++;
    return moveOrMerge;
}

/// @brief 随机生成新数字块：位置随机、数值随机
void Game::generate(){
    // 在 [0, blankNum) 中随机选择一个数 index
    // 在棋盘格子中从上到下从左到右依次找到第 index 个空白块
    // 可以以 step、maxNum 控制的概率分布，随机选择 2、4、8 数字块放在这个空白块
    // 然后更新统计数据
    if(blankNum <= 0) return;
    // cout << "Debug: blankNum=" << blankNum << endl;
    int index = getRandomIndex(blankNum);
    int x = getRandomIndex(100);
    // 分三个等级设置概率分布
    if(maxNum <= 128){
        if(x < 98) fillBlank(index, 2);
        else fillBlank(index, 4);
    }else if(maxNum <= 512){
        if(x < 90) fillBlank(index, 2);
        else if(x < 98) fillBlank(index, 4);
        else fillBlank(index, 8);
    }else{
        if(x < 80) fillBlank(index, 2);
        else if(x < 95) fillBlank(index, 4);
        else fillBlank(index, 8);
    }
    blankNum--;
}

/// @brief 填充指定的空白块
/// @param index 从上到下从左到右数，第 index 个空白块（从 0 开始）
/// @param num 数字块的数值
void Game::fillBlank(int index, int num){
    // cout << "Debug: index=" << index << endl;
    // cout << "Debug: num=" << num << endl;
    maxNum = maxNum > num ? maxNum : num;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(grids[i][j] == 0){
                if(index==0){
                    grids[i][j] = num;
                    return;
                }
                index--;
            }
        }
    }
}

/// @brief 检查是否不能再移动了
/// @return true：我还能操作！ false：欸嘿！又失败了！
bool Game::check(){
    // 有空白，肯定还能操作
    if(blankNum > 0) return true;
    // 行、列 上有相邻的相同数值，则还能合并
    for(int i=0; i<4; i++){
        for(int j=1; j<4; j++){
            if(grids[i][j-1] == grids[i][j]) return true;
            if(grids[j-1][i] == grids[j][i]) return true;
        }
    }
    return false;
}

// 游戏结束时判断游戏是否胜利：To2048
bool Game::victory(){
    return maxNum==2048;
}

// 判断游戏是否结束：不能继续移动了
bool Game::defeat(){
    return check()==false;
}

// 返回值：[0, n) 中的整数（服从均匀分布）
int Game::getRandomIndex(int n){
    // 这里若使用 C++ 的 random 库，似乎每次都是同样的随机数
    srand(time(NULL));
    return (rand() % n);
}

int Game::getStep(){
    return step;
}

int Game::getScore(){
    return score;
}

vector<vector<int> > Game::getGrids(){
    vector<vector<int> > res(4, vector<int>(4));
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            res[i][j] = grids[i][j];
        }
    }
    return res;
}

// 打印棋盘格子
void Game::print(){
    cout << "-----------Game-----------" << endl;
    for(int i=0; i<4; i++){
        cout << "|";
        for(int j=0; j<4; j++){
            if(grids[i][j]>0) cout << setw(5) << grids[i][j];
            else cout << "     ";
            cout << " |";
        }
        cout << endl;
        cout << "----------------------------" << endl;
    }
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
        if(game.defeat()){
            cout << "Game Over !!!" << endl;
            break;
        }
        if(game.victory()){
            cout << "Congratulations!" << endl;
            cout << "Step: " << game.getStep() << endl;
            cout << "Score: " << game.getScore() << endl;
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
            if(game.checkAndMove(cmds.at(dir))){
                game.generate();
            }
        }
    }
    return 0;
}
#endif // __MAIN__
