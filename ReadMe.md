#### 项目名称：Dungeon Game

#### 目录结构

```
csharp复制代码DungeonGame/
|-- SDL2/                  # SDL2库文件
|-- SDL2_image/            # SDL2_image库文件
|-- SDL2_ttf/              # SDL2_ttf库文件
|-- arial.ttf              # 字体文件
|-- DungeonGame.cpp        # 源代码文件
|-- DungeonGame.dev        # Dev-C++ 项目文件
|-- DungeonGame.exe        # 编译后的可执行文件
|-- DungeonGame.o          # 对象文件
|-- gmon.out               # 性能分析文件
|-- knight.png             # 骑士图标文件
|-- Makefile.win           # Makefile 文件
|-- princess.png           # 公主图标文件
|-- SDL2.dll               # SDL2 动态链接库文件
|-- SDL2_image.dll         # SDL2_image 动态链接库文件
|-- SDL2_ttf.dll           # SDL2_ttf 动态链接库文件
|-- sdl2-config            # SDL2 配置文件
```

#### 运行环境

- 操作系统：Windows
- 开发环境：Dev-C++ 或其他支持 SDL2 的 C++ 编译器
- 需要安装的库：
  - SDL2
  - SDL2_image
  - SDL2_ttf

#### 编译和运行

1. **确保安装了 Dev-C++**（或其他支持 SDL2 的 C++ 编译器）。

2. **打开 Dev-C++**。

3. 打开项目文件

   ：

   - 在 Dev-C++ 中，选择 `File > Open Project or File`，然后选择 `DungeonGame.dev`。

4. 配置 Dev-C++

   ：

   - 在 Dev-C++ 中，选择 `Tools > Compiler Options`。
   - 在 `Directories` 标签下，选择 `C++ includes` 并添加 `SDL2`、`SDL2_image` 和 `SDL2_ttf` 的包含目录。
   - 选择 `Libraries` 并添加 `SDL2`、`SDL2_image` 和 `SDL2_ttf` 的库目录。

5. 编译项目

   ：

   - 在 Dev-C++ 中，选择 `Execute > Compile and Run`。

6. 运行可执行文件

   ：

   - 在命令提示符或双击 `DungeonGame.exe`。

#### 游戏玩法

1. **启动程序**后，将会显示一个弹窗提示输入地牢的行数 (M) 和列数 (N)。输入 M 和 N

后，点击“确定”。 2. **生成地牢地图**：程序会随机生成一个 MxN 的地牢地图，其中每个房间的数值表示骑士在进入该房间时增加或减少的健康点数。负值表示减少健康点数，正值表示增加健康点数，零表示没有变化。红色表示负值房间，绿色表示正值房间，白色表示零值房间。 3. **显示骑士和公主**：左上角显示骑士图标，右下角显示公主图标。 4. **路径计算**：

- 点击 `Calc` 按钮，程序会计算骑士到达公主所需的最小初始健康点数，并在右下角显示路径和最小健康点数。
- 最小健康点数计算逻辑：
  - 如果存在一条路径的数值和大于或等于零，则骑士的最小初始健康点数为 1。
  - 如果所有路径的数值和均小于零，则取其中绝对值最小的路径和的绝对值加 1 作为骑士的最小初始健康点数。

1. 重新生成地牢地图

   ：

   - 点击 `Regen` 按钮，程序会重新生成一个随机的 MxN 地牢地图。

#### 文件说明

- **DungeonGame.cpp**：源代码文件，包含游戏的主要逻辑和渲染代码。
- **arial.ttf**：字体文件，用于显示地牢地图中的数值和按钮标签。
- **knight.png** 和 **princess.png**：分别表示骑士和公主的图标文件。
- **SDL2.dll**，**SDL2_image.dll**，**SDL2_ttf.dll**：动态链接库文件，确保与程序一起分发。
- **Makefile.win**：Makefile 文件，用于配置 Dev-C++ 的编译和链接设置。

#### 注意事项

- 确保所有必要的 DLL 文件（SDL2.dll，SDL2_image.dll，SDL2_ttf.dll）与可执行文件位于同一目录中。
- 确保字体文件（arial.ttf）和图标文件（knight.png，princess.png）路径正确无误。
- 如果在运行过程中遇到问题，请检查 Dev-C++ 的配置是否正确，特别是库文件和包含文件的路径设置。

#### 项目文件打包

将整个 `DungeonGame` 文件夹打包成 ZIP 文件，确保所有必要的文件和文件夹（包括 `SDL2`，`SDL2_image`，`SDL2_ttf`，以及所有 DLL 文件和资源文件）都包含在内。