# Fortune Hunter（校对批注：大部分由 deepseek 生成，我只修改一部分细节）

2020-2021 年写的一个寻宝小游戏。Python 起手，后来用 C++ 重写。这是我接触"项目式开发"之前、还只会写单文件程序时的作品。

原始 APCSP 课程作业存档：[2021-AP-CSP-Entries](https://github.com/Flash-Nova/2021-AP-CSP-Entries)

## 玩法

一张 60×15 的地图，藏着 3 个宝藏。你有 18 个探测器，每次放一个，返回它到**最近**宝藏的曼哈顿距离。要在探测器耗尽前把 3 个宝藏全找出来。

探测结果用字符显示：
- `@` — 这里就是宝藏
- `1` ~ `9` — 距离最近的宝藏有多远
- `-` — 附近 9 格内没有宝藏

和扫雷的反馈思路接近，但探的是"和最近宝藏的距离"而不是"周围雷数"。

原始仓库的说明：
```plaintext
EN:
Rough Rules:
A treasure-hunt game. Provides you 18 sensors (each can be used once), and there are 3 treasures to find.
You can press A S D W Keys to change the position you want, and press the Enter Key to place the sensor.
Once you placed a sensor, a character appears.
'@' means it is a treasure, congrats!
'1' to '9' means the Manhattan Distance (there will be a GUIDE in game) from the sensor to the treasure.
'-' means there's no treasure inside 9 distance units.

CN:
大致规则：寻宝游戏，18个传感器（一个能用一次），3个宝藏。
你可以通过按下 A S D W 键来改变位置，按下 Enter 键来放置传感器。
放置传感器后，会出现一个字符。
'@' 意味着你找到了一个宝藏，那么恭喜。
'1' 到 '9' 九个字符代表传感器到宝藏的 曼哈顿距离（游戏中会有说明）。
'-' 代表着附近9个距离单位内没有宝藏。
```

## 文件说明

```
python/
  treasure_seeking.py              2020.09 初版，纯命令行，手动输入坐标
cpp/
  treasure_seeking_cpp_rewrite.cpp 2021.04 C++ 重写版，APCSP 课程作业提交版
  alpha_1.cpp ~ alpha_3.cpp        2021.04-05 提交后自己继续加的迭代版本
README.txt                         2021.03 最早的规则说明（原文保留）
```

## 关于 C++ 重写

Python 版要手动输入坐标，玩起来费眼又低效。C++ 重写版把交互改成了：

- WASD 移动选中的格子
- 回车键放探测器
- 右侧内置一个简易状态栏（计时、剩余探测器、坐标、当前操作）
- 游戏结束后打印一份"战绩"，标出探测过的点、猜中/没猜中的宝藏位置

这是我第二次尝试做"纯命令行 UI"——第一次是 `猜数`（更早）。当时还不懂工程化，整个项目就一个 `.cpp` 文件，全程 `#define` 大法 + Win32 API 直接怼控制台光标。

## 关于 alpha_1 ~ alpha_3

APCSP 交完作业之后手痒，自己继续加的东西：

- `alpha_1` 加了开场打字机动画
- `alpha_2` 加了主菜单（START / GUIDE / RULES / SETTINGS / EXIT）
- `alpha_3` 加了规则页，用中文说明玩法

后来还想过写个自动寻宝的 bot（`alpha_4` 开了个头），没写完就搁下了。源码里有半成品和编译错误，就没归档进来。

## 环境

- Windows
- C++ 编译器（当年用的是 Dev-C++ 的 MinGW）
- Python 3

## 备注

这是一个"不愿意再维护"的历史作品。保留它是因为它记录了一个还没学会工程化的阶段——一个文件从头写到尾，不考虑解耦、不考虑测试，只管能不能跑。现在翻回去看，会想笑，但也是真实的一步。

如需编译，直接 `g++ alpha_3.cpp -o alpha_3.exe` 即可（在 Windows 的 `cmd.exe` 里运行，别用 Windows Terminal，会乱码）。