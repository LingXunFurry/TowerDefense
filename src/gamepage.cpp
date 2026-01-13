#include "include/gamepage.h"
#include "include/resourcemanager.h"
#include "include/config.h"
#include "include/mainwindow.h"

#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QMessageBox>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <cmath>

GamePage::GamePage(QWidget *parent)
    : QWidget(parent), gameScene(nullptr), gameView(nullptr), gold(GameConfig::INITIAL_GOLD), lives(GameConfig::INITIAL_LIVES), currentWave(1), enemiesSpawnedThisWave(0), isPaused(false), gameRunning(false)
{
    qDebug() << "GamePage constructor called";

    // 设置固定大小
    setFixedSize(800, 600);

    initUI();
    initGameScene();
    createPath();

    qDebug() << "GamePage initialized, size:" << size();
}

GamePage::~GamePage()
{
    resetGame();
}

void GamePage::initUI()
{
    qDebug() << "Initializing GamePage UI";

    // 设置主页面大小
    setFixedSize(800, 600);

    // 主布局
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建控制面板 - 作为浮动面板，不添加到布局中
    controlPanel = new QWidget(this);
    controlPanel->setFixedSize(800, 120);
    controlPanel->setGeometry(0, 80, 800, 120); // 设置位置为顶部

    // 控制面板提升到最前面
    controlPanel->raise();

    // 使用绝对定位而不是布局
    // 移除之前的布局设置

    QFont infoFont("Microsoft YaHei", 12, QFont::Bold);
    QFont numberFont("Microsoft YaHei", 24, QFont::Bold);
    QFont titleFont("Microsoft YaHei", 16, QFont::Normal);

    // ==================== 金币信息 ====================
    // 金币标题（图标/文字）
    QLabel *goldTitle = new QLabel("💰 金币", controlPanel);
    goldTitle->setGeometry(80, 0, 120, 40);
    goldTitle->setFont(titleFont);
    goldTitle->setStyleSheet("color: #FFD700;");
    goldTitle->setAlignment(Qt::AlignCenter);

    // 金币数量
    goldLabel = new QLabel(QString("%1").arg(gold), controlPanel);
    goldLabel->setGeometry(80, 40, 120, 40);
    goldLabel->setFont(numberFont);
    goldLabel->setStyleSheet("color: #FFD700; font-weight: bold;");
    goldLabel->setAlignment(Qt::AlignCenter);

    // ==================== 生命信息 ====================
    // 生命标题（图标/文字）
    QLabel *lifeTitle = new QLabel("❤️ 生命", controlPanel);
    lifeTitle->setGeometry(240, 0, 120, 40);
    lifeTitle->setFont(titleFont);
    lifeTitle->setStyleSheet("color: #FF4444;");
    lifeTitle->setAlignment(Qt::AlignCenter);

    // 生命数量
    livesLabel = new QLabel(QString("%1").arg(lives), controlPanel);
    livesLabel->setGeometry(240, 40, 120, 40);
    livesLabel->setFont(numberFont);
    livesLabel->setStyleSheet("color: #FF4444; font-weight: bold;");
    livesLabel->setAlignment(Qt::AlignCenter);

    // ==================== 波次信息 ====================
    // 波次标题（图标/文字）
    QLabel *waveTitle = new QLabel("🌊 波次", controlPanel);
    waveTitle->setGeometry(400, 0, 120, 40);
    waveTitle->setFont(titleFont);
    waveTitle->setStyleSheet("color: #44AAFF;");
    waveTitle->setAlignment(Qt::AlignCenter);

    // 波次数值
    waveLabel = new QLabel(QString("第 %1 波").arg(currentWave), controlPanel);
    waveLabel->setGeometry(400, 40, 120, 40);
    waveLabel->setFont(numberFont);
    waveLabel->setStyleSheet("color: #44AAFF; font-weight: bold;");
    waveLabel->setAlignment(Qt::AlignCenter);

    // ==================== 右侧按钮区域 ====================

    // 暂停按钮
    pauseButton = new QPushButton("⏸️ 暂停", controlPanel);
    pauseButton->setGeometry(600, 60, 120, 40);
    pauseButton->setStyleSheet(
        "QPushButton {"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   color: white;"
        "   background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3498db, stop:1 #2980b9);"
        "   border: 2px solid #1f618d;"
        "   border-radius: 8px;"
        "   padding: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2980b9, stop:1 #3498db);"
        "   border: 2px solid #154360;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #21618c;"
        "   border: 2px solid #0e3a5e;"
        "}");
    connect(pauseButton, &QPushButton::clicked, this, &GamePage::pauseGame);

    // 返回按钮
    returnButton = new QPushButton("🏠 返回菜单", controlPanel);
    returnButton->setGeometry(600, 0, 120, 40);
    returnButton->setStyleSheet(
        "QPushButton {"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   color: white;"
        "   background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7f8c8d, stop:1 #616a6b);"
        "   border: 2px solid #424949;"
        "   border-radius: 8px;"
        "   padding: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #616a6b, stop:1 #7f8c8d);"
        "   border: 2px solid #2c3e50;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #515a5a;"
        "   border: 2px solid #1c2833;"
        "}");
    connect(returnButton, &QPushButton::clicked, this, &GamePage::returnToMainMenu);

    qDebug() << "Control panel created";
}

