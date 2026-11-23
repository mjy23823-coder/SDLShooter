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
#include <list>

class SceneMain : public Scene
{
private:
       Game &game;
       Player player;

       // 创建子弹模板
       ProjectilePlayer projectilePlayerTemplate;
       // 存储活动子弹的列表
       std::list<ProjectilePlayer *> projectilesPlayer;

       // 处理键盘控制
       void keyboardControl(float deltaTime);
       // 射击子弹
       void shootPlayer();
       // 更新子弹
       void updatePlayerProjectiles(float deltaTime); // 更新子弹
       // 渲染子弹
       void renderPlayerProjectiles();

public:
       SceneMain();
       ~SceneMain();

       void init() override;
       void update(float deltaTime) override;
       void render() override;
       void clean() override;
       void handleEvent(SDL_Event *event) override;
};
