#include "Game.hpp"
#include "SceneMain.hpp"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SceneTitle.hpp"
#include <fstream>

Game::Game()
{
}

Game::~Game()
{
        saveData();
        clean();
}

void Game::run()
{
        // 游戏主循环
        while (isRunning)
        {
                auto frameStart = SDL_GetTicks(); // 记录帧开始时间
                SDL_Event event;
                handleEvent(&event);
                update(deltaTime); // 传递deltaTime
                render();
                auto frameEnd = SDL_GetTicks();    // 记录帧结束时间
                auto diff = frameEnd - frameStart; // 计算帧处理时间
                // 帧率限制和deltaTime计算
                if (diff < frameTime)
                {
                        SDL_Delay(frameTime - diff);     // 如果处理太快，延迟一下
                        deltaTime = frameTime / 1000.0f; // 转换为秒
                }
                else
                {
                        deltaTime = diff / 1000.0f; // 如果处理较慢，使用实际时间
                }
        }
}

void Game::init()
{

        frameTime = 1000 / FPS;
        // 读取得分榜
        loadData();
        // 初始化
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
                isRunning = false;
        }
        // 创建窗口
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
                isRunning = false;
        }
        // 创建渲染器
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr)
        {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                isRunning = false;
        }
        // 设置逻辑分辨率：全屏时保持 600x800 的比例，需要时自动加黑边，而不是拉伸画面
        if (renderer != nullptr)
        {
                SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);
        }
        // 初始化SDL_image
        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                isRunning = false;
        }
        // 初始化SDL_ttf（必须在 TTF_OpenFont 之前）
        if (TTF_Init() < 0)
        {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_ttf could not initialize! TTF Error: %s\n", TTF_GetError());
                isRunning = false;
        }
        // 初始化SDL_mixer
        if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG))
        {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                isRunning = false;
        }

        // 初始化背景卷轴
        nearStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-A.png");
        SDL_QueryTexture(nearStars.texture, NULL, NULL, &nearStars.width, &nearStars.height);
        nearStars.height /= 2;
        nearStars.width /= 2;

        farStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-B.png");
        SDL_QueryTexture(farStars.texture, NULL, NULL, &farStars.width, &farStars.height);
        farStars.height /= 2;
        farStars.width /= 2;
        farStars.speed = 20; // 远处的星星移动速度较慢

        // 打开音频设备
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not open audio! SDL_mixer Error: %s\n", Mix_GetError());
                isRunning = false;
        }

        // 设置音效channel数量
        Mix_AllocateChannels(32);

        // 设置音乐音量
        Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
        Mix_Volume(-1, MIX_MAX_VOLUME / 8);

        // 载入字体（必须在 TTF_Init 之后）
        titleFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 48);
        if (titleFont == nullptr)
        {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load title font: %s", TTF_GetError());
        }
        textFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 24);
        if (textFont == nullptr)
        {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load text font: %s", TTF_GetError());
        }

        // 音频初始化完成后再创建场景（场景 init 里要加载音频资源）
        currentScene = new SceneTitle();
        currentScene->init();
}

void Game::clean()
{
        if (currentScene != nullptr)
        {
                currentScene->clean();
                delete currentScene;
        }
        if (nearStars.texture != nullptr)
        {
                SDL_DestroyTexture(nearStars.texture);
        }
        if (farStars.texture != nullptr)
        {
                SDL_DestroyTexture(farStars.texture);
        }
        // 销毁渲染器
        SDL_DestroyRenderer(renderer);
        // 清理SDL_mixer
        Mix_CloseAudio();
        Mix_Quit();
        // 清理字体和SDL_ttf
        if (titleFont != nullptr)
        {
                TTF_CloseFont(titleFont);
        }
        if (textFont != nullptr)
        {
                TTF_CloseFont(textFont);
        }
        TTF_Quit();
        // 销毁窗口
        SDL_DestroyWindow(window);
        SDL_Quit();
}

void Game::changeScene(Scene *scene)
{
        //
        if (currentScene != nullptr)
        {
                currentScene->clean();
                delete currentScene;
        }
        // 场景变化
        currentScene = scene;
        // 新场景初始化
        currentScene->init();
}

