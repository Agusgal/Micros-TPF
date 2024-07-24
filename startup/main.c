/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/
//#define TEST_LuyBruno
#include "hardware.h"

void App_Init_test (void);
void App_Run_test(void);
void App_Init (void);
void App_Run (void);


int main (void)
{
    hw_Init();
    hw_DisableInterrupts();

	#ifdef TEST_LuyBruno
    	App_Init_test();
	#else
    	App_Init(); /* Program-specific setup */
	#endif
    hw_EnableInterrupts();



    __FOREVER__
	#ifdef TEST_LuyBruno
		App_Run_test();
	#else
        App_Run(); /* Program-specific loop  */
	#endif
}
