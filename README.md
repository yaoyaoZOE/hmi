# HMI — Machine Control Panel

基于 GTKmm 技术栈的工业人机界面 (HMI) 示例应用，模拟机器控制面板。

## 技术栈

| 库 | 用途 |
|---|------|
| **gtkmm-4.0** | 跨平台 GUI 框架（C++ 绑定） |
| **glibmm-2.68** | GLib 的 C++ 封装 — 事件循环、定时器、字符串 |
| **sigc++-3.0** | 类型安全的 C++ 信号/回调框架 |
| **cairomm-1.16** | 2D 矢量图形渲染（仪表绑制） |
| **pangomm-2.48** | 文本布局和渲染 |

## 项目结构

```
hmi/
├── CMakeLists.txt
├── README.md
├── resources/
└── src/
    ├── main.cpp              # 入口，全局 CSS 样式
    ├── MainWindow.h/.cpp     # 主窗口，状态机，仿真逻辑
    ├── StatusWidget.h/.cpp   # 子系统状态面板（5 个指示器）
    ├── ControlPanel.h/.cpp   # Start / Stop / Reset / Emergency 按钮
    ├── GaugeWidget.h/.cpp    # 自定义圆形仪表（Cairo 绑制）
    └── LogView.h/.cpp        # 带时间戳的事件日志
```

## 界面布局

```
┌──────────────────────────────────────────────────────┐
│  HMI Machine Control                        [_][□][X] │
├──────────────┬───────────────────────────────────────┤
│              │                                       │
│ SYSTEM STATUS│  Process Value          Machine State │
│ ──────────── │  45.2 RPM               RUNNING       │
│ Power    [OK]│                                       │
│ Motor    [OK]│     ┌───────────────────┐            │
│ Cooling [WARN]│    │    ╭──────╮       │            │
│ Comm     [OK]│     │   │  45  │       │            │
│ Safety   [OK]│     │   ╰──────╯       │            │
│              │     │      RPM          │            │
│ CONTROLS     │     └───────────────────┘            │
│ ──────────── │                                       │
│ [▶ START]   │  Event Log                             │
│ [■ STOP]    │  ─────────────────────────────────    │
│ [↻ RESET]   │  [14:32:01] HMI started — ready       │
│              │  [14:32:15] Machine STARTED            │
│ [⚠ EMERGENCY]│  [14:32:20] Runtime: 5s  PV=45.2      │
└──────────────┴───────────────────────────────────────┘
```

## 功能特性

- **状态机**：`IDLE → RUNNING → STOPPED → EMERGENCY` 带完整信号控制
- **10 Hz 仿真**：`Glib::signal_timeout()` 驱动过程值 (PV) 在 0~100 之间震荡
- **Cairo 仪表**：270° 弧形仪表，绿→黄→红渐变，刻度标记，中心数字
- **子系统状态**：Power Supply / Motor Drive / Cooling / Communication / Safety Relay，根据 PV 阈值自动切换 OK / WARNING / ALARM
- **工业风格 UI**：深色主题，CSS 样式表，颜色语义化（绿=正常，橙=警告，红=报警）
- **事件日志**：时间戳 `[HH:MM:SS]` 格式，自动滚屏

## 构建

### 依赖安装

**Windows (MSYS2 UCRT64)**:
```bash
pacman -S mingw-w64-ucrt-x86_64-gtkmm-4.0
pacman -S mingw-w64-ucrt-x86_64-cmake
pacman -S mingw-w64-ucrt-x86_64-ninja
```

**Ubuntu / Debian**:
```bash
sudo apt install libgtkmm-4.0-dev cmake g++
```

**Fedora**:
```bash
sudo dnf install gtkmm4.0-devel cmake gcc-c++
```

### 编译运行

```bash
cd hmi
cmake -B build -G Ninja
cmake --build build
./build/hmi_app
```

> 如果系统只有 GTKmm 3，CMakeLists.txt 会自动回退到 `gtkmm-3.0`，但源码使用的是 GTKmm 4 API，需要手动适配（将 `append()` → `add()`, `set_child()` → `add()`, `set_draw_func()` → `signal_draw().connect()` 等）。

## 类说明

### MainWindow
主窗口，管理全局布局和机器状态机。构造函数启动 `Glib::signal_timeout()` 100ms 定时器驱动 `on_simulation_tick()`。

| 方法 | 说明 |
|------|------|
| `start_machine()` | IDLE → RUNNING，启动按钮灰掉，停止按钮可用 |
| `stop_machine()` | RUNNING → STOPPED，PV 冻结 |
| `reset_machine()` | 回到 IDLE，PV 归零，所有状态恢复 OK |
| `emergency_stop()` | 强制进入 EMERGENCY，禁用全部按钮，Safety Relay 报警 |
| `on_simulation_tick()` | 每 100ms 更新 PV（0.5/step 震荡），联动子系统状态 |

### StatusWidget
`Gtk::ListBox` 实现的状态面板。`set_status(name, level)` 更新指定子系统颜色和文本。

### ControlPanel
四个按钮 + sigc++ 信号代理。`set_buttons_enabled()` 控制按钮的启用/禁用状态。

### GaugeWidget
继承 `Gtk::DrawingArea`，通过 `set_draw_func()` 注入 Cairo 绑制回调。`set_value(0~100)` 更新并触发重绑。

### LogView
`Gtk::TextView` + `Gtk::ScrolledWindow`，`append()` 自动追加时间戳并滚到底部。

## 键合方式

```cpp
// MainWindow 连接 ControlPanel 信号（sigc++）
m_controls.signal_start().connect(
    sigc::mem_fun(*this, &MainWindow::start_machine));

// Glib 主循环定时器
m_timer_conn = Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &MainWindow::on_simulation_tick), 100);
```

## License

MIT
