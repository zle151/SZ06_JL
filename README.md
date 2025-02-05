## ✅需求一：旋钮+数码管

> 空气炸锅机，洗衣机，烘干机，消毒机，洗碗机 交互流程。

**数码管+旋转编码器**

### 默认状态，待机模式：

数码管显示：000（表示温度，默认100°C，或可以显示上次设置的温度）。
旋转编码器：旋转编码器默认用于调整温度。
顺时针旋转：增加温度（每次增加1°C，范围为80°C到199°C）。
逆时针旋转：减少温度。每次减少1°C

### 按下编码器，时间设置模式：

数码管显示：上次设定的时间或默认时间（如10分钟）。
旋转编码器：现在用于调整烹饪时间。
顺时针旋转：增加时间（每次增加1分钟，最大设定时间60分钟）。
逆时针旋转：减少时间

再次按下恢复为待机模式

### 接线方式

|     设备     |    引脚    |   类型   |
| :----------: | :--------: | :------: |
|    公共线    |  3V3, GND  | 公共供电 |
| EC11旋钮输入 |  A: PD11   |   EXTI   |
|              |  B: PD13   |   EXTI   |
|              |  C: PD15   |   EXTI   |
| LED188数码管 | LED1: PE15 |   GPIO   |
|              | LED2: PE13 |   GPIO   |
|              | LED3: PE11 |   GPIO   |
|              | LED4: PE9  |   GPIO   |
|              | LED5: PE7  |   GPIO   |

### 运行方式

1. 按照上一步接好杜邦线

2. 切换Target到`BUSINESS_1 EC11`，编译烧录即可：

![image-20241022195949882](./Image/image-20241022195949882.png)

## ✅需求二：AD按键+OLED

> 筋膜枪，电动牙刷，烘干机，小风扇  

**adc按键+IIC屏幕** 

### 按键功能

共3个按键

- 按键1 (模式切换键)：用于切换筋膜枪的工作模式（例如模式1、模式2、模式3等）。
- 按键2 (强度调整键)：用于调节振动强度（例如低、中、高三挡）。
- 按键3 (开关/暂停键)：用于开关筋膜枪，或在工作时暂停。

### IIC屏幕显示

IIC屏幕可以用来实时显示筋膜枪的状态信息，包括当前模式、强度、工作状态、倒计时（如果有定时功能）。

屏幕上显示信息的布局建议如下：

- 第一行：显示当前模式（例如“Mode 1”、“Mode 2”）。

- 第二行：显示当前强度（例如“ Low”、“ Medium”、“High”）。
- 第三行：显示工作状态（例如“Running”、“Paused” + 倒计时/剩余时间）。

### 接线方式

|        功能        |   引脚   |    说明     |
| :----------------: | :------: | :---------: |
|       公共线       | 3V3, GND |  公共供电   |
|   ADKey按键输入    |   PA2    |    ADC0     |
| I2C OLED输出 - SCL |   PA8    | I2C2 时钟线 |
| I2C OLED输出 - SDA |   PC9    | I2C2 数据线 |

### 运行方式

1. 按照上一步接好杜邦线

2. 切换Target到`BUSINESS_2 ADKey`，编译烧录即可：

![image-20241022200111119](./Image/image-20241022200111119.png)

## 🟩需求三：密码锁+OLED

> 密码锁  

**触摸按键+ iic屏幕**

1. 按键功能
0-9数字键：用于输入密码。
#键：确认键，确认输入的密码并进行验证。
*键：取消键或删除键，用于清除错误输入或者返回上一步。
2. 智能密码锁的交互流程
初始状态：待机模式
显示屏显示：密码锁屏幕可以显示待机状态信息，如锁的状态和用户提示。
Locked
Enter PIN:

**触摸按键功能：**
用户触摸数字按键（0-9）开始输入密码，每触摸一次按键，屏幕上会显示多一个*,例如：

Enter PIN: `******`
****

输入完密码后，按下`#键`进行确认。系统会根据预设的密码进行验证。按下`*键`可清除当前输入

- 密码正确：屏幕显示 Unlocking...
  密码锁解锁，按下任意键回到待机状态。
- 密码错误：屏幕显示 Try Again

- 连续3次密码输入错误：屏幕显示 Locked for 10 sec

  进入锁定状态，屏幕提示等待一段时间后再尝试

- 锁定状态：幕上显示倒计时信息Try Again in: 9 sec

## 🟩需求四：RGB+点阵屏

