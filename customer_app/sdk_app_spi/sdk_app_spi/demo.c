/*
 * Copyright (c) 2020 Bouffalolab.
 *
 * This file is part of
 *     *** Bouffalolab Software Dev Kit ***
 *      (see www.bouffalolab.com).
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Bouffalo Lab nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "demo.h"
#include <device/vfs_spi.h>  //  For spi_ioc_transfer_t
#include <hal/soc/spi.h>     //  For hal_spi_transfer
#include <hal_spi.h>
#include <cli.h>

/// Use SPI Port Number 0
#define SPI_PORT  0

/// SPI Port
static spi_dev_t *spi;

/// Init the SPI Port
static void test_spi_init(char *buf, int len, int argc, char **argv)
{
    //  SPI settings based on BL602 Device Tree: https://github.com/bouffalolab/BLOpenFlasher/blob/main/bl602/device_tree/bl_factory_params_IoTKitA_40M.dts
    spi = spi_init(
        SPI_PORT,             //  SPI Port
        HAL_SPI_MODE_MASTER,  //  SPI Mode
        0,                    //  SPI Polar Phase
        500 * 1000,           //  SPI Frequency (500 kHz). Previously 3 * 1000 * 0000
        2,  //  Transmit DMA Channel
        3,  //  Receive DMA Channel
        3,  //  SPI Clock Pin 
        2,  //  SPI Chip Select Pin
        1,  //  SPI Serial Data Out Pin (Previously MOSI)
        0   //  SPI Serial Data In Pin (Previously MISO)
    );
    assert(spi != NULL);

    //  TODO: int hal_spi_set_rwmode(spi_dev_t *spi_dev, int mode);
    //  TODO: int hal_spi_set_rwspeed(spi_dev_t *spi_dev, uint32_t speed);
}

/// Start the SPI data transfer
static void test_spi_transfer(char *buf, int len, int argc, char **argv)
{
    static spi_ioc_transfer_t trans;
    //  TODO: int hal_spi_transfer(spi_dev_t *spi_dev, void *xfer, uint8_t size);/* spi_ioc_transfer_t */
}

#ifdef NOTUSED
typedef struct spi_ioc_transfer {
    uint32_t   tx_buf;               /* uint64_t to uint32_t */
    uint32_t   rx_buf;               /* uint64_t to uint32_t */
    uint32_t   len;
    uint32_t   speed_hz;
    uint16_t   delay_usecs;          /* Unimplemented */
    uint16_t   delay_msecs;          /* delay ms, bl add*/
    uint8_t    bits_per_word;        /* Unimplemented */
    uint8_t    cs_change;            /* 0: Keep CS activated */
    uint8_t    tx_nbits;             /* Unimplemented */
    uint8_t    rx_nbits;             /* Unimplemented */
    uint8_t    word_delay_usecs;     /* Unimplemented */
    uint8_t    pad;                  /* Unimplemented */
} spi_ioc_transfer_t;
#endif  //  NOTUSED

// STATIC_CLI_CMD_ATTRIBUTE makes this(these) command(s) static
const static struct cli_command cmds_user[] STATIC_CLI_CMD_ATTRIBUTE = {
    {"spi_init", "Init SPI port", test_spi_init},
    {"spi_transfer", "Transfer SPI data", test_spi_transfer},
};                                                                                   

int i2c_cli_init(void)
{
    // static command(s) do NOT need to call aos_cli_register_command(s) to register.
    // However, calling aos_cli_register_command(s) here is OK but is of no effect as cmds_user are included in cmds list.
    // XXX NOTE: Calling this *empty* function is necessary to make cmds_user in this file to be kept in the final link.
    //return aos_cli_register_commands(cmds_user, sizeof(cmds_user)/sizeof(cmds_user[0]));          
    return 0;
}

#ifdef NOTUSED

#define HAL_SPI_MODE_MASTER 1  /* spi communication is master mode */
#define HAL_SPI_MODE_SLAVE  2  /* spi communication is slave mode */

typedef struct {
    uint8_t mode;           /* spi communication mode */
    uint32_t freq;          /* communication frequency Hz */
} spi_config_t;

typedef struct {
    uint8_t      port;    /* spi port */
    spi_config_t config;  /* spi config */
    void        *priv;    /* priv data */
} spi_dev_t;

#endif  //  NOTUSED