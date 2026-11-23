#include "Game.hpp"

Game::Game()
{

}

Game::~Game()
{

}

void Game::run(){
        // 游戏主循环
        while (isRunning){
                SDL_Event event;
                currentScene->handleEvent(&event);
                currentScene->update();
                currentScene->render();
        }
}

void Game::init()
{

}

void Game::clean(){

}

void Game::changeScene(Scene* scene){
        // 
        if(currentScene!=nullptr){
                currentScene->clean();
                delete currentScene;
        }
        // 场景变化
        currentScene = scene;
        // 新场景初始化
        currentScene->init();
}