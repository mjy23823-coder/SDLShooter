// SceneTitle.h
#pragma once

#include "Scene.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class SceneTitle : public Scene
{
public:
    SceneTitle();
    virtual void init() override;
    virtual void update(float deltaTime) override;
    virtual void render() override;
    virtual void clean() override;
    virtual void handleEvent(SDL_Event* event) override;

private:
    Mix_Music* bgm = nullptr; // 背景音乐
    float timer = 0.0f; // 计时器
};