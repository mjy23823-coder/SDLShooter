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
#include <map>
#include <SDL_ttf.h>
class SceneMain : public Scene
{
private:
       Game &game;
       Player player;
       bool isDead = false; // 玩家是否死亡
       SDL_Texture *uiHealth = nullptr;
       TTF_Font *scoreFont = nullptr;
       int score = 0;

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

       Explosion explosionTemplate;       // 爆炸动画模板
       std::list<Explosion *> explosions; // 存储活动爆炸动画的列表

       Item itemLifeTemplate;   // 物品模板
       std::list<Item *> items; // 物品容器

       Mix_Music *bgm;
       std::map<std::string, Mix_Chunk *> sounds; // 存储音效

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
       // 更新爆炸动画
       void updateExplosions(float deltaTime);
       // 渲染爆炸动画
       void renderExplosions();
       // 玩家爆炸
       void playerExplode();
       // 物品生成
       void dropItem(Enemy *enemy);
       // 更新物品
       void updateItems(float deltaTime);
       // 玩家获取物品
       void playerGetItem(Item *item);
       // 渲染物品
       void renderItems();
       // 渲染UI
       void renderUI();

public:
       SceneMain();
       ~SceneMain();

       void init() override;
       void update(float deltaTime) override;
       void render() override;
       void clean() override;
       void handleEvent(SDL_Event *event) override;
};
