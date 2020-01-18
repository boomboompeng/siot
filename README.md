# 概述

本工程基于乐鑫官方ESP32-IDF-V3.3进行开发，所使用硬件平台为ESP32平台，具体型号为ESPWROOM32D
***关于乐鑫官方SDK介绍见[乐鑫github首页](https://github.com/espressif/esp-idf)***
***关于ESPWROOM32D硬件平台介绍见[乐鑫官网](https://www.espressif.com/zh-hans/products/hardware/esp32/overview)***

# 硬件环境搭建

- 准备开发前首先需要在主机配备USB串口驱动，方便对ESP32开发板进行烧录和调试，推荐Silicon Labs提供的驱动固件，比较稳定

- 其次需要了解ESP32开发板几个常用的GPIO

| 管脚编号 | 管脚名称 | 管脚功能 |
| -------- | -------- | -------- |
| 3        | EN       | 使能模组，高电平有效 |
| 

