/**
 * @file        epd_driver.c
 * @brief       EPD driver source
 * @author      Esteban CADIC
 * @version     1.0
 * @date        2024
 * @copyright   MIT License
 *
 */

#include <stdio.h>
#include "epd_driver.h"
#include "epd_screens.h"

// TODO : manage the display power supply (VCC) using an external pin.

/*------------------------------------------------------------------------------
	CONSTANTS
------------------------------------------------------------------------------*/

#define EPD_SOFT_RESET_INDEX		0
#define EPD_TSSET_INDEX				1
#define EPD_ACTIVE_TEMP_INDEX		2
#define EPD_CDI_INDEX				3
#define EPD_BORDER_SETTING_INDEX	4

#define EPD_SOFT_RESET				0x0E // Const
#define EPD_TSSET 					25 // Temperature of 25°C
#define EPD_ACTIVE_TEMP				0x02 // Const
#define EPD_CDI						0x07 // Const, only for 1.54", 2.13", 2.66", 3.7" and 4.37"
#define EPD_BORDER_SETTING			0x27 // Const, only for 1.54", 2.13", 2.66", 3.7" and 4.37"

static uint8_t EPD_CONSTANTS[3] = { EPD_SOFT_RESET, EPD_TSSET, EPD_ACTIVE_TEMP };
static uint8_t EPD_FAST_CONSTANTS[5] = { EPD_SOFT_RESET, EPD_TSSET + 0x40, EPD_ACTIVE_TEMP, EPD_CDI, EPD_BORDER_SETTING };


/*------------------------------------------------------------------------------
	PROTOTYPES
------------------------------------------------------------------------------*/

static void powerOnCOG(ePaperDisplay_t*);
static void transmitData(ePaperDisplay_t*, uint8_t, uint8_t*, uint16_t);
static void updateCOG(ePaperDisplay_t*);


/*------------------------------------------------------------------------------
	FONCTIONS
------------------------------------------------------------------------------*/