void GamePage::initGameScene()
{
    qDebug() << "Initializing game scene";

    // 创建游戏场景 - 占据整个800x600
    gameScene = new QGraphicsScene(this);
    gameScene->setSceneRect(0, 0, 800, 600);

    // 创建视图 - 占据整个GamePage
    gameView = new QGraphicsView(gameScene, this);
    gameView->setRenderHint(QPainter::Antialiasing);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setFixedSize(800, 600);
    gameView->setFrameShape(QFrame::NoFrame);

    // 绘制背景
    drawBackground();
    drawGrid();

    // 添加到主布局
    mainLayout->addWidget(gameView);

    // 将控制面板提升到最前面（在gameView添加后）
    controlPanel->raise();

    // 创建定时器
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GamePage::updateGame);

    enemySpawnTimer = new QTimer(this);
    connect(enemySpawnTimer, &QTimer::timeout, this, &GamePage::spawnEnemy);

    qDebug() << "Game scene initialized, view size:" << gameView->size();
}

void GamePage::drawBackground()
{
    ResourceManager &rm = ResourceManager::instance();
    QPixmap background = rm.getGameMap();

    // 创建背景图形项 - 占据整个600x600
    QGraphicsPixmapItem *backgroundItem = new QGraphicsPixmapItem(background);
    backgroundItem->setZValue(-100); // 最底层
    gameScene->addItem(backgroundItem);

    // 绘制路径
    if (!pathPoints.isEmpty())
    {
        QPainterPath path;
        path.moveTo(pathPoints.first());

        for (int i = 1; i < pathPoints.size(); ++i)
        {
            path.lineTo(pathPoints[i]);
        }

        QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
        pathItem->setPen(QPen(QColor(139, 69, 19, 150), 30)); // 棕色半透明路径
        pathItem->setZValue(-50);
        gameScene->addItem(pathItem);
    }
}

void GamePage::drawGrid()
{
    QPen gridPen(QColor(200, 255, 200, 100), 1);

    // 绘制垂直线
    for (int x = 0; x <= 800; x += GameConfig::GRID_SIZE)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem(x, 0, x, 600);
        line->setPen(gridPen);
        line->setZValue(-90);
        gameScene->addItem(line);
    }

    // 绘制水平线
    for (int y = 0; y <= 600; y += GameConfig::GRID_SIZE)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem(0, y, 800, y);
        line->setPen(gridPen);
        line->setZValue(-90);
        gameScene->addItem(line);
    }
}

