#include "include/mainwindow.h"
#include "include/gamepage.h"
#include "include/mainmenupage.h"

#include <ui_mainwindow.h>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), gamePage(nullptr), mainMenuPage(nullptr)
{
    ui->setupUi(this);

    // 移除UI中stackedWidget的默认页面（如果有）
    while (ui->stackedWidget->count() > 0)
    {
        QWidget *widget = ui->stackedWidget->widget(0);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }

    // 创建页面
    mainMenuPage = new MainMenuPage(this);
    gamePage = new GamePage(this);

    // 添加到 stackedWidget
    ui->stackedWidget->addWidget(mainMenuPage);
    ui->stackedWidget->addWidget(gamePage);
    ui->stackedWidget->setCurrentWidget(mainMenuPage);

    // 连接信号
    connect(mainMenuPage, &MainMenuPage::startGameRequested,
            this, &MainWindow::switchToGamePage);
    connect(mainMenuPage, &MainMenuPage::exitGameRequested,
            this, &QApplication::quit);
    connect(gamePage, &GamePage::returnToMainMenu,
            this, &MainWindow::switchToMainMenu);
    connect(gamePage, &GamePage::gameOver,
            this, &MainWindow::onGameOver);

    setWindowTitle("保卫萝卜 - TD Defense Game");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchToGamePage()
{
    ui->stackedWidget->setCurrentIndex(1);
    if (gamePage)
    {
        gamePage->startGame();
    }
}

void MainWindow::switchToMainMenu()
{
    ui->stackedWidget->setCurrentIndex(0);
    if (gamePage)
    {
        gamePage->resetGame();
    }
}

void MainWindow::onGameOver()
{
    switchToMainMenu();
}
