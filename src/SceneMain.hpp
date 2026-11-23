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
#include <random>

class SceneMain : public Scene
{
private:
       Game &game;
       Player player;
       bool isDead = false;                 // 玩家是否死亡
       // 随机数相关成员
       std::mt19937 gen;                          // 随机数生成器
       std::uniform_real_distribution<float> dis; // 随机数分布器

       // 敌机相关成员
       Enemy enemyTemplate;        // 敌机模板
       std::list<Enemy *> enemies; // 存储活动敌机的列表

       // 创建子弹模板
       ProjectilePlayer projectilePlayerTemplate;
       // 存储活动子弹的列表
       std::list<ProjectilePlayer *> projectilesPlayer;

       ProjectileEnemy projectileEnemyTemplate;       // 敌机子弹模板
       std::list<ProjectileEnemy *> projectilesEnemy; // 敌机子弹容器

       // 处理键盘控制
       void keyboardControl(float deltaTime);
       // 射击子弹
       void shootPlayer();
       // 更新子弹
       void updatePlayerProjectiles(float deltaTime); // 更新子弹
       // 渲染子弹
       void renderPlayerProjectiles();
       // 生成敌机
       void spawEnemy();
       // 更新敌人
       void updateEnemies(float deltaTime);
       // 渲染敌人
       void renderEnemies();
       // 敌机发射子弹
       void shootEnemy(Enemy *enemy);
       // 更新敌机子弹
       void updateEnemyProjectiles(float deltaTime);
       // 渲染敌机子弹
       void renderEnemyProjectiles();
       // 计算子弹方向
       SDL_FPoint getDirection(Enemy *enemy);
       // 更新玩家状态
       void updatePlayer(float deltaTime);
       // 敌机爆炸效果
       void enemyExplode(Enemy *enemy);

public:
       SceneMain();
       ~SceneMain();

       void init() override;
       void update(float deltaTime) override;
       void render() override;
       void clean() override;
       void handleEvent(SDL_Event *event) override;
};