void initializeGlobalUpdateCOG(ePaperDisplay_t *epd)
{
	HAL_GPIO_WritePin(epd->panelReset.port, epd->panelReset.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(epd->panelCS.port, epd->panelCS.pin, GPIO_PIN_SET);

	/* COG Power On */
	powerOnCOG(epd);

	/* Set environment and PSR */
	transmitData(epd, 0xE5, EPD_CONSTANTS + EPD_TSSET_INDEX, 1); // Input Temperature
	transmitData(epd, 0xE0, EPD_CONSTANTS + EPD_ACTIVE_TEMP_INDEX, 1); // Active Temperature

	if(epd->epdType == EPD_DISPLAY_290) // 2.90" EPD (no PSR)
	{
		uint8_t epd290settings[2] = { 0x55, 0x02 };
		transmitData(epd, 0x4D, epd290settings + 0, 1);
		transmitData(epd, 0xE9, epd290settings + 1, 1);
	}
	else
	{
		transmitData(epd, 0x00, epd->epdPSR, 2); // Panel Settings (PSR may change between displays!)
	}
}

void initializeFastUpdateCOG(ePaperDisplay_t *epd)
{
	HAL_GPIO_WritePin(epd->panelReset.port, epd->panelReset.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(epd->panelCS.port, epd->panelCS.pin, GPIO_PIN_SET);

	/* COG Power On */
	powerOnCOG(epd);

	/* Set environment and PSR */
	transmitData(epd, 0xE5, EPD_FAST_CONSTANTS + EPD_TSSET_INDEX, 1); // Input Temperature
	transmitData(epd, 0xE0, EPD_FAST_CONSTANTS + EPD_ACTIVE_TEMP_INDEX, 1); // Active Temperature

	if(epd->epdType == EPD_DISPLAY_290) // 2.90" EPD (no PSR)
	{
		uint8_t epd290settings[2] = { 0x55, 0x02 };
		transmitData(epd, 0x4D, epd290settings + 0, 1);
		transmitData(epd, 0xE9, epd290settings + 1, 1);
	}
	else
	{
		uint8_t psrFastUpdate[2] = { epd->epdPSR[0] |= 0x10, epd->epdPSR[1] |= 0x02 };

		transmitData(epd, 0x00, psrFastUpdate, 2); // Panel Settings (PSR may change between displays!)
	}

	transmitData(epd, 0x50, EPD_FAST_CONSTANTS + EPD_CDI_INDEX, 1); // Vcom and data interval settings
}

void globalUpdateDisplay(ePaperDisplay_t *epd, uint8_t *dispBuffer1, uint8_t *dispBuffer2)
{
	uint32_t displaySize = epd->displaySizeH * epd->displaySizeV / 8;
	// uint32_t only for the multiplication. 16 bits are enough for a 724² pixel WB screen.

	transmitData(epd, 0x10, dispBuffer1, (uint16_t) displaySize);
	transmitData(epd, 0x13, dispBuffer2, (uint16_t) displaySize);

	updateCOG(epd);
}

void fastUpdateDisplay(ePaperDisplay_t *epd, uint8_t *dispBuffer1, uint8_t *dispBuffer2)
{
	uint32_t displaySize = epd->displaySizeH * epd->displaySizeV / 8;
	// uint32_t only for the multiplication. 16 bits are enough for a 724² pixel WB screen.

	if(epd->epdType == EPD_DISPLAY_154 || epd->epdType == EPD_DISPLAY_213 || epd->epdType == EPD_DISPLAY_266 || epd->epdType == EPD_DISPLAY_370 || epd->epdType == EPD_DISPLAY_437)
	{
		// Only for 1.54", 2.13", 2.66", 3.7" and 4.37" EPDs
		transmitData(epd, 0x50, EPD_FAST_CONSTANTS + EPD_BORDER_SETTING_INDEX, 1); // Border setting
	}

	transmitData(epd, 0x10, dispBuffer1, (uint16_t) displaySize);
	transmitData(epd, 0x13, dispBuffer2, (uint16_t) displaySize);

	if(epd->epdType == EPD_DISPLAY_154 || epd->epdType == EPD_DISPLAY_213 || epd->epdType == EPD_DISPLAY_266 || epd->epdType == EPD_DISPLAY_370 || epd->epdType == EPD_DISPLAY_437)
	{
		// Only for 1.54", 2.13", 2.66", 3.7" and 4.37" EPDs
		transmitData(epd, 0x50, EPD_FAST_CONSTANTS + EPD_CDI_INDEX, 1); // CDI
	}

	updateCOG(epd);
}

void powerOffCOG(ePaperDisplay_t *epd)
{
	while(HAL_GPIO_ReadPin(epd->panelBusy.port, epd->panelBusy.pin) != GPIO_PIN_SET);

	transmitData(epd, 0x02, 0, 0);
	while(HAL_GPIO_ReadPin(epd->panelBusy.port, epd->panelBusy.pin) != GPIO_PIN_SET);

	HAL_GPIO_WritePin(epd->panelDC.port, epd->panelDC.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(epd->panelCS.port, epd->panelCS.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(epd->panelReset.port, epd->panelReset.pin, GPIO_PIN_RESET);
}

/**
 * @brief Power on EPD COG.
 *
 * @param epd Pointer to EPD structure
 */
static void powerOnCOG(ePaperDisplay_t *epd)
{
	HAL_Delay(5);
	HAL_GPIO_WritePin(epd->panelReset.port, epd->panelReset.pin, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(epd->panelReset.port, epd->panelReset.pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(epd->panelReset.port, epd->panelReset.pin, GPIO_PIN_SET);
	HAL_Delay(5);
	transmitData(epd, 0x00, EPD_CONSTANTS + EPD_SOFT_RESET_INDEX, 1);

	while(HAL_GPIO_ReadPin(epd->panelBusy.port, epd->panelBusy.pin) != GPIO_PIN_SET);
}

/**
 * @brief Transmits SPI data to screen (with CS and D/C management)
 *
 * @param epd Pointer to EPD structure
 * @param cmd Register index
 * @param data Pointer to data to be sent to screen
 * @param dataSize Size of data to be sent
 */
static void transmitData(ePaperDisplay_t *epd, uint8_t cmd, uint8_t *data, uint16_t dataSize)
{
	// CMD
	uint8_t cmdSPIArray[1] = { cmd };

	HAL_GPIO_WritePin(epd->panelDC.port, epd->panelDC.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(epd->panelCS.port, epd->panelCS.pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(epd->spi, cmdSPIArray, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(epd->panelCS.port, epd->panelCS.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(epd->panelDC.port, epd->panelDC.pin, GPIO_PIN_SET);

	// DATA
	HAL_GPIO_WritePin(epd->panelCS.port, epd->panelCS.pin, GPIO_PIN_RESET);

	if(dataSize != 0)
		HAL_SPI_Transmit(epd->spi, data, dataSize, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(epd->panelCS.port, epd->panelCS.pin, GPIO_PIN_SET);
}

/**
 * @brief Command the COG to update the display.
 *
 * @param epd Pointer to EPD structure
 */
static void updateCOG(ePaperDisplay_t *epd)
{
	while(HAL_GPIO_ReadPin(epd->panelBusy.port, epd->panelBusy.pin) != GPIO_PIN_SET);

	transmitData(epd, 0x04, 0, 0);
	while(HAL_GPIO_ReadPin(epd->panelBusy.port, epd->panelBusy.pin) != GPIO_PIN_SET);

	transmitData(epd, 0x12, 0, 0);
	while(HAL_GPIO_ReadPin(epd->panelBusy.port, epd->panelBusy.pin) != GPIO_PIN_SET);
}
