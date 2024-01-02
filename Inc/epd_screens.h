/**
 * @file        epd_screens.h
 * @brief       Constants for small-sized EPDs
 * @author      Esteban CADIC
 * @version     1.0
 * @date        2024
 * @copyright   MIT License
 *
 */

#ifndef INC_EPD_SCREENS_H_
#define INC_EPD_SCREENS_H_


/*------------------------------------------------------------------------------
	CONSTANTS
------------------------------------------------------------------------------*/

/* EPD SCREENS */

#define EPD_DISPLAY_154				0	/** 1.54" EPD */
#define EPD_DISPLAY_206				1	/** 2.06" EPD */
#define EPD_DISPLAY_213				2	/** 2.13" EPD */
#define EPD_DISPLAY_266				3	/** 2.66" EPD */
#define EPD_DISPLAY_271				4	/** 2.71" EPD */
#define EPD_DISPLAY_287				5	/** 2.87" EPD */
#define EPD_DISPLAY_290				6	/** 2.90" EPD */
#define EPD_DISPLAY_370				7	/** 3.70" EPD */
#define EPD_DISPLAY_417				8	/** 4.17" EPD */
#define EPD_DISPLAY_437				9	/** 4.37" EPD */

/* EPD CONSTANTS */
/* EPD_PSR0, EPD_PSR1, EPD_SIZE_H, EPD_SIZE_V */

#define EPD_DISPLAY_154_PSR0 		0xCF
#define EPD_DISPLAY_154_PSR1		0x02
#define EPD_DISPLAY_154_SIZE_H 		152
#define EPD_DISPLAY_154_SIZE_V 		152

#define EPD_DISPLAY_206_PSR0		0xCF
#define EPD_DISPLAY_206_PSR1		0x02
#define EPD_DISPLAY_206_SIZE_H		128
#define EPD_DISPLAY_206_SIZE_V		248

#define EPD_DISPLAY_213_PSR0		0xCF
#define EPD_DISPLAY_213_PSR1		0x02
#define EPD_DISPLAY_213_SIZE_H		104
#define EPD_DISPLAY_213_SIZE_V		212

#define EPD_DISPLAY_266_PSR0		0xCF
#define EPD_DISPLAY_266_PSR1		0x02
#define EPD_DISPLAY_266_SIZE_H		152
#define EPD_DISPLAY_266_SIZE_V		296

#define EPD_DISPLAY_271_PSR0		0xCF
#define EPD_DISPLAY_271_PSR1		0x8D
#define EPD_DISPLAY_271_SIZE_H		176
#define EPD_DISPLAY_271_SIZE_V		264

#define EPD_DISPLAY_287_PSR0		0xCF
#define EPD_DISPLAY_287_PSR1		0x8D
#define EPD_DISPLAY_287_SIZE_H		128
#define EPD_DISPLAY_287_SIZE_V		196

#define EPD_DISPLAY_290_PSR0		0x00
#define EPD_DISPLAY_290_PSR1		0x00
#define EPD_DISPLAY_290_SIZE_H		168
#define EPD_DISPLAY_290_SIZE_V		386

#define EPD_DISPLAY_370_PSR0		0xCF
#define EPD_DISPLAY_370_PSR1		0x8F
#define EPD_DISPLAY_370_SIZE_H		240
#define EPD_DISPLAY_370_SIZE_V		416

#define EPD_DISPLAY_417_PSR0		0x0F
#define EPD_DISPLAY_417_PSR1		0x0E
#define EPD_DISPLAY_417_SIZE_H		400
#define EPD_DISPLAY_417_SIZE_V		300

#define EPD_DISPLAY_437_PSR0		0x0F
#define EPD_DISPLAY_437_PSR1		0x0E
#define EPD_DISPLAY_437_SIZE_H		176
#define EPD_DISPLAY_437_SIZE_V		480

#endif /* INC_EPD_SCREENS_H_ */
