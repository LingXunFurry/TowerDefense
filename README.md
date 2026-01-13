# TowerDefense

这是一个使用 Qt + C++ 编写的塔防游戏项目框架。  
主要负责游戏核心管理（GameManager）和对象管理，框架提供游戏循环、对象更新接口、金币和生命值信号接口，方便团队其他成员扩展敌人、防御塔和 UI。

---

## **目录结构**

- TowerDefense/
- ├─ GameManager.h / GameManager.cpp # 游戏核心管理类
- ├─ GameObject.h # 游戏对象基类
- ├─ DummyObject.h / DummyObject.cpp # 测试对象
- ├─ main.cpp # 程序入口
- ├─ mainwindow.h / mainwindow.cpp / mainwindow.ui # 主界面（此为qt自带，我并没有做修改，后续成员可修改文件内容）
- ├─ TowerDefense_zh_CN.ts # 国际化翻译文件
- ├─ CMakeLists.txt # CMake 构建文件
- ├─ .gitignore # 忽略文件

---

## **项目功能**

- 提供 `GameManager` 框架：
  - 游戏循环（每帧调用 `update`）
  - 对象统一管理（添加、移除、销毁）
  - 游戏状态控制（`startGame()`, `pauseGame()`）
  - 信号接口：
    - `gameOver()` 游戏结束
    - `enemyDead(int moneyReward)` 敌人死亡时发出金币奖励信号

- `GameObject` 抽象基类：
  - 每个游戏对象需继承 `GameObject`
  - 必须实现 `update()` 方法

- `DummyObject` 示例：
  - 提供了一个测试对象，用于验证框架是否正常运行

---

## **构建和运行**

### **使用 Qt Creator**
1. 打开 Qt Creator，选择 **Open Project** → 选择 `CMakeLists.txt` 或 `.pro` 文件  
2. 选择合适的 Kit（例如 Desktop Qt 6 + MinGW）  
3. 构建并运行项目  

### **命令行（CMake）**
```bash
mkdir build
cd build
cmake ..
cmake --build .
./TowerDefense   # 或 TowerDefense.exe (Windows)

