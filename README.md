# STC89C52 开发板项目

本项目旨在利用SDCC编译器开发STC89C52单片机应用，主要功能包括：
- LCD1602液晶显示驱动
- Timer0定时器应用
- 按键输入处理

## 开发环境

- **单片机**: STC89C52
- **编译器**: SDCC (Small Device C Compiler)
- **开发板**: 普中A2 (或兼容的51系列开发板)

## 文件结构

- `src/`: 存放源代码文件 (`.c`)。
  - `main.c`: 主程序文件，包含主函数 `main()` 和Timer0中断服务程序。
  - `LCD1602.c`: LCD1602液晶显示驱动实现文件。
  - `Delay.c`: 软件延时函数实现文件。
  - `Timer0.c`: Timer0定时器相关配置与初始化实现文件。
- `inc/`: 存放头文件 (`.h`)。
  - `LCD1602.h`: LCD1602液晶显示驱动头文件。
  - `Delay.h`: 软件延时函数头文件。
  - `Timer0.h`: Timer0定时器相关头文件。
  - `reg52.h`: 51系列单片机寄存器定义头文件。

## SDCC 编译与链接

### 编译设置
默认编译参数：`--iram-size 256 --xram-size 0 --code-size 8192`，对应参数如下：
- IRAM  SIZE: 256  Bytes
- XRAM  SIZE: 0    Bytes
- FLASH SIZE: 8192 Bytes

您可以根据实际需求在构建器选项中修改这些参数。

### 编译步骤

#### 1. 编译C源文件
对每个`.c`源文件进行编译，生成`.rel`目标文件。

```bash
E:\SoftWare\SDCC\bin\sdcc.exe -c --std-c99 -Iinc -mmcs51 --opt-code-speed --iram-size 256 --xram-size 0 --code-size 8192 -o build/Debug/.obj/src/main.rel src/main.c
E:\SoftWare\SDCC\bin\sdcc.exe -c --std-c99 -Iinc -mmcs51 --opt-code-speed --iram-size 256 --xram-size 0 --code-size 8192 -o build/Debug/.obj/src/LCD1602.rel src/LCD1602.c
E:\SoftWare\SDCC\bin\sdcc.exe -c --std-c99 -Iinc -mmcs51 --opt-code-speed --iram-size 256 --xram-size 0 --code-size 8192 -o build/Debug/.obj/src/Delay.rel src/Delay.c
E:\SoftWare\SDCC\bin\sdcc.exe -c --std-c99 -Iinc -mmcs51 --opt-code-speed --iram-size 256 --xram-size 0 --code-size 8192 -o build/Debug/.obj/src/Timer0.rel src/Timer0.c
```
**注意**: 请确保 `build/Debug/.obj/src/` 目录存在。

#### 2. 链接目标文件
将所有`.rel`目标文件链接成一个`.ihx`可执行文件。

```bash
E:\SoftWare\SDCC\bin\sdcc.exe --iram-size 256 --xram-size 0 --code-size 8192 --out-fmt-ihx build/Debug/.obj/src/main.rel build/Debug/.obj/src/LCD1602.rel build/Debug/.obj/src/Delay.rel build/Debug/.obj/src/Timer0.rel -o build/Debug/89c52_sdcc_demo.ihx
```

#### 3. 转换为HEX文件
使用 `packihx` 工具将`.ihx`文件转换为`.hex`文件，用于烧录到单片机。

```bash
E:\SoftWare\SDCC\bin\packihx.exe build/Debug/89c52_sdcc_demo.ihx > build/Debug/89c52_sdcc_demo.hex
```

## SDCC 兼容性注意事项

- **SFR (`sbit`) 定义**: SDCC 不支持Keil C51的 `sbit P2^6` 语法。需要使用 `__sbit __at(0xA0+6) LCD_RS;` 这种形式来定义特殊功能寄存器位。`0xA0` 是P2口的地址，`+6` 是P2.6的位偏移。
- **函数原型**: 为了避免 `warning 283: function declarator with no prototype` 警告，函数定义和声明应使用现代C标准，例如 `void func(void)` 而非 `void func()`。
- **`volatile` 关键字**: 在中断服务程序中修改的全局变量，应使用 `volatile` 关键字修饰，防止编译器优化导致的问题。

## 功能说明 (以LCD时钟为例)

- **LCD1602**: 用于显示时间信息（时、分、秒）。
- **Timer0**: 配置为定时模式，通过中断服务程序周期性更新时间变量。
- **按键**: 可用于控制时钟的设置或模式切换。

