/***************************************************************************//**
  @file     OLEDdisplay.c
  @brief    OLED display functions
  @author   Grupo 5
 ******************************************************************************/

/*---- Based in  fsl_SSD1306.h from NXP for SPI ------*/

#include "OLEDdisplay.h"
#include "I2CWrapper.h"
#include "fsl_i2c.h"

#include "timer.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief OLED buffer */
static uint8_t OLED_Buffer[(OLED_WIDTH * OLED_HEIGHT) / 8];
static uint8_t OLED_Scroll_Buffer[2][OLED_WIDTH * 8];

static bool isInit = false;
int OLEDtimerClbID = -1;

static bool roll = false;
static char* screenString;


volatile bool completionFlag = false;
volatile bool nakFlag        = false;
i2c_master_handle_t handle;

static void rollCLB(void);

static void shiftPageLeft(uint8_t page, uint8_t Scale);
static int OLED_Render_Scroll_Char (uint16_t X_axis, uint8_t Y_axis, uint8_t SC, int8_t String, uint8_t Scale);

static uint32_t stringLength = 0;
static int index = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void OLED_Command (uint8_t Cmd)
{
	i2c_master_transfer_t xfer = {0};

	xfer.data = &Cmd;
	xfer.dataSize = sizeof(Cmd);
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = SSD1306_ADDRESS_1;
	xfer.direction = kI2C_Write;
	xfer.subaddress = 0x0;
	xfer.subaddressSize = 1;

	I2C_MasterTransferBlocking(I2C0, &xfer);
}

static void OLED_Data (uint8_t *Data)
{
	i2c_master_transfer_t xfer = {0};

	/*Start Transfer*/
	xfer.data = Data;
	xfer.dataSize = sizeof(OLED_Buffer);
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = SSD1306_ADDRESS_1;
	xfer.direction = kI2C_Write;
	xfer.subaddress = 0x40;
	xfer.subaddressSize = 1;

	I2C_MasterTransferNonBlocking(I2C0, &handle, &xfer);
}

static void OLED_Reset (void)
{
	OLED_Command(OLED_DISPLAYON);
	OLED_Command(OLED_DISPLAYOFF);
	OLED_Command(OLED_DISPLAYON);
}

static void OLED_Config_Display (void)
{

	OLED_Reset();

	// Turn the OLED Display off
	OLED_Command(OLED_DISPLAYOFF);

	// Configure the display for 128x64 pixels, KS0108 mode
	OLED_Command(OLED_SETDISPLAYCLOCKDIV);
	OLED_Command(0x80);
	OLED_Command(OLED_SETMULTIPLEX);
	OLED_Command(OLED_HEIGHT-1);				// LCD Height
	OLED_Command(OLED_SETDISPLAYOFFSET);
	OLED_Command(0x0);
	OLED_Command(OLED_SETSTARTLINE | 0x0);
	OLED_Command(OLED_CHARGEPUMP);
	OLED_Command(0x14);							// Use 3.3V supply to generate high voltage supply
	OLED_Command(OLED_MEMORYMODE);
	OLED_Command(0x00);
	OLED_Command(OLED_SEGREMAP | 0x1);
	OLED_Command(OLED_COMSCANDEC);
	OLED_Command(OLED_SETCOMPINS);
	OLED_Command(0x12);
	OLED_Command(OLED_SETCONTRAST);
	OLED_Command(0xCF);
	OLED_Command(OLED_SETPRECHARGE);
	OLED_Command(0xF1);
	OLED_Command(OLED_SETVCOMDETECT);
	OLED_Command(0x40);
	OLED_Command(OLED_DISPLAYALLON_RESUME);
	OLED_Command(OLED_NORMALDISPLAY);
	OLED_Command(OLED_DEACTIVATE_SCROLL);

	OLED_Command(OLED_COLUMNADDR);
	OLED_Command(0);
	OLED_Command(OLED_WIDTH-1);
	OLED_Command(OLED_PAGEADDR);
	OLED_Command(0);
	OLED_Command(OLED_HEIGHT/8-1);

	// Turn the OLED display on!
	OLED_Command(OLED_DISPLAYON);

	OLED_Command(OLED_SETLOWCOLUMN | 0x0);  // low col = 0
	OLED_Command(OLED_SETHIGHCOLUMN | 0x0);  // hi col = 0
	OLED_Command(OLED_SETSTARTLINE | 0x0); // line #0
}


