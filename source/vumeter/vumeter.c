/***************************************************************************//**
  @file     vumeter.c
  @brief    Vumeter functions
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#include "arm_math.h"
#include "math.h"
#include <stdbool.h>
#include <vumeter/vumeter.h>
#include "matrix_display.h"
#include "fsl_device_registers.h"
#include "Systick.h"

#define SAMPLE_LENGTH       FFT_SIZE
#define NUMBER_OF_BANDS     8  
#define VUMETER_HEIGHT      8
#define NOISE_THRES         1
#define MAX_AMPLITUDE       60
#define AVERAGE				2

static arm_rfft_fast_instance_f32 rfft_fast_instance;
static float32_t FFT_Cplx[SAMPLE_LENGTH];
static float32_t FFT_Mag[SAMPLE_LENGTH/2];
static int VU_Values[NUMBER_OF_BANDS];
static colors_t Color_Matrix[VUMETER_HEIGHT * NUMBER_OF_BANDS];
static uint8_t timer_id;
static colors_t Image_Matrix_5[VUMETER_HEIGHT * NUMBER_OF_BANDS]=
		{
				YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,
				YELLOW,YELLOW,RED,RED,RED,RED,YELLOW,YELLOW,
				YELLOW,YELLOW,RED,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,
				YELLOW,YELLOW,RED,RED,RED,RED,YELLOW,YELLOW,
				YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,RED,YELLOW,YELLOW,
				YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,RED,YELLOW,YELLOW,
				YELLOW,YELLOW,RED,RED,RED,RED,YELLOW,YELLOW,
				YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW
		};


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*!
 * @brief fills the Color_Matrix local array with the given values
 *
 * @param vumeterValues: vumeter values array passed by reference
 *
 * @return void
 */
void VU_Fill_Color_Matrix(int * vumeterValues);


void Fill_Spiral_Display(void);



/*******************************************************************************
 *                        GLOBAL FUNCTION DEFINITIONS
 ******************************************************************************/


void VU_Init()
{
    arm_rfft_fast_init_f32(&rfft_fast_instance, SAMPLE_LENGTH);
    timer_id=SysTick_AddCallback(Fill_Spiral_Display, 50);
}


int VU_FFT(float32_t * inputSignal, float32_t SR, int lowerFreq, int higherFreq)
{ 
    static volatile char average = 0;
    volatile unsigned int usableBins = (SAMPLE_LENGTH / 2 - 1);
    volatile double BinWidth = SAMPLE_LENGTH/SR;

    // Salto entre bandas para que queden equiespaciadas en escala logaritmica.
    volatile double base = higherFreq / lowerFreq;
    volatile double exp = 1.0f / (NUMBER_OF_BANDS - 1.0f );
    volatile float32_t Freq_Jump = pow(base,exp);

    arm_rfft_fast_f32(&rfft_fast_instance, inputSignal, FFT_Cplx, 0);

    for(uint16_t j = 0; j < SAMPLE_LENGTH/2; j++)
    {
    	volatile float32_t temp = FFT_Cplx[2*j]*FFT_Cplx[2*j] +FFT_Cplx[2*j+1]*FFT_Cplx[2*j+1];
    	arm_sqrt_f32(temp, &FFT_Mag[j]);
    }

    FFT_Mag[0] = 0; //Removing DC

    //unsigned int binFreq[NUMBER_OF_BANDS];
    volatile float32_t Current_Bin_Freq = lowerFreq;		// Frecuencia central del bin que se analizará
    volatile float32_t Next_Bin_Freq = lowerFreq * Freq_Jump;		// Frecuencia central del próximo bin a analizar
    volatile float32_t Current_Center_Bin;					// Indice del bin de frecuencia a analizar en el array de FFT
    volatile float32_t Next_Bin_Center;
    volatile float32_t Bin_Lower_Limit = 0;					// Límites inferiores y superiores de los indices incluidos
    volatile float32_t Bin_Upper_Limit;						//en el bin de la matriz
    volatile float32_t temp;
    for (size_t i = 0; i < NUMBER_OF_BANDS; i++)
    {

        Current_Center_Bin = Next_Bin_Freq * BinWidth;
        Next_Bin_Center = Next_Bin_Freq * BinWidth;
        Bin_Lower_Limit = i ? Bin_Upper_Limit : 0;
        Bin_Upper_Limit = (Current_Center_Bin+Next_Bin_Center)*0.5;//calcular higherBin!!!


        temp = 0;
        // Filter Out Sound below Noise Threshold
        for (size_t j = (int)(Bin_Lower_Limit); ((j < (int)(Bin_Upper_Limit))&&(j < usableBins)); j++)
        {
        	temp += (FFT_Mag[j] > NOISE_THRES)? FFT_Mag[j] : 0;
        }
        int roundedHeight = (int)(temp/MAX_AMPLITUDE);

        VU_Values[i] += (roundedHeight > VUMETER_HEIGHT ? VUMETER_HEIGHT : roundedHeight)/AVERAGE;

        Current_Bin_Freq = Next_Bin_Freq;
        Next_Bin_Freq *= Freq_Jump;
    } 

    average = (average + 1) % AVERAGE;
    if(average == 0)
    {
    	VU_Fill_Color_Matrix(VU_Values);
    	for(int j = 0; j <  NUMBER_OF_BANDS; j++)
    	{
    		VU_Values[j] = 0;
    	}
    }
    return 0;
}


void VU_Clear_Display()
{
    for(int i = 0; i < NUMBER_OF_BANDS; i++)
    {
    	for(int j = 0; j < VUMETER_HEIGHT; j++)
    	{
    		Color_Matrix[j*8 + i] = CLEAN;
    	}
    	VU_Values[i] = 0;
    }

    VU_Draw_Display();
}


void VU_Draw_Display()
{
    md_writeBuffer(Color_Matrix);
}


/***************************************************************************//**
  LOCAL FUNCTIONS
 ******************************************************************************/

void VU_Fill_Color_Matrix(int * vumeterValues)
{
    for(int i = 0; i < NUMBER_OF_BANDS; i++)
    {
    	for(int j = 0; j<VUMETER_HEIGHT; j++)
    	{
    		if(vumeterValues[i] > j)
			{
				if(j >= 7)
					Color_Matrix[7-i+j*8] = RED;
				else if(j >= 4)
					Color_Matrix[7-i+j*8] = YELLOW;
				else
					Color_Matrix[7-i+j*8] = GREEN;
			}
			else
				Color_Matrix[7-i+j*8] = CLEAN;
    	}
    }
    
}


void Fill_Spiral_Display(void)
{
	static int step=0;
	static int i=0;
	static int j=0;
	static bool done=false;
	Color_Matrix[8*i+j] = Image_Matrix_5[8*(7-i)+(7-j)];
	if (!done)
	{
		if (step<7)
			j++;
		else if (step<14)
			i++;
		else if (step<21)
			j--;
		else if (step<27)
			i--;
		else if (step<33)
			j++;
		else if (step<38)
		{
			i++;
		}
		else if (step<43)
		{
			j--;
		}
		else if (step<47)
		{
			i--;
		}
		else if (step<51)
		{
			j++;
		}
		else if (step<54)
		{
			i++;
		}
		else if (step<57)
		{
			j--;
		}
		else if (step<59)
		{
			i--;
		}
		else if (step<61)
		{
			j++;
		}
		else if (step==61)
		{
			i++;
		}
		else if (step==62)
		{
			j--;

		}
		else if (step==63)
		{
			done=true;
			Systick_PauseCallback(timer_id);
		}
		step++;
	}
	 VU_Draw_Display();
}



