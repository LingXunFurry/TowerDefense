#ifndef CONFIG_H
#define CONFIG_H

namespace GameConfig {
    // 窗口设置
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const int GRID_SIZE = 40;

    // 游戏设置
    const int INITIAL_LIVES = 20;
    const int INITIAL_GOLD = 300;
    const int ENEMY_REWARD = 15;

    // 敌人设置
    const int ENEMY_HEALTH = 100;
    const float ENEMY_SPEED = 1.0f;

    // 防御塔设置
    namespace TowerStats {
        // 箭塔
        const int ARROW_DAMAGE = 20;
        const int ARROW_RANGE = 150;
        const int ARROW_COST = 100;
        const int ARROW_FIRE_RATE = 1000; // ms

        // 炮塔
        const int CANNON_DAMAGE = 50;
        const int CANNON_RANGE = 120;
        const int CANNON_COST = 200;
        const int CANNON_FIRE_RATE = 2000; // ms
    }

    // 波浪设置
    const int WAVE_ENEMY_COUNT = 10;
    const int WAVE_SPAWN_INTERVAL = 1000; // ms
}

#endif // CONFIG_H