static int OLED_Render_Char (uint8_t X_axis, uint8_t Y_axis, uint8_t SC, int8_t String, uint8_t Scale)
{

	uint8_t px, py;
	uint16_t start_pos;

	if ((X_axis >= OLED_WIDTH) || (Y_axis >= OLED_HEIGHT))
	{
		return 1;
	}

	if (String > 127)
	{
		return 2;
	}

	if (Scale > 3)
	{
		return 3;
	}

	start_pos = ((uint8_t)String) * 7;			// Characters have a 7 row offset
	for (px = 0; px < 5; px++)
	{
		for (py = 0; py < 7; py++)
		{
			if ((font5x7[start_pos + py] >> (7 - px)) & 1)
			{
				switch (Scale)
				{
				case 3:
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+2, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+2, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+2, SC);
					break;
				case 2:
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					break;
				case 1:
				default:
					OLED_Set_Pixel(X_axis+px, Y_axis+py, SC);
					break;
				}
			}
		}
	}
	return 0;
}


static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        completionFlag = true;
    }
    /* Signal transfer success when received success status. */
    if ((status == kStatus_I2C_Nak) || (status == kStatus_I2C_Addr_Nak))
    {
        nakFlag = true;
    }
}


void OLED_Init(void)
{
	//Initialize I2C module
	BOARD_InitPins();
	BOARD_InitBootClocks();

	BOARD_I2C_ConfigurePins();

	I2C_InitModule();

	I2C_MasterTransferCreateHandle(I2C0, &handle, i2c_master_callback, NULL);

	/*Give the display a reset*/
	OLED_Reset();

	/* Clear the framebuffer*/
	OLED_Clear();

	/*Configure the OLED display controller*/
	OLED_Config_Display();

	//Configure start message with roll ON
	isInit = true;

	screenString = "WELCOME!";
	OLED_Set_Text(10, 32, kOLED_Pixel_Set, screenString, 2, false);
	OLED_Refresh();


	OLEDtimerClbID = Timer_AddCallback(rollCLB, 20, false);
}


void OLED_Refresh(void)
{
	OLED_Command(0xb0);
	OLED_Command(((0&0xf0)>>4) | 0x10);
	OLED_Command((0&0x0f) | 0x01);

	OLED_Data(&OLED_Buffer[0]);
}


void OLED_Clear(void)
{

	memset(OLED_Buffer + OLED_WIDTH, 0, sizeof(OLED_Buffer) - OLED_WIDTH);
	memset(OLED_Scroll_Buffer[0], 0, sizeof(OLED_Scroll_Buffer[0]));
	memset(OLED_Scroll_Buffer[1], 0, sizeof(OLED_Scroll_Buffer[1]));
}


void OLED_Clear_First_Page(void)
{
	memset(OLED_Buffer, 0, sizeof(OLED_Buffer)/8);
}


void OLED_Scroll_Clear(void)
{
	memset(OLED_Scroll_Buffer[0], 0, sizeof(OLED_Scroll_Buffer[0]));
	memset(OLED_Scroll_Buffer[1], 0, sizeof(OLED_Scroll_Buffer[1]));
}


void OLED_Fill(uint8_t Pattern)
{
	memset(OLED_Buffer, Pattern, sizeof(OLED_Buffer));
}


void OLED_Display_Mode (uint8_t Mode)
{

	if (Mode)
	{
		OLED_Command(OLED_INVERTDISPLAY);
	}
	else
	{
		OLED_Command(OLED_NORMALDISPLAY);
	}

}


void OLED_Set_Pixel (uint8_t X_axis, uint8_t Y_axis, uint8_t SC)
{

	if((X_axis >= OLED_WIDTH) || (Y_axis >= OLED_HEIGHT))
	{
		//Do nothing
	}
	else
	{
		switch(SC)
		{
			case kOLED_Pixel_Clear:
				OLED_Buffer [X_axis + (Y_axis / 8) * OLED_WIDTH] &= ~(1 << (Y_axis & 7));
				break;
			case kOLED_Pixel_Set:
				OLED_Buffer [X_axis + (Y_axis / 8) * OLED_WIDTH] |= (1 << (Y_axis & 7));
				break;
		}
	}
}


void OLED_Set_Scroll_Pixel(uint16_t X_axis, uint16_t Y_axis, uint8_t SC)
{
	int page = 0;

	if (Y_axis >= (8))
	{
		page++;
	}

	switch(SC)
	{
		case kOLED_Pixel_Clear:
			OLED_Scroll_Buffer[page][X_axis] &= ~(1 << (Y_axis & 7));
			break;
		case kOLED_Pixel_Set:
			OLED_Scroll_Buffer[page][X_axis]|= (1 << (Y_axis & 7));
			break;
	}
}