void GamePage::createPath()
{
    // 创建简单的路径
    pathPoints.clear();
    pathPoints << QPointF(660 - 20, 260 - 20)
               << QPointF(660 - 20, 300 - 20)
               << QPointF(460 - 20, 300 - 20)
               << QPointF(460 - 20, 260 - 20)
               << QPointF(180 - 20, 260 - 20)
               << QPointF(180 - 20, 300 - 20)
               << QPointF(300 - 20, 300 - 20)
               << QPointF(300 - 20, 460 - 20)
               << QPointF(380 - 20, 460 - 20)
               << QPointF(380 - 20, 380 - 20)
               << QPointF(660 - 20, 380 - 20);
}

void GamePage::startGame()
{
    if (gameRunning)
        return;

    gameRunning = true;
    isPaused = false;
    pauseButton->setText("暂停");

    // 启动游戏循环
    gameTimer->start(16); // 约60FPS

    // 启动敌人生成器
    enemySpawnTimer->start(2000); // 每2秒生成一个敌人

    qDebug() << "游戏开始!";
}

void GamePage::pauseGame()
{
    if (!gameRunning)
        return;

    if (isPaused)
    {
        // 恢复游戏
        gameTimer->start(16);
        enemySpawnTimer->start(2000);
        pauseButton->setText("暂停");
        isPaused = false;
        qDebug() << "游戏继续";
    }
    else
    {
        // 暂停游戏
        gameTimer->stop();
        enemySpawnTimer->stop();
        pauseButton->setText("继续");
        isPaused = true;
        qDebug() << "游戏暂停";
    }
}

void GamePage::resetGame()
{
    // 停止所有定时器
    if (gameTimer && gameTimer->isActive())
        gameTimer->stop();
    if (enemySpawnTimer && enemySpawnTimer->isActive())
        enemySpawnTimer->stop();

    // 清理所有敌人
    for (Enemy *enemy : enemies)
    {
        if (enemy)
        {
            gameScene->removeItem(enemy);
            enemy->deleteLater();
        }
    }
    enemies.clear();

    // 清理所有防御塔
    for (Tower *tower : towers)
    {
        if (tower)
        {
            gameScene->removeItem(tower);
            tower->deleteLater();
        }
    }
    towers.clear();

    // 重置游戏状态
    gold = GameConfig::INITIAL_GOLD;
    lives = GameConfig::INITIAL_LIVES;
    currentWave = 1;
    enemiesSpawnedThisWave = 0;
    gameRunning = false;
    isPaused = false;

    // 更新UI显示
    goldLabel->setText(QString::number(gold));
    livesLabel->setText(QString::number(lives));
    waveLabel->setText(QString("第 %1 波").arg(currentWave));

    qDebug() << "游戏重置";
}

void GamePage::spawnEnemy()
{
    if (!gameRunning || isPaused)
        return;

    // 检查当前波次是否已经生成足够敌人
    if (enemiesSpawnedThisWave >= GameConfig::WAVE_ENEMY_COUNT)
    {
        enemySpawnTimer->stop();
        qDebug() << "第" << currentWave << "波敌人已全部生成";

        // 等待所有敌人被消灭或到达终点
        if (enemies.isEmpty())
        {
            // 进入下一波
            currentWave++;
            enemiesSpawnedThisWave = 0;
            waveLabel->setText(QString("第 %1 波").arg(currentWave));
            enemySpawnTimer->start(GameConfig::WAVE_SPAWN_INTERVAL);
            qDebug() << "进入第" << currentWave << "波";
        }
        return;
    }

    // 创建新敌人
    Enemy *enemy = new Enemy(0, this); // 类型0:基础敌人
    enemy->setPath(pathPoints);

    // 添加到场景和列表
    gameScene->addItem(enemy);
    enemies.append(enemy);
    enemiesSpawnedThisWave++;

    // 连接敌人信号
    // 注意：需要在Enemy类中添加reachedEndPoint信号

    qDebug() << "生成敌人，当前敌人数量:" << enemies.size()
             << "，本波已生成:" << enemiesSpawnedThisWave;
}

void GamePage::updateGame()
{
    if (!gameRunning || isPaused)
        return;

    // 更新所有敌人
    updateEnemies();

    // 更新所有防御塔
    updateTowers();

    // 清理死亡实体
    removeDeadEntities();

    // 检查游戏结束
    checkGameOver();
}

