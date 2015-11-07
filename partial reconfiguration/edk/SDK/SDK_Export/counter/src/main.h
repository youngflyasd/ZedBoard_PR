/*
 * main.h
 *
 *  Created on: Mar 22, 2015
 *      Author: thepvongsa
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <stdio.h>
#include <stdlib.h>
#include "xil_printf.h"
#include "platform.h"
#include "xgpio.h"
#include "sd_reader/ff.h"
#include "sd_reader/diskio.h"
#include "xdevcfg.h"

#define BTN_DEVICE_ID			XPAR_BTNS_5BITS_DEVICE_ID
#define LED_DEVICE_ID			XPAR_LEDS_8BITS_DEVICE_ID
#define IP_BASSADDR			 	XPAR_BIT_COUNTER_IP_0_BASEADDR
#define DCFG_DEVICE_ID			XPAR_XDCFG_0_DEVICE_ID
#define COUNTUP_BITADDR 		0x00200000
#define COUNTDOWN_BITADDR 		0x00300000
#define BITFILE_LEN				0x01a650
#define USERTOIP				IP_BASSADDR
#define IPTOUSER				IP_BASSADDR+0x4
#define DELAY					10000000
#define CHANNEL1				1

const u32 BTN_INPUT = 0xFFFFFFFF;
const u32 LED_OUTPUT = 0x00000000;
const u32 ZERO_HEX = 0X00;

int status;
XGpio xBtn,xSw,xLed;
FATFS fatfs;
FIL filRead;
XDcfg *XDcfg_0;

u32 ParAddrMod1,ParAddrMod2;
u32 OutPut;
u32 btnData,btnOld;
uint firstMod=0;

//Functin prototype
int SD_Init();
int SD_TransferPartial(char *FileName, u32 DestinationAddress, u32 ByteLength);
int init_devices();
void ledOut(u32 ledIn);
int XDcfg_TransferBitfile(XDcfg *Instance, u32 StartAddress, u32 WordLength);
XDcfg *XDcfg_Initialize(u16 DeviceId);
void delay(uint delayMax);

#endif /* MAIN_H_ */
