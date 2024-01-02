# STM32 HAL Driver for Pervasive Displays EPDs
 
STM32 CubeHAL driver for controlling small-sized EPDs from [Pervasive Displays Inc](https://www.pervasivedisplays.com/) ( 1.54", 2.06", 2.13", 2.66", 2.71", 2.87", 2.90", 3.70", 4.17", 4.37") with internal driving (iTC).

This driver supports monochrome screens only, and can use [Global Update and Fast Update](https://www.pervasivedisplays.com/how-e-paper-works/fast-and-partial-updates/) modes.

## Adding the driver to STM32CubeIDE

If you're using Git in your project, you can add this repo as a git submodule in the `Drivers` directory of your STM32 project. Alternatively, you can simply clone this repo into the `Drivers` folder.

After that, you need to add the path `Drivers/EPD_SmallSized_Driver_HAL/Inc` to your build paths. Go to your project properties (right-click on your project > Properties) then C/C++ General > Paths and Symbols and then add the path `Drivers/EPD_SmallSized_Driver_HAL/Inc`.

## Using the driver

With CubeMX or manually, you must configure the D/C, CS and RESET pins as outputs and the BUSY pin as an input. For SPI communication, use the Half Duplex Master mode, with software management of the SS signal and default parameters (Motorola frame format, 8 bits data size, MSB first, CPOL = 0 and CPHA = 0). SPI clock speed must not exceed 10 MHz (according to PDI).

You can now initialize the screen and use it in your project (simple example with Global Update mode on 2.66" EPD) :

```c
ePaperDisplay_t epd;
epd.spi = &hspi1;

// CubeMX pin and port constants
epd.panelBusy.pin = EPD_BUSY_Pin;
epd.panelBusy.port = EPD_BUSY_GPIO_Port;
epd.panelCS.pin = EPD_CS_Pin;
epd.panelCS.port = EPD_CS_GPIO_Port;
epd.panelDC.pin = EPD_DC_Pin;
epd.panelDC.port = EPD_DC_GPIO_Port;
epd.panelReset.pin = EPD_RST_Pin;
epd.panelReset.port = EPD_RST_GPIO_Port;

// EPD type and PSR constants for each screen
epd.epdType = EPD_DISPLAY_266;
epd.displaySizeH = EPD_DISPLAY_266_SIZE_H;
epd.displaySizeV = EPD_DISPLAY_266_SIZE_V;

epd.epdPSR[0] = EPD_DISPLAY_266_PSR0;
epd.epdPSR[1] = EPD_DISPLAY_266_PSR1;

initializeGlobalUpdateCOG(&epd);
globalUpdateDisplay(&epd, /* IMAGE BUFFER HERE */, /* 0x00 BUFFER HERE */);
powerOffCOG(&epd);
``` 

This driver only handles the display of screen-size buffers using the globalUpdateDisplay and fastUpdateDisplay functions. A separate GUI library is required (and must be integrated) to use the screen dynamically (LVGL, µGUI...).

## Related projects and documentation
- [PDI Wiki](https://docs.pervasivedisplays.com/) 
- [EPD_Driver_GF_small](https://github.com/PervasiveDisplays/EPD_Driver_GF_small/) : Officiel driver from PDI using the Arduino framework for PS/KS-type small-sized 1.54", 2.13", 2.66", 2.71", 2.87", 2.92", 3.70", 4.17" and 4.37" screens

## Disclaimer

This project is in no way affiliated with Pervasive Displays Inc. Furthermore, I was only able to test the driver on a 2.66" screen, so I can't guarantee that it will work on another EPD from PDI. Please let me know if you've managed to get it to work on other compatible screens!
