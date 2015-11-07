#include "main.h"

int main()
{
	//Inititialize the base platform
	init_platform();

    //Initialize the hardware devices
    init_devices();

    //The address of the DDR where the partial bitstream is located
    ParAddrMod1 = COUNTUP_BITADDR;
    ParAddrMod2 = COUNTDOWN_BITADDR;

    while(1){
    	btnData = XGpio_DiscreteRead(&xBtn,CHANNEL1); 	//Reads the button input data

        OutPut = Xil_In32(IPTOUSER);	//Reads the output data from the IP

        ledOut(OutPut); //sends the output data to the led
        Xil_Out32(USERTOIP,OutPut);//send the output back into the IP

        delay(DELAY); //delay the system for visibility to the user

		//Whenever a button is pushed the FPGA will switch between the Partial Bits
        if(btnData != ZERO_HEX && btnData != btnOld){
        	if(firstMod){
        		firstMod = 0;
        		status = XDcfg_TransferBitfile(XDcfg_0, ParAddrMod1, (BITFILE_LEN >> 2));
        		if (status != XST_SUCCESS) {return XST_FAILURE;}
        	}else{
        		firstMod = 1;
        		status = XDcfg_TransferBitfile(XDcfg_0, ParAddrMod2, (BITFILE_LEN >> 2));
        		if (status != XST_SUCCESS) {return XST_FAILURE;}
        	}
	        btnOld = btnData;
        }
    }
    return 0;
}

int init_devices(){
	//Initialize the buttons
	status = XGpio_Initialize(&xBtn,BTN_DEVICE_ID);
    if(status != XST_SUCCESS){return XST_FAILURE;}
    XGpio_SetDataDirection(&xBtn,CHANNEL1,BTN_INPUT);

	//Initialize the leds
	status = XGpio_Initialize(&xLed,LED_DEVICE_ID);
	if(status != XST_SUCCESS){return XST_FAILURE;}
	XGpio_SetDataDirection(&xLed,CHANNEL1,LED_OUTPUT);

    //Initialize the PCAP
    XDcfg_0 = XDcfg_Initialize(DCFG_DEVICE_ID);

    //Initialize the SD card host
    status =  SD_Init();
    if (status != XST_SUCCESS){return XST_FAILURE;}

    //Loads the bitfiles to the DDR memory
    status = SD_TransferPartial("bcu.bin", COUNTUP_BITADDR, BITFILE_LEN);
    if (status != XST_SUCCESS) {return XST_FAILURE;}
    status = SD_TransferPartial("bcd.bin", COUNTDOWN_BITADDR, BITFILE_LEN);
    if (status != XST_SUCCESS) {return XST_FAILURE;}

    return XST_SUCCESS;
}

//simple output function for the ledoutput
void ledOut(u32 ledIn){
	XGpio_DiscreteWrite(&xLed,CHANNEL1,ledIn);
}

//initializes the PCAP
XDcfg *XDcfg_Initialize(u16 DeviceId)
{
	XDcfg *Instance = malloc(sizeof *Instance);
	XDcfg_Config *Config = XDcfg_LookupConfig(DeviceId);
	XDcfg_CfgInitialize(Instance, Config, Config->BaseAddr);

	// Enable and select PCAP interface for partial reconfiguration
	XDcfg_EnablePCAP(Instance);
	XDcfg_SetControlRegister(Instance, XDCFG_CTRL_PCAP_PR_MASK);

	return Instance;
}

//Transfers the bit from the DDR to the FPGA and reconfigues the hardware
int XDcfg_TransferBitfile(XDcfg *Instance, u32 StartAddress, u32 WordLength)
{
	int Status;
	volatile u32 IntrStsReg = 0;

	// Clear DMA and PCAP Done Interrupts
	XDcfg_IntrClear(Instance, XDCFG_IXR_D_P_DONE_MASK);

	// Transfer bitstream from DDR into fabric in non secure mode
	Status = XDcfg_Transfer(Instance, (u32 *) StartAddress, WordLength, (u32 *) XDCFG_DMA_INVALID_ADDRESS, 0, XDCFG_NON_SECURE_PCAP_WRITE);
	if (Status != XST_SUCCESS)return Status;

	// Poll PCAP Done Interrupt
	while ((IntrStsReg & XDCFG_IXR_D_P_DONE_MASK) != XDCFG_IXR_D_P_DONE_MASK)
		IntrStsReg = XDcfg_IntrGetStatus(Instance);

	delay(DELAY);
	return XST_SUCCESS;
}

//Generic delay function to slow down the clock in order to see the LEDs increments
void delay(uint delayMax){
	uint i;
	for(i = 0; i < delayMax;i++);
}

//Initialize the sdcard reader
int SD_Init()
{
	FRESULT FrSu;

	//mounts the sd host
	FrSu = f_mount(0, &fatfs);
	if (FrSu) {return XST_FAILURE;}

	return XST_SUCCESS;
}

//Transfer the bitstreams to the DDR ram
int SD_TransferPartial(char *FileName, u32 DestinationAddress, u32 ByteLength)
{
	FRESULT FrSu;
	UINT br;

	//Open the current bit stream by filename
	FrSu = f_open(&filRead, FileName, FA_READ);
	if (FrSu){return XST_FAILURE;}

	//Points the pointer to the top of the file
	FrSu = f_lseek(&filRead, 0);
	if (FrSu){return XST_FAILURE;}

	//Read a section of the bitstream and store it to the DDR
	FrSu = f_read(&filRead, (void*) DestinationAddress, ByteLength, &br);
	if (FrSu){return XST_FAILURE;}

	//Close the file
	FrSu = f_close(&filRead);
	if (FrSu){return XST_FAILURE;}

	return XST_SUCCESS;
}
