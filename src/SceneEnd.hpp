#pragma once
#include "Scene.hpp"
#include <SDL_mixer.h>
#include <string>

class SceneEnd : public Scene
{
public:
    SceneEnd();

    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    void handleEvent(SDL_Event *event) override;

private:
    bool isTyping = true;      // 是否处于输入名字阶段
    std::string name;          // 玩家输入的名字
    float blinkTimer = 1.0f;   // 光标闪烁计时器
    Mix_Music *bgm = nullptr;  // 背景音乐

    // 渲染输入名字阶段
    void renderPhase1();
    // 渲染得分榜阶段
    void renderPhase2();
    // 正确删除最后一个 UTF-8 字符
    void removeLastUTF8Char(std::string &str);
};
