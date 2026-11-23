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
 
 struct Player
 {      
        // 纹理
        SDL_Texture* texture = nullptr;
        // 位置
        SDL_FPoint position = {0,0};
        // 宽度
        int width = 0;
        // 高度
        int height = 0;
 };
 