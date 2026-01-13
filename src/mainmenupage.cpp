#include "include/mainmenupage.h"
#include "include/resourcemanager.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>
#include <QSpacerItem>
#include <QApplication>

MainMenuPage::MainMenuPage(QWidget *parent)
    : QWidget(parent)
{
    initUI();

    // 确保背景正确显示
    setAutoFillBackground(false);
    setAttribute(Qt::WA_StyledBackground, false);
}

void MainMenuPage::initUI()
{
    // 设置主页面大小
    setFixedSize(800, 600);

    // 获取背景
    ResourceManager &rm = ResourceManager::instance();
    QPixmap background = rm.getDefaultBackground();

    // 创建背景标签
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(background);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(0, 0, 800, 600);

    // 主布局
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 添加顶部空白
    mainLayout->addStretch(2);

    // 标题
    titleLabel = new QLabel("保卫萝卜", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("Microsoft YaHei", 48, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #FF6B00;"
        "   background-color: rgba(255, 255, 255, 180);"
        "   padding: 20px;"
        "   border-radius: 20px;"
        "   border: 3px solid #FF8C00;"
        "}");
    titleLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->addWidget(titleLabel);

    // 添加中间空白
    mainLayout->addStretch(1);

    // 按钮容器
    QWidget *buttonContainer = new QWidget(this);
    buttonContainer->setStyleSheet("background-color: rgba(255, 255, 255, 150); border-radius: 15px;");
    buttonContainer->setFixedWidth(400);

    QVBoxLayout *buttonContainerLayout = new QVBoxLayout(buttonContainer);
    buttonContainerLayout->setSpacing(30);
    buttonContainerLayout->setContentsMargins(40, 40, 40, 40);

    // 开始游戏按钮
    startButton = new QPushButton("开始游戏", buttonContainer);
    startButton->setFixedHeight(60);
    startButton->setStyleSheet(
        "QPushButton {"
        "   font-size: 24px;"
        "   font-weight: bold;"
        "   color: white;"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4CAF50, stop:1 #45a049);"
        "   border: 3px solid #388E3C;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #45a049, stop:1 #4CAF50);"
        "   border: 3px solid #2E7D32;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3d8b40;"
        "   border: 3px solid #1B5E20;"
        "}");
    connect(startButton, &QPushButton::clicked, this, &MainMenuPage::onStartButtonClicked);
    buttonContainerLayout->addWidget(startButton);

    // 退出游戏按钮
    exitButton = new QPushButton("退出游戏", buttonContainer);
    exitButton->setFixedHeight(60);
    exitButton->setStyleSheet(
        "QPushButton {"
        "   font-size: 24px;"
        "   font-weight: bold;"
        "   color: white;"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f44336, stop:1 #da190b);"
        "   border: 3px solid #D32F2F;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #da190b, stop:1 #f44336);"
        "   border: 3px solid #C62828;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #a4160a;"
        "   border: 3px solid #B71C1C;"
        "}");
    connect(exitButton, &QPushButton::clicked, this, &MainMenuPage::onExitButtonClicked);
    buttonContainerLayout->addWidget(exitButton);

    // 将按钮容器居中
    mainLayout->addWidget(buttonContainer, 0, Qt::AlignCenter);

    // 添加底部空白
    mainLayout->addStretch(2);

    setLayout(mainLayout);

    // 确保背景在最底层
    backgroundLabel->lower();
}

void MainMenuPage::loadResources()
{
    // 已经通过ResourceManager加载
}

void MainMenuPage::onStartButtonClicked()
{
    emit startGameRequested();
}

void MainMenuPage::onExitButtonClicked()
{
    emit exitGameRequested();
}
