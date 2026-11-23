/*
 * @Description: 游戏主类
 * @Author: mjy
 * @Date: 2026-11-23 15:53:00
 * @LastEditTime: 2026-11-23 16:50:28
 * @LastEditors: mjy
 * @FilePath: \SDLShooter\src\Game.hpp
 */

#pragma once
#include "Scene.hpp"

class Game
{
private:
       // 游戏是否运行
       bool isRunning = true;
       // 指向当前活动场景的指针
       Scene *currentScene = nullptr;
       // 游戏窗口
       SDL_Window *window = nullptr;
       // 游戏渲染器
       SDL_Renderer *renderer = nullptr;
       // 游戏窗口宽度
       int windowWidth = 600;
       // 游戏窗口长度
       int windowHeight = 800;

public:
       Game();
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
       void update();
       void render();
};
