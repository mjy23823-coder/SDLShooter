/*
 * @Description: 定义游戏中的各种对象
 * @Author: mjy
 * @Date: 2026-11-23 17:07:07
 * @LastEditTime: 2026-11-23 17:18:45
 * @LastEditors: mjy
 * @FilePath: \SDLShooter\src\Object.hpp
 */

#pragma once
#include <SDL.h>

//  玩家
struct Player
{
      // 纹理
      SDL_Texture *texture = nullptr;
      // 位置
      SDL_FPoint position = {0, 0};
      // 宽度
      int width = 0;
      // 高度
      int height = 0;
      //  速度
      int speed = 200;
      // 玩家初始生命值为3
      int currentHealth = 3;
      //  子弹射击冷却时间
      Uint32 coolDown = 500;
      // 子弹上次射击时间
      Uint32 lastShootTime = 0;
};

// 子弹
struct ProjectilePlayer
{
      SDL_Texture *texture = nullptr;
      SDL_FPoint position = {0, 0};
      int width = 0;
      int height = 0;
      int speed = 400;
      int damage = 1; // 玩家子弹伤害值为1
};

// 敌机
struct Enemy
{
      SDL_Texture *texture = nullptr;
      SDL_FPoint position = {0, 0};
      int width = 0;
      int height = 0;
      int speed = 400;
      int currentHealth = 2;    // 敌机初始生命值为2
      Uint32 coolDown = 1000;   // 发射冷却时间（1秒）
      Uint32 lastShootTime = 0; // 上次发射时间
};

// 敌机子弹
struct ProjectileEnemy
{
      SDL_Texture *texture = nullptr;
      SDL_FPoint position = {0, 0};
      SDL_FPoint direction = {0, 0}; // 子弹方向向量
      int width = 0;
      int height = 0;
      int speed = 400;
      int damage = 1; // 敌机子弹伤害值为1
};
