/*!
 * \file      sx126x-board.h
 *
 * \brief     Target board SX126x driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#ifndef __SX126x_BOARD_H__
#define __SX126x_BOARD_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "sx126x.h"

/* Connect BL602 to SX1262 LoRa Transceiver
| BL602 Pin     | SX1262 Pin          | Wire Colour 
|:--------------|:--------------------|:-------------------
////| __`GPIO 0`__  | `DIO1`              | Dark Green
| __`GPIO 1`__  | `ISO` _(MISO)_      | Light Green (Top)
////| __`GPIO 2`__  | Do Not Connect      | (Unused Chip Select)
////| __`GPIO 3`__  | `SCK`               | Yellow (Top)
////| __`GPIO 4`__  | `OSI` _(MOSI)_      | Blue (Top)
////| __`GPIO 5`__  | `DIO2`              | Blue (Bottom)
| __`GPIO 11`__ | `DIO0`              | Yellow (Bottom)
| __`GPIO 12`__ | `DIO3`              | Light Green (Bottom)
| __`GPIO 14`__ | `NSS`               | Orange
| __`GPIO 17`__ | `RST`               | White
| __`3V3`__     | `3.3V`              | Red
| __`GND`__     | `GND`               | Black
#define SX126X_RADIO_BUSY_PIN   -1  //  TODO
#define SX126X_RADIO_DEVICE_SEL -1  //  TODO
*/

#define SX126X_SPI_IDX      0  //  SPI Port 0
#define SX126X_SPI_SDI_PIN  1  //  SPI Serial Data In Pin  (formerly MISO)
#define SX126X_SPI_SDO_PIN  4  //  SPI Serial Data Out Pin (formerly MOSI)
#define SX126X_SPI_CLK_PIN  3  //  SPI Clock Pin
#define SX126X_SPI_CS_PIN  14  //  SPI Chip Select Pin
#define SX126X_SPI_CS_OLD   2  //  Unused SPI Chip Select Pin
#define SX126X_NRESET      17  //  Reset Pin
////#define SX126X_DIO0        11  //  DIO0: Trigger for Packet Received
#define SX126X_DIO1         0  //  DIO1: Trigger for Sync Timeout
////#define SX126X_DIO2         5  //  DIO2: Trigger for Change Channel (Spread Spectrum / Frequency Hopping)
////#define SX126X_DIO3        12  //  DIO3: Trigger for CAD Done
////#define SX126X_DIO4        -1  //  DIO4: Unused (FSK only)
////#define SX126X_DIO5        -1  //  DIO5: Unused (FSK only)
#define SX126X_SPI_BAUDRATE  (200 * 1000)  //  SPI Frequency (200 kHz)
////#define SX126X_LF_USE_PA_BOOST  1  //  Enable Power Amplifier Boost for LoRa Frequency below 525 MHz
////#define SX126X_HF_USE_PA_BOOST  1  //  Enable Power Amplifier Boost for LoRa Frequency 525 MHz and above

#define SX126X_BUSY_PIN   -1  //  TODO
#define SX126X_DEVICE_SEL_PIN -1  //  TODO

//  CAD = Channel Activity Detection. We detect whether a Radio Channel 
//  is in use, by scanning very quickly for the LoRa Packet Preamble.

/*!
 * \brief Initializes the radio I/Os pins interface
 */
void SX126xIoInit( void );

/*!
 * \brief Initializes DIO IRQ handlers
 *
 * \param [IN] irqHandlers Array containing the IRQ callback functions
 */
void SX126xIoIrqInit( DioIrqHandler dioIrq );

/*!
 * \brief De-initializes the radio I/Os pins interface.
 *
 * \remark Useful when going in MCU low power modes
 */
void SX126xIoDeInit( void );

/*!
 * \brief Initializes the TCXO power pin.
 */
void SX126xIoTcxoInit( void );

/*!
 * \brief Initializes RF switch control pins.
 */
void SX126xIoRfSwitchInit( void );

/*!
 * \brief Initializes the radio debug pins.
 */
void SX126xIoDbgInit( void );

/*!
 * \brief HW Reset of the radio
 */
void SX126xReset( void );

/*!
 * \brief Blocking loop to wait while the Busy pin in high
 */
void SX126xWaitOnBusy( void );

/*!
 * \brief Wakes up the radio
 */
void SX126xWakeup( void );

/*!
 * \brief Send a command that write data to the radio
 *
 * \param [in]  opcode        Opcode of the command
 * \param [in]  buffer        Buffer to be send to the radio
 * \param [in]  size          Size of the buffer to send
 */
void SX126xWriteCommand( RadioCommands_t opcode, uint8_t *buffer, uint16_t size );

/*!
 * \brief Send a command that read data from the radio
 *
 * \param [in]  opcode        Opcode of the command
 * \param [out] buffer        Buffer holding data from the radio
 * \param [in]  size          Size of the buffer
 *
 * \retval status Return command radio status
 */
uint8_t SX126xReadCommand( RadioCommands_t opcode, uint8_t *buffer, uint16_t size );

/*!
 * \brief Write a single byte of data to the radio memory
 *
 * \param [in]  address       The address of the first byte to write in the radio
 * \param [in]  value         The data to be written in radio's memory
 */
void SX126xWriteRegister( uint16_t address, uint8_t value );

/*!
 * \brief Read a single byte of data from the radio memory
 *
 * \param [in]  address       The address of the first byte to write in the radio
 *
 * \retval      value         The value of the byte at the given address in radio's memory
 */
uint8_t SX126xReadRegister( uint16_t address );

/*!
 * \brief Sets the radio output power.
 *
 * \param [IN] power Sets the RF output power
 */
void SX126xSetRfTxPower( int8_t power );

/*!
 * \brief Gets the device ID
 *
 * \retval id Connected device ID
 */
uint8_t SX126xGetDeviceId( void );

/*!
 * \brief Initializes the RF Switch I/Os pins interface
 */
void SX126xAntSwOn( void );

/*!
 * \brief De-initializes the RF Switch I/Os pins interface
 *
 * \remark Needed to decrease the power consumption in MCU low power modes
 */
void SX126xAntSwOff( void );

/*!
 * \brief Checks if the given RF frequency is supported by the hardware
 *
 * \param [IN] frequency RF frequency to be checked
 * \retval isSupported [true: supported, false: unsupported]
 */
bool SX126xCheckRfFrequency( uint32_t frequency );

/*!
 * \brief Gets the Defines the time required for the TCXO to wakeup [ms].
 *
 * \retval time Board TCXO wakeup time in ms.
 */
uint32_t SX126xGetBoardTcxoWakeupTime( void );

/*!
 * \brief Gets current state of DIO1 pin state.
 *
 * \retval state DIO1 pin current state.
 */
uint32_t SX126xGetDio1PinState( void );

/*!
 * \brief Gets the current Radio OperationMode variable
 *
 * \retval      RadioOperatingModes_t last operating mode
 */
RadioOperatingModes_t SX126xGetOperatingMode( void );

/*!
 * \brief Sets/Updates the current Radio OperationMode variable.
 *
 * \remark WARNING: This function is only required to reflect the current radio
 *                  operating mode when processing interrupts.
 *
 * \param [in] mode           New operating mode
 */
void SX126xSetOperatingMode( RadioOperatingModes_t mode );

/*!
 * Radio hardware and global parameters
 */
extern SX126x_t SX126x;

#ifdef __cplusplus
}
#endif

#endif // __SX126x_BOARD_H__