void GamePage::updateEnemies()
{
    QList<Enemy *> enemiesToRemove;

    for (Enemy *enemy : enemies)
    {
        if (!enemy)
            continue;

        enemy->update();

        // 检查敌人是否到达终点
        if (enemy->x() > 640-20 && enemy->x() < 680+20 && enemy->y() > 360-20 && enemy->y() < 400+20)
        {
            // 敌人到达终点，扣减生命值
            lives--;
            livesLabel->setText(QString::number(lives));

            enemiesToRemove.append(enemy);
            gameScene->removeItem(enemy);

            qDebug() << "敌人到达终点，剩余生命:" << lives;
        }
    }

    // 移除到达终点的敌人
    for (Enemy *enemy : enemiesToRemove)
    {
        enemies.removeOne(enemy);
        enemy->deleteLater();
    }
}

void GamePage::updateTowers()
{
    // 更新每个防御塔的敌人列表
    for (Tower *tower : towers)
    {
        if (!tower)
            continue;

        // 收集范围内的敌人
        QList<Enemy *> enemiesInRange;
        for (Enemy *enemy : enemies)
        {
            if (!enemy)
                continue;

            qreal dx = enemy->x() - tower->x();
            qreal dy = enemy->y() - tower->y();
            qreal distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= tower->getRange())
            {
                enemiesInRange.append(enemy);
            }
        }

        // 设置防御塔的攻击目标
        tower->setEnemiesInRange(enemiesInRange);
        tower->update();
    }
}

void GamePage::removeDeadEntities()
{
    // 移除死亡的敌人
    QList<Enemy *> deadEnemies;

    for (Enemy *enemy : enemies)
    {
        if (!enemy)
            continue;

        if (enemy->getHealth() <= 0)
        {
            // 敌人死亡，获得金币
            gold += enemy->getReward();
            goldLabel->setText(QString::number(gold));

            deadEnemies.append(enemy);
            gameScene->removeItem(enemy);

            qDebug() << "敌人死亡，获得金币:" << enemy->getReward()
                     << "，当前金币:" << gold;
        }
    }

    for (Enemy *enemy : deadEnemies)
    {
        enemies.removeOne(enemy);
        enemy->deleteLater();
    }
}

void GamePage::checkGameOver()
{
    if (lives <= 0)
    {
        gameTimer->stop();
        enemySpawnTimer->stop();
        gameRunning = false;

        QMessageBox::information(this, "游戏结束",
                                 QString("游戏结束！\n你坚持到了第 %1 波\n获得了 %2 金币")
                                     .arg(currentWave)
                                     .arg(gold));

        emit gameOver();
    }
}

