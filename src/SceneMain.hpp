/*
 * @Description: 主场景类
 * @Author: mjy
 * @Date: 2026-11-23 15:55:54
 * @LastEditTime: 2026-11-23 16:22:31
 * @LastEditors: mjy
 * @FilePath: \SDLShooter\src\SceneMain.hpp
 */

 #pragma once
 #include "Scene.hpp"

 class SceneMain:public Scene
 {
 private:
        
 public:
        SceneMain();
        ~SceneMain();

        void init() override;
        void update() override;
        void render() override;
        void clean() override;
        void handleEvent(SDL_Event* event) override;
 };
 

