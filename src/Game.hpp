/*
 * @Description: 游戏主类
 * @Author: mjy
 * @Date: 2026-11-23 15:53:00
 * @LastEditTime: 2026-11-23 17:17:51
 * @LastEditors: mjy
 * @FilePath: \SDLShooter\src\Game.hpp
 */

#pragma once
#include "Scene.hpp"
#include <SDL_mixer.h>
#include "Object.hpp"
#include <SDL_ttf.h>
#include <string>
#include <map>
#include <functional>

class Game
{
private:
       Game();
       // 删除拷贝构造和拷贝赋值
       Game(const Game &);
       Game &operator=(const Game &) = delete;
       // 游戏是否运行
       bool isRunning = true;
       // 指向当前活动场景的指针
       Scene *currentScene = nullptr;
       // 游戏窗口
       SDL_Window *window = nullptr;
       // 是否处于全屏模式
       bool isFullscreen = false;
       // 游戏渲染器
       SDL_Renderer *renderer = nullptr;
       // 游戏窗口宽度
       int windowWidth = 600;
       // 游戏窗口长度
       int windowHeight = 800;
       // 目标帧率
       int FPS = 60;
       // 每帧的目标时间（毫秒）
       Uint32 frameTime;
       // 两帧之间的时间差（秒）
       float deltaTime;
       Background nearStars; // 近处的星星
       Background farStars;  // 远处的星星

       TTF_Font *titleFont = nullptr;
       TTF_Font *textFont = nullptr;

       // 玩家最终得分
       int finalScore = 0;
       // 得分榜（分数从高到低）
       std::multimap<int, std::string, std::greater<int>> leaderBoard;

       // 保存/读取得分榜
       void saveData();
       void loadData();

public:
       static Game &getInstance()
       {
              static Game instance;
              return instance;
       }
       ~Game();
       // 运行游戏主循环
       void run();
       // 游戏初始化
       void init();
       // 清理游戏资源
       void clean();
       // 切换当前场景
       void changeScene(Scene *scene);
       //  事件处理
       void handleEvent(SDL_Event *event);
       //
       void update(float deltaTime);
       void render();
       // 背景更新
       void backgroundUpdate(float deltaTime);
       // 背景渲染
       void renderBackground();

       // 渲染居中文字，返回文字末尾位置（用于绘制光标）
       SDL_Point renderTextCentered(std::string text, float posY, bool isTitle);
       // 在指定位置渲染文字，isLeft 为 true 表示左对齐，false 表示右对齐
       void renderTextPos(std::string text, int posX, int posY, bool isLeft = true);

       // 插入得分榜（只保留前 8 名）
       void insertLeaderBoard(int score, std::string name);
       // 获取得分榜
       std::multimap<int, std::string, std::greater<int>> &getLeaderBoard() { return leaderBoard; }

       void setFinalScore(int score) { finalScore = score; }
       int getFinalScore() { return finalScore; }

       SDL_Window *getWindow() { return window; }
       SDL_Renderer *getRenderer() { return renderer; }
       int getWindowWidth() { return windowWidth; }
       int getWindowHeight() { return windowHeight; }
};