static int OLED_Render_Scroll_Char(uint16_t X_axis, uint8_t Y_axis, uint8_t SC, int8_t String, uint8_t Scale)
{

	Y_axis %= 8;
	uint16_t px, py;
	uint16_t start_pos;

	if ((X_axis >= OLED_WIDTH * 8))
	{
		return 1;
	}

	start_pos = ((uint16_t)String) * 7;			// Characters have a 7 row offset
	for (px = 0; px < 5; px++)
	{
		for (py = 0; py < 7; py++)
		{
			if ((font5x7[start_pos + py] >> (7 - px)) & 1)
			{
				switch (Scale)
				{
				case 3:
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale),  SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+2, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+2, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+2, SC);
					break;
				case 2:
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Scroll_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					break;
				case 1:
				default:
					OLED_Set_Scroll_Pixel(X_axis + px, Y_axis + py, SC);
					break;
				}
			}
		}
	}
	return 0;
}


void OLED_Set_Text(uint8_t X_axis, uint8_t Y_axis, uint8_t SC, char* String, uint8_t Scale, bool fromTime)
{
	uint16_t Cont;
	uint16_t xscaled;

	int strLength = strlen(String);
	if (strLength > 12 && !fromTime)
	{
		roll = true;
		OLED_Scroll_Clear();
	}


	for (Cont = 0; String[Cont] != '\0'; Cont++)
	{
		xscaled = X_axis + (Cont * 5 * Scale);

		if (roll)
		{
			if ((xscaled > OLED_WIDTH * 8))
			{
				//Do nothing
			}
			else
			{
				if (!fromTime)
					OLED_Render_Scroll_Char(xscaled, Y_axis, SC, String[Cont], Scale);
			}
		}



		if ((xscaled > OLED_WIDTH))
		{
			//Do nothing
		}
		else
		{
			OLED_Render_Char(xscaled, Y_axis, SC, String[Cont], Scale);
		}
	}
}


void OLED_Copy_Image(const uint8_t *Img, uint16_t size)
{

	uint16_t CpyBuffer;

	OLED_Clear();

	for(CpyBuffer = 0; CpyBuffer < size - 1; CpyBuffer++)
	{
		OLED_Buffer[CpyBuffer] = *(Img + CpyBuffer);
	}
}


bool OLEDisInit()
{
	return isInit;
}


void OLED_Write_Time(char* timestring, char* datestring)
{
	OLED_Set_Text(0,  0, kOLED_Pixel_Set, timestring, 1, true);
	OLED_Set_Text(75, 0, kOLED_Pixel_Set, datestring, 1, true);
}


void OLED_write_Text(uint8_t X_axis, uint8_t Y_axis, char* String)
{
	int strLength = strlen(String);

	if (strLength > 12)
	{
		roll = true;
		stringLength = strLength;
		index = 0;
		screenString = String;
	}
	else
	{
		roll = false;
	}

	OLED_Set_Text(X_axis, Y_axis, kOLED_Pixel_Set, String, 2, false);
}


static void rollCLB(void)
{
	//gpioWrite(TP, true);
	size_t count;
	if (I2C_MasterTransferGetCount(I2C0, &handle, &count) != kStatus_Success)
	{
		return;
	}


	if(roll)
	{
		shiftPageLeft(4, 2);
		OLED_Refresh();
	}
	else
	{
		OLED_Refresh();
	}

	//gpioWrite(TP, false);
}


static void shiftPageLeft(uint8_t page, uint8_t scale)
{
	int startIndex = page * OLED_WIDTH;

	// Make the string come back
	if ((index > (int)(5 * scale * (stringLength + 1)) ) || (index > (int)(OLED_WIDTH * 8)))
	{
		index = - OLED_WIDTH - OLED_WIDTH / 10;
	}


	for (int i = 0; i < OLED_WIDTH; i++)
	{
		if((i + index) >= 0 && (i + index) < OLED_WIDTH * 8)
		{
			OLED_Buffer[startIndex + i] = OLED_Scroll_Buffer[0][i + index];
		}

	}

	startIndex = (page + 1) * OLED_WIDTH;
	for (int i = 0; i < OLED_WIDTH; i++)
	{
		if((i + index) >= 0 && (i + index) < OLED_WIDTH * 8)
		{
			OLED_Buffer[startIndex + i] = OLED_Scroll_Buffer[1][i + index];
		}
	}


	index++;
}





