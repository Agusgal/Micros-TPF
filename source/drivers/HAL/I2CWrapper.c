/***************************************************************************/ /**
  @file     I2CWrapper.c
  @brief    I2C Driver
  @author   Grupo 5 - Lab. de Micros
 ******************************************************************************/
#include "I2CWrapper.h"

void I2C_InitModule(void)
{
    i2c_master_config_t masterConfig;
    uint32_t sourceClock = 0;

    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = I2C_BAUDRATE;
    sourceClock               = ACCEL_I2C_CLK_FREQ;
    I2C_MasterInit(I2C0, &masterConfig, sourceClock);
}

void BOARD_InitPins(void)
{
	/* Port E Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortE);
	/* PORTE24 (pin 31) is configured as I2C0_SCL */
	PORT_SetPinMux(PORTE, 24U, kPORT_MuxAlt5);

	/* PORTE25 (pin 32) is configured as I2C0_SDA */
	PORT_SetPinMux(PORTE, 25U, kPORT_MuxAlt5);
}

void BOARD_I2C_ConfigurePins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porte24_pin31_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C0_SCL */
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTE24 (pin 31) is configured as I2C0_SCL */
    PORT_SetPinConfig(PORTE, 24U, &porte24_pin31_config);

    const port_pin_config_t porte25_pin32_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C0_SDA */
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTE25 (pin 32) is configured as I2C0_SDA */
    PORT_SetPinConfig(PORTE, 25U, &porte25_pin32_config);
}
