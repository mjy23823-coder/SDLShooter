/*
 * @Description: 主场景类
 * @Author: mjy
 * @Date: 2026-11-23 15:55:54
 * @LastEditTime: 2026-11-23 17:36:15
 * @LastEditors: mjy
 * @FilePath: \SDLShooter\src\SceneMain.hpp
 */

#pragma once
#include "Scene.hpp"
#include "Object.hpp"
#include "Game.hpp"

class SceneMain : public Scene
{
private:
       Game &game;
       Player player;

public:
       SceneMain();
       ~SceneMain();

       void init() override;
       void update() override;
       void render() override;
       void clean() override;
       void handleEvent(SDL_Event *event) override;
};
