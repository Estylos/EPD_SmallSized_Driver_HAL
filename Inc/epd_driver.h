/**
 * @file        epd_driver.h
 * @brief       EPD driver header
 * @author      Esteban CADIC
 * @version     1.0
 * @date        2024
 * @copyright   MIT License
 *
 */

#ifndef INC_EPD_DRIVER_H_
#define INC_EPD_DRIVER_H_

#include <stdint.h>
#include "main.h" // To include the correct HAL header


/*------------------------------------------------------------------------------
	TYPE DEFINITIONS
------------------------------------------------------------------------------*/

/**
 * @brief Struct representing a GPIO (for use with the HAL).
 *
 */
struct pinState {
	GPIO_TypeDef *port;
	uint16_t pin;
};


/**
 * @brief Struct representing a PDI E-Paper screen.
 *
 */
typedef struct ePaperDisplay {
	SPI_HandleTypeDef *spi; /**< SPI peripheral */

	struct pinState panelReset; /**< Reset GPIO */
	struct pinState panelDC; /**< D/C GPIO */
	struct pinState panelBusy; /**< BUSY GPIO */
	struct pinState panelCS; /**< Panel CS GPIO */

	uint8_t epdType; /**< EPD type (constants EPD_DISPLAY_XXX from epd_screens.h) */
	uint16_t displaySizeH; /**< Horizontal display size (constants from epd_screens.h) */
	uint16_t displaySizeV; /**< Vertical display size (constants from epd_screens.h) */
	uint8_t epdPSR[2]; /**< PSR constant of the EPD (constants from epd_screens.h) */
} ePaperDisplay_t;


/*------------------------------------------------------------------------------
	DECLARATIONS
------------------------------------------------------------------------------*/

/**
 * @brief Initializes EPD COG (Chip-On-Glass) with Global Update mode.
 * Switches on display and sends environment temperature and PSR to COG.
 *
 * @param epd Pointer to EPD structure
 */
extern void initializeGlobalUpdateCOG(ePaperDisplay_t *);

/**
 * @brief Initializes EPD COG (Chip-On-Glass) with Fast Update mode (only for small-sized PS and KS-type EPDs)
 * Switches on display and sends environment temperature and PSR to COG.
 *
 * @param epd Pointer to EPD structure
 */
extern void initializeFastUpdateCOG(ePaperDisplay_t *);

/**
 * @brief Sends and displays a new image to the EPD with full screen refresh (Global Update).
 * The driver only supports monochrome screens. An array bit set to 1 is black, set to 0 is white.
 *
 * @param epd Pointer to EPD structure
 * @param dispBuffer1 Pointer to a screen-sized buffer containing the new image to be displayed
 * @param dispBuffer2 Pointer to a screen-sized buffer dummy data. The buffer must be filled with 0x00.
 */
extern void globalUpdateDisplay(ePaperDisplay_t *, uint8_t *, uint8_t *);

/**
 * @brief Sends and displays a new image to the EPD with partial screen refresh (Fast Update).
 * The driver only supports monochrome screens. An array bit set to 1 is black, set to 0 is white.
 *
 * @param epd Pointer to EPD structure
 * @param dispBuffer1 Pointer to a screen-sized buffer containing the new image to be displayed
 * @param dispBuffer2 Pointer to a screen-sized buffer dummy data. The buffer must be filled with 0x00
 */
extern void fastUpdateDisplay(ePaperDisplay_t *, uint8_t *, uint8_t *);

/**
 * @brief Turn off COG DC/DC.
 * Can be called after each screen update.
 *
 * @param epd Pointer to EPD structure
 */
extern void powerOffCOG(ePaperDisplay_t *);

#endif /* INC_EPD_DRIVER_H_ */
