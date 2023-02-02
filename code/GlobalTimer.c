/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xtime_l.h"
#include "xgpio.h"

#define TIMER_LOAD_VALUE 0xFFFFFFFF

#define printf xil_printf
#define GPIO_DEVICE_ID_LED XPAR_AXI_GPIO_0_DEVICE_ID
#define LED_CHANNEL 1

#define GPIO_DEVICE_ID_SW XPAR_AXI_GPIO_1_DEVICE_ID
#define SW_CHANNEL 1


int main()
{
    init_platform();

    print("-- Entering main() -- \n\r");

    XGpio GpioLED;
    XGpio GpioSW;
    int status, statusAll = 1;
    status = XGpio_Initialize(&GpioLED, GPIO_DEVICE_ID_LED);
    if(status != XST_SUCCESS){
    	statusAll = 0;
    	printf("LEDs initilization failure! \n\r");
    }
    else{
        XGpio_SetDataDirection(&GpioLED, LED_CHANNEL, 0x00);
        printf("LEDs initilization sucess! \n\r");
    }

    status = XGpio_Initialize(&GpioSW, GPIO_DEVICE_ID_SW);
    if(status != XST_SUCCESS){
      	statusAll = 0;
    	printf("SWITCHES initilization failure! \n\r");
    }
    else{
    	XGpio_SetDataDirection(&GpioSW, SW_CHANNEL, 0xFF);
        printf("SWITCHES initilization success! \n\r");
    }

    int i, SW_check;
    int count = 0;
    XTime time;
    unsigned int cycles, cycles1, cycles2, cycles3;

    XTime_SetTime(0);
    int whole, fract;
    float xx;

    if(statusAll){
    	while(1){
    		XTime_SetTime(0);
    		for(i = 0; i < 9999; i++);
    		printf("--------------------------------------------------\n\r");
    		XTime_GetTime(&time);
    		cycles1 = time;
    		printf("Cycles loop 1 - %10d \r\n", cycles1);

    		SW_check = XGpio_DiscreteRead(&GpioSW, 1);
    		printf("\nDIP switches status %x\r\n\n", SW_check);
    		XGpio_DiscreteWrite(&GpioLED, 1, count);

    		for(i = 0; i < 99999; i++);
    		XTime_GetTime(&time);
    		cycles2 = time;
    		printf("Cycles loop 2 - %10d \r\n", cycles2);
    		for(i = 0; i < 99999; i++);
    		count ++;

    		XTime_GetTime(&time);
    		cycles3 = time;
    		printf("Cycles loop 3 - %10d \r\n", cycles3);

    		cycles = cycles1+cycles2+cycles3;
    		printf("--------------------------------------------------\n\r");
    		printf("Profiling\r\n");

    		xx = (float)cycles1/cycles; whole = xx; fract = (xx-whole)*1000;
    		printf("Start         - cycles1 = %d.%3d\r\n", whole, fract);
    		xx = (float)cycles2/cycles; whole = xx; fract = (xx-whole)*1000;
    		printf("Start         - cycles2 = %d.%3d\r\n", whole, fract);
    		xx = (float)cycles3/cycles; whole = xx; fract = (xx-whole)*1000;
    		printf("Start         - cycles3 = %d.%3d\r\n", whole, fract);
    		printf("--------------------------------------------------\n\r");
    		for(i = 0; i < 99999999; i++);
    	}
    }

    cleanup_platform();
    return 0;
}
