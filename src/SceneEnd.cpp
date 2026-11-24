#include "SceneEnd.hpp"
#include "Game.hpp"
#include "SceneMain.hpp"
#include <string>

SceneEnd::SceneEnd() : Scene(Game::getInstance())
{
}

void SceneEnd::init()
{
    // 载入并播放背景音乐
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if (bgm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load music: %s", Mix_GetError());
    }
    else
    {
        Mix_PlayMusic(bgm, -1);
    }

    // 开启文本输入，才能收到 SDL_TEXTINPUT 事件（支持中文输入法）
    if (!SDL_IsTextInputActive())
    {
        SDL_StartTextInput();
    }
    if (!SDL_IsTextInputActive())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to start text input: %s", SDL_GetError());
    }
}

void SceneEnd::update(float deltaTime)
{
    blinkTimer -= deltaTime;
    if (blinkTimer <= 0)
    {
        blinkTimer += 1.0f;
    }
}

void SceneEnd::render()
{
    if (isTyping)
    {
        renderPhase1();
    }
    else
    {
        renderPhase2();
    }
}

void SceneEnd::clean()
{
    if (SDL_IsTextInputActive())
    {
        SDL_StopTextInput();
    }
    // 停止并释放背景音乐
    if (bgm != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }
}

void SceneEnd::handleEvent(SDL_Event *event)
{
    if (isTyping)
    {
        if (event->type == SDL_TEXTINPUT)
        {
            name += event->text.text;
        }
        else if (event->type == SDL_KEYDOWN)
        {
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN)
            {
                isTyping = false;
                SDL_StopTextInput();
                if (name.empty())
                {
                    name = "无名玩家";
                }
                // 把本次成绩加入得分榜
                game.insertLeaderBoard(game.getFinalScore(), name);
            }
            else if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
            {
                removeLastUTF8Char(name);
            }
        }
    }
    else
    {
        if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_J)
        {
            game.changeScene(new SceneMain());
        }
    }
}

void SceneEnd::renderPhase1()
{
    auto score = game.getFinalScore();
    std::string scoreText = "你的得分是：" + std::to_string(score);
    std::string gameOver = "Game Over";
    std::string instructionText = "请输入你的名字，按回车确认";

    game.renderTextCentered(scoreText, 0.1f, false);
    game.renderTextCentered(gameOver, 0.4f, true);
    game.renderTextCentered(instructionText, 0.6f, false);

    if (!name.empty())
    {
        SDL_Point p = game.renderTextCentered(name, 0.8f, false);
        if (blinkTimer < 0.5f)
        {
            game.renderTextPos("_", p.x, p.y);
        }
    }
    else
    {
        if (blinkTimer < 0.5f)
        {
            game.renderTextCentered("_", 0.8f, false);
        }
    }
}

void SceneEnd::renderPhase2()
{
    game.renderTextCentered("得分榜", 0.05f, true);

    int posY = static_cast<int>(0.2f * game.getWindowHeight());
    int i = 1;
    for (const auto &item : game.getLeaderBoard())
    {
        std::string rankName = std::to_string(i) + ". " + item.second;
        std::string scoreText = std::to_string(item.first);
        game.renderTextPos(rankName, 100, posY, true);
        game.renderTextPos(scoreText, 100, posY, false);
        posY += 45;
        ++i;
    }

    if (blinkTimer < 0.5f)
    {
        game.renderTextCentered("按 J 键重新开始游戏", 0.85f, false);
    }
}

void SceneEnd::removeLastUTF8Char(std::string &str)
{
    if (str.empty())
    {
        return;
    }
    // UTF-8 多字节字符的后继字节都以 10 开头
    auto lastChar = static_cast<unsigned char>(str.back());
    if ((lastChar & 0b10000000) == 0b10000000)
    {
        str.pop_back();
        while (!str.empty() && (static_cast<unsigned char>(str.back()) & 0b11000000) != 0b11000000)
        {
            str.pop_back();
        }
    }
    if (!str.empty())
    {
        str.pop_back();
    }
}
