/*
 * @Description: 场景基类
 * @Author: mjy
 * @Date: 2026-11-23 15:55:54
 * @LastEditTime: 2026-11-23 16:27:42
 * @LastEditors: mjy
 * @FilePath: \SDLShooter\src\Scene.hpp
 */
 #pragma once

 #include<SDL.h>

 // 前向声明，避免和 Game.hpp 循环包含
 class Game;

 class Scene{
 protected:
        Game& game;

 public:
        explicit Scene(Game& game) : game(game) {}
        virtual ~Scene() = default;

        // 初始化场景
        virtual void init() = 0;
        // 更新场景渲染
        virtual void update(float deltaTime) = 0;
        // 渲染场景
        virtual void render() = 0;
        // 清理场景资源
        virtual void clean () = 0;
        // 处理输入事件
        virtual void handleEvent(SDL_Event* event) = 0;
 };
 

 