void GamePage::mousePressEvent(QMouseEvent *event)
{
    if (!gameRunning)
        return;

    // 将鼠标点击位置从视图坐标转换到场景坐标
    QPoint viewPos = event->pos();

    // 计算view相对于GamePage的位置
    QPoint viewGlobalPos = gameView->mapFromParent(viewPos);

    // 确保点击在view范围内
    if (!gameView->rect().contains(viewGlobalPos))
    {
        return;
    }

    // 转换到场景坐标
    QPointF scenePos = gameView->mapToScene(viewGlobalPos);

    qDebug() << "Mouse click - Widget pos:" << event->pos()
             << "View pos:" << viewGlobalPos
             << "Scene pos:" << scenePos;

    // 计算网格位置（对齐到网格）
    int gridSize = GameConfig::GRID_SIZE;
    int gridX = int(scenePos.x() / gridSize) * gridSize;
    int gridY = int(scenePos.y() / gridSize) * gridSize;

    // 确保在有效范围内
    if (gridX < 0 || gridY < 0 || gridX >= 800 || gridY >= 600)
    {
        qDebug() << "Click outside valid area";
        return;
    }

    // 检查是否在路径上（简化检查）
    bool onPath = false;
    for (const QPointF &point : pathPoints)
    {
        qreal dx = point.x() - scenePos.x();
        qreal dy = point.y() - scenePos.y();
        if (dx * dx + dy * dy < 900)
        { // 30像素半径内
            onPath = true;
            break;
        }
    }

    if (onPath)
    {
        qDebug() << "Cannot build on path";
        return;
    }

    if (event->button() == Qt::LeftButton)
    {
        // 检查是否已经有防御塔
        bool towerExists = false;
        for (Tower *tower : towers)
        {
            if (tower &&
                qAbs(tower->x() - gridX) < gridSize / 2 &&
                qAbs(tower->y() - gridY) < gridSize / 2)
            {
                towerExists = true;
                qDebug() << "Tower already exists at (" << gridX << "," << gridY << ")";
                break;
            }
        }

        if (!towerExists)
        {
            // 检查是否有足够金币建造防御塔
            if (gold >= 100)
            { // 箭塔价格100
                Tower *tower = new Tower(Tower::ARROW_TOWER, QPointF(gridX, gridY), this);
                tower->setPos(gridX, gridY);    // 显式设置位置
                tower->setGameScene(gameScene); // 设置gameScene，用于发射子弹
                towers.append(tower);
                gameScene->addItem(tower);

                // 扣除金币
                gold -= tower->getCost();
                goldLabel->setText(QString::number(gold));

                qDebug() << "Tower built at (" << gridX << "," << gridY
                         << "), cost:" << tower->getCost()
                         << ", gold remaining:" << gold;

                // 可选：添加建造动画或音效
                QGraphicsRectItem *highlight = new QGraphicsRectItem(gridX, gridY, gridSize, gridSize);
                highlight->setBrush(QBrush(QColor(255, 255, 0, 100)));
                highlight->setPen(QPen(Qt::NoPen));
                gameScene->addItem(highlight);

                // 淡出动画
                QTimer::singleShot(500, [highlight]()
                                   {
                    if (highlight->scene()) {
                        highlight->scene()->removeItem(highlight);
                        delete highlight;
                    } });
            }
            else
            {
                qDebug() << "Not enough gold to build tower";
                // 可以添加金币不足的提示
                QGraphicsTextItem *text = new QGraphicsTextItem("金币不足!");
                text->setPos(scenePos);
                text->setDefaultTextColor(Qt::red);
                text->setFont(QFont("Arial", 12, QFont::Bold));
                gameScene->addItem(text);

                QTimer::singleShot(1000, [text]()
                                   {
                    if (text->scene()) {
                        text->scene()->removeItem(text);
                        delete text;
                    } });
            }
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        // 右键可以显示信息或取消选择
        qDebug() << "Right click at grid (" << gridX << "," << gridY << ")";
    }

    QWidget::mousePressEvent(event);
}

void GamePage::mouseMoveEvent(QMouseEvent *event)
{
    if (!gameRunning)
        return;

    // 转换坐标
    QPoint viewPos = event->pos();
    QPoint viewGlobalPos = gameView->mapFromParent(viewPos);

    if (gameView->rect().contains(viewGlobalPos))
    {
        QPointF scenePos = gameView->mapToScene(viewGlobalPos);

        // 计算网格位置
        int gridSize = GameConfig::GRID_SIZE;
        int gridX = int(scenePos.x() / gridSize) * gridSize;
        int gridY = int(scenePos.y() / gridSize) * gridSize;

        // 可以在这里添加网格高亮效果
        static QGraphicsRectItem *lastHighlight = nullptr;
        if (lastHighlight)
        {
            gameScene->removeItem(lastHighlight);
            delete lastHighlight;
            lastHighlight = nullptr;
        }

        // 创建新的高亮
        if (gridX >= 0 && gridY >= 0 && gridX < 800 && gridY < 520)
        {
            lastHighlight = new QGraphicsRectItem(gridX, gridY, gridSize, gridSize);
            lastHighlight->setBrush(QBrush(QColor(255, 255, 255, 30)));
            lastHighlight->setPen(QPen(QColor(255, 255, 255, 100), 2));
            lastHighlight->setZValue(1000); // 确保在最上层
            gameScene->addItem(lastHighlight);
        }
    }

    QWidget::mouseMoveEvent(event);
}