void Game::handleEvent(SDL_Event *event)
{
        while (SDL_PollEvent(event))
        {
                if (event->type == SDL_QUIT)
                {
                        isRunning = false;
                }
                // F4 切换全屏 / 窗口模式
                if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_F4)
                {
                        isFullscreen = !isFullscreen;
                        if (isFullscreen)
                        {
                                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        }
                        else
                        {
                                SDL_SetWindowFullscreen(window, 0);
                        }
                }
                currentScene->handleEvent(event);
        }
}

void Game::update(float deltaTime)
{
        backgroundUpdate(deltaTime);
        currentScene->update(deltaTime);
}

void Game::render()
{
        // 清空
        SDL_RenderClear(renderer);
        // 渲染星空背景
        renderBackground();

        currentScene->render();
        // 显示更新
        SDL_RenderPresent(renderer);
}

// Game.cpp
void Game::backgroundUpdate(float deltaTime)
{
        nearStars.offset += nearStars.speed * deltaTime;
        if (nearStars.offset >= 0)
        {
                nearStars.offset -= nearStars.height;
        }

        farStars.offset += farStars.speed * deltaTime;
        if (farStars.offset >= 0)
        {
                farStars.offset -= farStars.height;
        }
}

// Game.cpp
void Game::renderBackground()
{
        // 渲染远处的星星
        for (int posY = static_cast<int>(farStars.offset); posY < getWindowHeight(); posY += farStars.height)
        {
                for (int posX = 0; posX < getWindowWidth(); posX += farStars.width)
                {
                        SDL_Rect ds = {posX, posY, farStars.width, farStars.height};
                        SDL_RenderCopy(renderer, farStars.texture, NULL, &ds);
                }
        }
        // 渲染近处的星星
        for (int posY = static_cast<int>(nearStars.offset); posY < getWindowHeight(); posY += nearStars.height)
        {
                for (int posX = 0; posX < getWindowWidth(); posX += nearStars.width)
                {
                        SDL_Rect dstRect = {posX, posY, nearStars.width, nearStars.height};
                        SDL_RenderCopy(renderer, nearStars.texture, nullptr, &dstRect);
                }
        }
}



SDL_Point Game::renderTextCentered(std::string text, float posY, bool isTitle)
{
    SDL_Color color = {255, 255, 255, 255};
    TTF_Font *font = isTitle ? titleFont : textFont;
    if (font == nullptr)
    {
        return {0, 0};
    }
    SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    if (surface == nullptr)
    {
        return {0, 0};
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        SDL_FreeSurface(surface);
        return {0, 0};
    }
    int y = static_cast<int>((getWindowHeight() - surface->h) * posY);
    SDL_Rect rect = {getWindowWidth() / 2 - surface->w / 2,
                     y,
                     surface->w,
                     surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    // 返回文字末尾位置，方便在文字后面画光标
    return {rect.x + rect.w, y};
}

void Game::renderTextPos(std::string text, int posX, int posY, bool isLeft)
{
    SDL_Color color = {255, 255, 255, 255};
    if (textFont == nullptr)
    {
        return;
    }
    SDL_Surface *surface = TTF_RenderUTF8_Solid(textFont, text.c_str(), color);
    if (surface == nullptr)
    {
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        SDL_FreeSurface(surface);
        return;
    }
    SDL_Rect rect;
    if (isLeft)
    {
        rect = {posX, posY, surface->w, surface->h};
    }
    else
    {
        rect = {getWindowWidth() - posX - surface->w, posY, surface->w, surface->h};
    }
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Game::insertLeaderBoard(int score, std::string name)
{
    leaderBoard.insert({score, name});
    // 只保留前 8 名
    while (leaderBoard.size() > 8)
    {
        leaderBoard.erase(--leaderBoard.end());
    }
    saveData();
}

void Game::saveData()
{
    std::ofstream file("assets/save.dat");
    if (!file.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open save file for writing");
        return;
    }
    for (const auto &entry : leaderBoard)
    {
        file << entry.first << " " << entry.second << std::endl;
    }
}

void Game::loadData()
{
    std::ifstream file("assets/save.dat");
    if (!file.is_open())
    {
        SDL_Log("No save file found, leaderboard is empty");
        return;
    }
    leaderBoard.clear();
    int score;
    std::string name;
    while (file >> score >> name)
    {
        leaderBoard.insert({score, name});
    }
}