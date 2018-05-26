# Auto-following Suitcase Application
This application is designed to show how to develop an **Auto-following Suitcase** using embARC.A person holding a tag in hand,three ultra-wideband anchors should be installed inside the suitcase in an equilateral triangular shape with a side length of 20cm. The distance information measured by the UWB module is transmitted to EMSK through UART.Based on the principle of three-point positioning,the azimuth between the person and the suitcase can be obtained to achieve following.This suitcase can also monitor ambient temperature and air conditions.When the distance between the trunk and the owner exceed the preset ,the trunk will stop automatically,and then sound the alarm or send an SMS alert to the mobile phone, so you don't have to worry about losing the trunk.This design is mainly suitable for flat and open indoor applications such as airport waiting halls.

* [Introduction](#introduction)
	* [Function](#function)
	* [System Architecture](#system-architecture)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

## Introduction
**Smart Auto-following Suitcase**

### Function

- **Ambient temperature and air condition monitoring**
- **obstacle avoiding**
- **Auto-following** 
- **Auto-alarm** (The device will sound the alarm when the distance between one and suitcase is longer than 4 meters, and send an SMS alert to the mobile phone when it is longer than 5.5 meters)

![ibaby_function][0]

### System Architecture

![system_architecture][1]

## Hardware and Software Setup
### Required Hardware
- 1 DesignWare ARC EM Starter Kit(EMSK)
- 1 Digilent PMOD TMP2
- 1 Dust Sensor(ZPH01)
- 1 Display Screen(OLED)
- 1 Buzzer
- 1 GPRS Module(SIM900A)
- 2 Motor Driver Module(TB6612FNG)
- 4 Positioning Module(DWM1000)
- 4 Wheel
- 4 Motor
- 1 SD Card
- 3 Ultrasonic Sensor(HCSR04)

The list of hardware is shown in the picture following. 

![wearable_node][3]

### Required Software
- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom

### Hardware Connection
1. The EMSK implement **Auto-folowing** smart device, it will process the data returned by positioning module and ultrasonic sensor,and send instructions to motor driver module.It can also monitor temperature and dust concentration of air,we can view these data on OLED. 
   - Connect **Positioning module** to **J1**(Using UART interface), connect **Motor driver module** to **J3** and **J6**,connect **Ultrasonic sensor** to **J1** and **J3**.
   - Connect **PMOD TMP2** to **J4**(Using IIC interface), connect **ZPH01** to **J5**(Using uart interface),connect **OLED** and **GPRS** to **J2**(Using IIC interface),connect **Buzzer** to **J6**.
2. Configure your EMSKs with proper core configuration.

## User Manual
### Before Running This Application
Download source code of **Auto-following Suitcase** from github.

The hardware resources are allocated as following table.

|  Hardware Resource  |            Function                                           |
| ------------------- | ------------------------------------------------------------- |
|  PMOD TMP2          |        Temperature sensor                                     |
|  ZPH01              |        Dust sensor                                            |
|  OLED               |        Display screen                                         |
|  SIM900A            |        Send messages or call                                  |
|  TB6612FNG          |        Motor driver module                                    |
|  HCSR04             |        Dust sensor                                            |
|  OLED               |        Ultrasonic sensor                                      |
|  DWM1000            |        Positioning module                                     |

- Modify mux.c (/board/emsk/common/emsk_init.c)
```
line 107: change 
	set_pmod_mux(PM1_UR_UART_0 | PM1_LR_SPI_S	\
				| PM2_I2C_HRI		\
				| PM3_GPIO_AC		\
				| PM4_I2C_GPIO_D	\
				| PM5_UR_SPI_M1 | PM5_LR_GPIO_A	\
				| PM6_UR_SPI_M0 | PM6_LR_GPIO_A );
 to 
	set_pmod_mux(mux_regs, PM1_UR_UART_0 | PM1_LR_GPIO_A	\
				| PM2_I2C_HRI			\
				| PM3_GPIO_AC			\
				| PM4_I2C_GPIO_D		\
				| PM5_UR_GPIO_C | PM5_LR_SPI_M2	\
				| PM6_UR_GPIO_C | PM6_LR_GPIO_A );
```
### Run This Application

Here take **EMSK2.2 - ARC EM7D** with GNU Toolset for example to show how to run this application.

#### Makefile

- Target options about EMSK and toolchain:

		BOARD ?= emsk
		BD_VER ?= 22
		CUR_CORE ?= arcem7d
		TOOLCHAIN ?= gnu

- The relative series of the root directory, here the path of the Makefile is 

		#
		# root dir of embARC
		#
		EMBARC_ROOT = ../../..


- Directories of source files and header files, notice that it **is not recursive**:

		# application source dirs
                   APPL_CSRC_DIR = .

                   APPL_ASMSRC_DIR = .

                # application include dirs
                   APPL_INC_DIR = .

                # application defines
                   APPL_DEFINES =

See [ embARC Example User Guide][40], **"Options to Hard-Code in the Application Makefile"** for more detailed information about **Makefile Options**.

#### Driver

Placing the drivers' source code in `driver` folder, you can see there are subfolders for ultasonic deiver
Placing the C source file and header file in the corresponding subfolder.

|  folder/file        |            Function           |
| ------------------- | ------------------------------|
|  ultrasonic         |       ultrasonic driver       |
|  temperature        |       temperature driver      |
|  gpio               |       gpio driver             |
|  iic                |       iic driver              |
|  uart               |       uart driver             |

[0]: ./doc/screenshots/system_architecture.PNG         "ibaby_function"
[1]: ./doc/screenshots/system_architecture.PNG    "system_architecture"
[2]: ./doc/screenshots/freeboard_ui.png           "freeboard_ui"
[3]: ./doc/screenshots/wearable_node.jpg          "wearable_node"
[4]: ./doc/screenshots/wifi_connected_info.PNG    "wifi_connected_info"
[5]: ./doc/screenshots/lwm2m_started_info.PNG     "lwm2m_started_info"
[6]: ./doc/screenshots/lamp_work_info.PNG         "lamp_work_info"


[30]: https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit    "DesignWare ARC EM Starter Kit(EMSK)"
[31]: http://store.digilentinc.com/pmodwifi-wifi-interface-802-11g/    "Digilent PMOD WiFi(MRF24WG0MA)"
[32]: https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/    "Acceleration sensor(MPU6050)"
[33]: http://www.electronics-lab.com/max30102/    "Heartrate sensor(MAX30102)"
[34]: https://developer.mbed.org/components/MLX90614-I2C-Infrared-Thermometer/    "Temperature sensor(MLX90614)"
[35]: https://github.com/XiangcaiHuang/ibaby.git    "iBaby Smarthome Gateway"
[36]: https://github.com/XiangcaiHuang/ibaby.git    "iBaby Freeboard UI"
[37]: http://www.openmobilealliance.org/release/LightweightM2M/V1_0_1-20170704-A/OMA-TS-LightweightM2M-V1_0_1-20170704-A.pdf    "LwM2M Protocol"
[38]: http://www.openmobilealliance.org/wp/OMNA/LwM2M/LwM2MRegistry.html#omalabel   "LwM2M Object and Resource"
[39]: http://www.freertos.org/a00106.html   "FreeRTOS API"
[40]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html   " embARC Example User Guide"
