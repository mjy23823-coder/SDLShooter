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
       // 游戏渲染器
       SDL_Renderer *renderer = nullptr;
       // 游戏窗口宽度
       int windowWidth = 600;
       // 游戏窗口长度
       int windowHeight = 800;

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
       void update();
       void render();

       SDL_Window *getWindow() { return window; }
       SDL_Renderer *getRenderer() { return renderer; }
       int getWindowWidth() { return windowWidth; }
       int getWindowHeight() { return windowHeight; }
};
