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

### Run This Application

Modify the settings for connecting to the LwM2M Server(Gateway), as shown below:

(path: `src/wearable_node/function/lwm2m/lwm2m.c`):

		const static char *p_port   = (char *)"5683";    /* lwm2mServer's port and IP */
		const static char *p_server = (char *)"192.168.43.199";
		const static char *p_client_name = (char *)"wn"; /* name of lwm2m client node */

(path: `src/lamp_node/function/lwm2m/lwm2m.c`):

		const static char *p_port   = (char *)"5683";    /* lwm2mServer's port and IP */
		const static char *p_server = (char *)"192.168.43.199";
		const static char *p_client_name = (char *)"ln"; /* name of lwm2m client node */

Here take **EMSK2.2 - ARC EM11D** with Metaware Toolset for example to show how to run this application.

1. We need to use embARC 2nd bootloader to automatically load application binary for different EMSK and run. See *embARC Secondary Bootloader Example* for reference.

2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.

3. Interact using EMSK and Freeboard.

### Add a New Node

See **Lamp Node** for reference. It's complete and very helpful to learn how to add a new node to iBaby System although it seems very simple.

|  folder/file        |            Function                                           |
| ------------------- | ------------------------------------------------------------- |
|  driver             |        source code of drivers                                 |
|  function           |        source code of function modules                        |
|  lwm2m_client       |        source code of LwM2M Client                            |
|  FreeRTOSConfig.h   |        header file of FreeRTOS configurations                 |
|  main.c             |        main entry of embARC Application                       |
|  makefile           |        Makefile of embARC Application                         |

#### Makefile

- Selected FreeRTOS here, then you can use [FreeRTOS API][39] in your application:

		# Selected OS
		OS_SEL ?= freertos

- Target options about EMSK and toolchain:

		BOARD ?= emsk
		BD_VER ?= 22
		CUR_CORE ?= arcem11d
		TOOLCHAIN ?= gnu

- Reset the heap and stack size for LwM2M, make sure they are big enough for your application:

		##
		# HEAP & STACK SETTINGS
		# For LwM2M Stack Usage
		##
		HEAPSZ ?= 81920
		STACKSZ ?= 81920

- The relative series of the root directory, here the path of the Makefile is `./embarc_osp/application/ibaby_smarthome_multinode/src/lamp_node/makefile`:

		#
		# root dir of embARC
		#
		EMBARC_ROOT = ../../../..

- The middleware used in your application:

		MID_SEL = common lwip-contrib wakaama fatfs lwip

	*common* for baremetal function, *lwip*, *lwip-contrib* and *wakaama* for LwM2M, *fatfs* for file system .

	You might be wondering about **how wifi works?** There is nothing about it in the lamp node or wearable node application. Goto `./embarc_osp/board/board.c`, and you'll solve the problem:

		#if defined(OS_FREERTOS) && defined(MID_LWIP)
		static void task_wifi(void *par)
		{
		...
		}

	Wifi works as a independent task on the FreeRTOS, so you ought to select *freertos* for OS_SEL and include *lwip* in the MID_SEL. Then, the task for wifi will start to work automatically.

	![wifi_connected_info][4]

- Directories of source files and header files, notice that it **is not recursive**:

		# application source dirs
		APPL_CSRC_DIR = . ./lwm2m_client ./driver/acceleration ./driver/body_temperature ./driver/heartrate ./driver/timer ./function/ ./function/lwm2m ./function/print_msg ./function/process_acc ./function/process_hrate
		APPL_ASMSRC_DIR = .

		# application include dirs
		APPL_INC_DIR = . ./lwm2m_client ./driver/acceleration ./driver/body_temperature ./driver/heartrate ./driver/timer ./function/ ./function/lwm2m ./function/print_msg ./function/process_acc ./function/process_hrate

See [ embARC Example User Guide][40], **"Options to Hard-Code in the Application Makefile"** for more detailed information about **Makefile Options**.

#### Main Entry

- Firstly, initializing the hardware, such as buttons on the emsk and GPIO interface for lamp.

- Secondly, try to start LwM2M Client. Before that, modify the ssid and password of WIFI AP in `./embarc_osp/board/emsk/emsk.h`:

		143 #define WF_HOTSPOT_NAME             "embARC"
		    #define WF_HOTSPOT_PASSWD           "qazwsxedc"

	You ought to modify the flag of WIFI module selection if you are using **RW009** not MRF24G, in `./embarc_osp/board/board.mk`:

		16 WIFI_SEL ?= 1

	![lwm2m_started_info][5]

- Finally, starting to run the function moudles. Reading value from sensors, processing it and controlling someting to work according to the results, just like the **wearable node** and **lamp node** do.

	![lamp_work_info][6]

#### Driver

Placing the drivers' source code in `driver` folder, you can see there are subfolders for button and lamp drivers.
Placing the C source file and header file in the corresponding subfolder.

|  folder/file        |            Function           |
| ------------------- | ------------------------------|
|  btn                |        button driver          |
|  lamp               |        lamp driver            |

#### Function Module

- The `function` folder contains the API implementations of functions.

	|  folder/file        |            Function                                         |
	| ------------------- | ------------------------------------------------------------|
	|  lamp_work          |        lamp controller                                      |
	|  lwm2m              |        LwM2M Client start to work                           |
	|  print_msg          |        print out message for debug                          |
	|  common.h           |        common variables, settings and reported data         |

- In the `common.h`, set 1 to enable corresponding function, set 0 to disable.

		/**
		 * \name    macros for settings
		 * @{
		 */
		#define LWM2M_CLIENT      (1) /*!< set 1 to be lwm2m client */
	
		#define PRINT_DEBUG_FUNC  (1) /*!< set 1 to print out message for debug major function */
		/** @} end of name */

#### LwM2M Client

- In the `lwm2m_client` folder, you can see several files about LwM2M. See [**LwM2M Protocol**][37] and [**LwM2M Object and Resource**][38] to learn more about it.

- The following objects are nessary, you ought to keep them:

	|  file                         |
	| ----------------------------- |
	|  object_connectivity_stat.c   |
	|  object_device.c              |
	|  object_firmware.c            |
	|  object_security.c            |
	|  object_server.c              |

- Only the `object_flag_lamp_work.c` is custom here, you ought to remove it and add the new object definition files for your node. Then, modify `lwm2mclient.c`:

	The number of objects in your node:

		87 #define OBJ_COUNT 7

	Logic of reporting data to LwM2M Server:

		346 /* update the flag of lamp working value */
	            if (data_report_ln.flag_lamp_work != data_report_ln_old.flag_lamp_work)
		    {
			    lwm2m_stringToUri("/3311/0/5850", 12, &uri);
			    valueLength = sprintf(value, "%d", data_report_ln.flag_lamp_work);
			    handle_value_changed(context, &uri, value, valueLength);
			    data_report_ln_old.flag_lamp_work = data_report_ln.flag_lamp_work;
		    }
	
	Register custom objects:
	
		667 objArray[5] = get_lamp_object();
		    if (NULL == objArray[5]) {
			    EMBARC_PRINTF("Failed to create lamp object\r\n");
			    return -1;
		    }
	
	Finally, modify `lwm2mclient.h`:
	
		62   extern lwm2m_object_t * get_lamp_object();


[0]: ./doc/screenshots/ibaby_function.PNG         "ibaby_function"
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
