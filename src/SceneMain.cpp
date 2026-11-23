#include "SceneMain.hpp"
#include <SDL_image.h>

SceneMain::SceneMain() : game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::update(float deltaTime)
{
        keyboardControl(deltaTime);     
        updatePlayerProjectiles(deltaTime);     //更新子弹
        spawEnemy();              // 生成敌机
        updateEnemies(deltaTime); // 更新敌机
}

void SceneMain::render()
{
        // 渲染子弹
        renderPlayerProjectiles();
        // 渲染敌机
        renderEnemies();
        // 玩家的渲染矩形
        SDL_Rect playerRect = {static_cast<int>(player.position.x),
                               static_cast<int>(player.position.y),
                               player.width,
                               player.height};
        SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
}

void SceneMain::handleEvent(SDL_Event *event)
{
}

void SceneMain::init()
{
        // 初始化随机数生成器
        std::random_device rd;                                   // 获取真随机数作为种子
        gen = std::mt19937(rd());                                // 用种子初始化梅森旋转引擎
        dis = std::uniform_real_distribution<float>(0.0f, 1.0f); // 设置[0,1]均匀分布

        // 加载玩家纹理
        player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
        if (player.texture == nullptr)
        {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player texture: %s", SDL_GetError());
        }
        // 修改纹理的属性
        SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
        player.width /= 4;
        player.height /= 4;
        player.position.x = game.getWindowWidth() / 2 - player.width / 2;
        player.position.y = game.getWindowHeight() - player.height;

        // 初始化子弹模板
        projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
        SDL_QueryTexture(projectilePlayerTemplate.texture, NULL, NULL, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
        projectilePlayerTemplate.width /= 4;
        projectilePlayerTemplate.height /= 4;

        // 初始化敌机模板
        enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-2.png");
        SDL_QueryTexture(enemyTemplate.texture, NULL, NULL, &enemyTemplate.width, &enemyTemplate.height);
        enemyTemplate.width /= 4;
        enemyTemplate.height /= 4;
}

void SceneMain::clean()
{

        // 清理子弹
        for (auto &projectile : projectilesPlayer)
        {
                if (projectile != nullptr)
                {
                        delete projectile;
                }
        }
        projectilesPlayer.clear();

        // 清理敌机
        for (auto &enemy : enemies)
        {
                if (enemy != nullptr)
                {
                        delete enemy;
                }
        }
        enemies.clear();

        // 清理纹理
        if (player.texture != nullptr)
        {
                SDL_DestroyTexture(player.texture);
        }
        if (projectilePlayerTemplate.texture != nullptr)
        {
                SDL_DestroyTexture(projectilePlayerTemplate.texture);
        }
        if (enemyTemplate.texture != nullptr)
        {
                SDL_DestroyTexture(enemyTemplate.texture);
        }
}

void SceneMain::keyboardControl(float deltaTime)
{
        // 获取键盘状态
        auto keyboardState = SDL_GetKeyboardState(NULL);

        if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
        {
                player.position.y -= player.speed * deltaTime;
        }
        if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN])
        {
                player.position.y += player.speed * deltaTime;
        }
        if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT])
        {
                player.position.x -= player.speed * deltaTime;
        }
        if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT])
        {
                player.position.x += player.speed * deltaTime;
        }

        // 限制飞机的移动范围
        if (player.position.x < 0)
        {
                player.position.x = 0;
        }
        if (player.position.x > game.getWindowWidth() - player.width)
        {
                player.position.x = game.getWindowWidth() - player.width;
        }
        if (player.position.y < 0)
        {
                player.position.y = 0;
        }
        if (player.position.y > game.getWindowHeight() - player.height)
        {
                player.position.y = game.getWindowHeight() - player.height;
        }

        // 控制子弹发射
        if (keyboardState[SDL_SCANCODE_J])
        {
                auto currentTime = SDL_GetTicks();
                if (currentTime - player.lastShootTime > player.coolDown)
                {
                        shootPlayer();
                        player.lastShootTime = currentTime;
                }
        }
}

void SceneMain::shootPlayer()
{
        // 创建新子弹
        auto projectile = new ProjectilePlayer(projectilePlayerTemplate);
        // 定位在飞机顶部中央
        projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
        projectile->position.y = player.position.y;
        // 添加到活动子弹列表
        projectilesPlayer.push_back(projectile);
}

void SceneMain::updatePlayerProjectiles(float deltaTime)
{
        int margin = 32; // 子弹超出屏幕外边界的距离
        for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();)
        {
                auto projectile = *it;
                // 更新子弹位置
                projectile->position.y -= projectile->speed * deltaTime;
                // 检查子弹是否超出屏幕
                if (projectile->position.y + margin < 0)
                {
                        delete projectile;
                        it = projectilesPlayer.erase(it);
                }
                else
                {
                        ++it;
                }
        }
}

void SceneMain::renderPlayerProjectiles()
{
        for (auto projectile : projectilesPlayer)
        {
                SDL_Rect projectileRect = {
                    static_cast<int>(projectile->position.x),
                    static_cast<int>(projectile->position.y),
                    projectile->width,
                    projectile->height};
                SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
        }
}

void SceneMain::spawEnemy()
{
        // 利用随机数决定是否生成敌机（每帧约1/60的概率）
        if (dis(gen) > 1 / 60.0f)
        {
                return;
        }

        // 创建新敌机
        Enemy *enemy = new Enemy(enemyTemplate);

        // 利用随机数决定敌机的水平位置
        enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);

        // 垂直位置设在屏幕上方（刚好不可见）
        enemy->position.y = -enemy->height;

        // 添加到活动敌机列表
        enemies.push_back(enemy);
}

void SceneMain::updateEnemies(float deltaTime)
{
        for (auto it = enemies.begin(); it != enemies.end();)
        {
                auto enemy = *it;

                // 更新敌机位置
                enemy->position.y += enemy->speed * deltaTime;

                // 检查敌机是否飞出屏幕底部
                if (enemy->position.y > game.getWindowHeight())
                {
                        delete enemy;
                        it = enemies.erase(it);
                }
                else
                {
                        ++it;
                }
        }
}

void SceneMain::renderEnemies()
{
        for (auto enemy : enemies)
        {
                SDL_Rect enemyRect = {
                    static_cast<int>(enemy->position.x),
                    static_cast<int>(enemy->position.y),
                    enemy->width,
                    enemy->height};
                SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);
        }
}