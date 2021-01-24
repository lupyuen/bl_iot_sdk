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
#include <bl_i2c.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "demo.h"
#include <bl_i2c.h>
#include <hal_i2c.h>
#include <bl602_i2c.h>
#include <bl_irq.h>
#include <cli.h>

int i2c_data_test(void)
{
    int i;
    int flag;
    i2c_msg_t msgs[6];
    TickType_t xdelay;
    int data_len;

    uint8_t testarr[32];
    uint8_t recvarr01[32];
    uint8_t recvarr02[32];
    uint8_t recvarr03[32];

    xdelay= 80 / portTICK_PERIOD_MS;
    for (i = 0; i < 32; i++) {
        testarr[i] = 100 - i * 2;
    }


    //write
    data_len = 31;
    msgs[0].addr = 0x50;
    msgs[0].subflag = 1;
    msgs[0].subaddr = 0x04;
    msgs[0].buf = testarr;
    msgs[0].direct = I2C_M_WRITE;
    msgs[0].block = I2C_M_BLOCK;
    msgs[0].len = data_len;
    msgs[0].idex = 0;
    msgs[0].sublen = 2;
    msgs[0].i2cx = 0;
    i2c_transfer_msgs_block(msgs, 1, 0);

    vTaskDelay(200 / portTICK_PERIOD_MS);
    data_len = 31;
    msgs[0].addr = 0x50;
    msgs[0].subflag = 1;
    msgs[0].subaddr = 0x24;
    msgs[0].buf = testarr;
    msgs[0].direct = I2C_M_WRITE;
    msgs[0].block = I2C_M_BLOCK;
    msgs[0].len = data_len;
    msgs[0].idex = 0;
    msgs[0].sublen = 2;
    msgs[0].i2cx = 0;
    i2c_transfer_msgs_block(msgs, 1, 0);

    vTaskDelay(200 / portTICK_PERIOD_MS);
    data_len = 31;
    msgs[0].addr = 0x50;
    msgs[0].subflag = 1;
    msgs[0].subaddr = 0x44;
    msgs[0].buf = testarr;
    msgs[0].direct = I2C_M_WRITE;
    msgs[0].block = I2C_M_BLOCK;
    msgs[0].len = data_len;
    msgs[0].idex = 0;
    msgs[0].sublen = 2;
    msgs[0].i2cx = 0;
    i2c_transfer_msgs_block(msgs, 1, 0);

    vTaskDelay(200 / portTICK_PERIOD_MS);


    //read
    data_len = 31;
    memset(recvarr01, 0, 32);
    msgs[1].addr = 0x50;
    msgs[1].subflag = 1;
    msgs[1].subaddr = 0x04;
    msgs[1].buf = recvarr01;
    msgs[1].direct = I2C_M_READ;
    msgs[1].block = I2C_M_BLOCK;
    msgs[1].len = data_len;
    msgs[1].idex = 0;
    msgs[1].sublen = 2;
    msgs[1].i2cx = 0;

    data_len = 31;
    msgs[2].addr = 0x50;
    msgs[2].subflag = 1;
    msgs[2].subaddr = 0x24;
    msgs[2].buf = recvarr02;
    msgs[2].direct = I2C_M_READ;
    msgs[2].block = I2C_M_BLOCK;
    msgs[2].len = data_len;
    msgs[2].idex = 0;
    msgs[2].sublen = 2;
    msgs[2].i2cx = 0;

    msgs[3].addr = 0x50;
    msgs[3].subflag = 1;
    msgs[3].subaddr = 0x44;
    msgs[3].buf = recvarr03;
    msgs[3].direct = I2C_M_READ;
    msgs[3].block = I2C_M_BLOCK;
    msgs[3].len = data_len;
    msgs[3].idex = 0;
    msgs[3].sublen = 2;
    msgs[3].i2cx = 0;


    i2c_transfer_msgs_block(&msgs[1], 3, 0); 

    for(i = 0; i < 31; i++) {
        printf("test[%d] = %d, recv[%d] = %d \r\n", i, testarr[i], i, recvarr01[i]);
    }      
    flag = memcmp(testarr, recvarr01, 31);
    if (flag == 0) {
        printf("data correct \r\n");
    } else {
        printf("data not correct \r\n");
    }


    for(i = 0; i < 31; i++) {
        printf("test[%d] = %d, recv[%d] = %d \r\n", i, testarr[i], i, recvarr02[i]);
    }    
    flag = memcmp(testarr, recvarr02, data_len);
    if (flag == 0) {
        printf("data correct \r\n");
    } else {
        printf("data not correct \r\n");
    }


    for(i = 0; i < 31; i++) {
        printf("test[%d] = %d, recv[%d] = %d \r\n", i, testarr[i], i, recvarr03[i]);
    }
    flag = memcmp(testarr, recvarr03, data_len);
    if (flag == 0) {
        printf("data correct \r\n");
    } else {
        printf("data not correct \r\n");
    }

    return 0;
}

static void test_i2c_api(char *buf, int len, int argc, char **argv)
{
    int i = 0;
    uint8_t test_arr[32];
    uint8_t recv_arr[32];
    static uint8_t recv_nob[32];

    for (i = 0; i < 32; i++) {
        test_arr[i] = i + 3 * i;
    }
    
    hal_i2c_write_block(0x50, (char *)test_arr, 32, 2, 0x04);
    vTaskDelay(80 / portTICK_PERIOD_MS);//eeprom, when write ,should be delay ,then it could be read
    
    hal_i2c_read_block(0x50, (char *)recv_arr, 32, 2, 0x04);

    for (i = 0; i < 32; i++) {
        printf("test[%d] = %d  recv_arr[%d] = %d \r\n", i, test_arr[i], i, recv_arr[i]);
    }

    hal_i2c_write_no_block(0x50, (char *)test_arr, 32, 2, 0x04);
    vTaskDelay(80 / portTICK_PERIOD_MS);
    memset(recv_nob, 0, 32);
    hal_i2c_read_no_block(0x50, (char *)recv_nob, 32, 2, 0x04);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    for (i = 0; i < 32; i++) {
        printf("test[%d] = %d  recv_nob[%d] = %d \r\n", i, test_arr[i], i, recv_nob[i]);
    }

    return;
}

///////////////////////////////////////////////////////////////////////////////
//  Test Functions for Low Level I2C HAL (bl_i2c.c)

/// Global pointer to current I2C Message
static i2c_msg_t *gpstmsg;

/// Messages for sending and receiving I2C Data
static i2c_msg_t recv_msg;
#ifdef NOTUSED
static i2c_msg_t send_msg;
#endif  //  NOTUSED

/// Buffers for sending and receiving I2C Data
static uint8_t recv_buf[32];
#ifdef NOTUSED
static uint8_t send_buf[32];
#endif  //  NOTUSED

/// Interrupt Counters
static int count_int, count_rfx, count_end, count_nak, count_txf, count_arb, count_fer, count_unk;

/// Stop the I2C Transfer. Called by I2C Interrupt Handler. Based on i2c_callback in hal_i2c.c
static void test_i2c_callback(i2c_msg_t *pstmsg)
{
    I2C_Disable(pstmsg->i2cx);
    I2C_IntMask(pstmsg->i2cx, I2C_INT_ALL, MASK);
    i2c_clear_status(pstmsg->i2cx);
}

/// For Rx FIFO Ready and Tx FIFO Ready, transfer 32 bits of data. Based on i2c_transferbytes in hal_i2c.c
static void test_i2c_transferbytes(i2c_msg_t *pstmsg)
{
    if ((pstmsg->direct == I2C_M_WRITE) && (pstmsg->event = EV_I2C_TXF_INT)) {
        if (pstmsg->idex < pstmsg->len) {
            do_write_data(pstmsg);
        } else if (pstmsg->idex == pstmsg->len) {
            I2C_IntMask(pstmsg->i2cx, I2C_TX_FIFO_READY_INT, MASK);
            return;
        } else {
        } 
    } else if ((pstmsg->direct == I2C_M_READ) && (pstmsg->event = EV_I2C_RXF_INT)){
        if (pstmsg->idex < pstmsg->len) {
             do_read_data(pstmsg);      
        } else {
            I2C_IntMask(pstmsg->i2cx, I2C_RX_FIFO_READY_INT, MASK);
            return;
        } 
    } else {
    }
}

/// I2C Interrupt Handler. Based on i2c_interrupt_entry in hal_i2c.c
static void test_i2c_interrupt_entry(void *ctx)
{
    i2c_msg_t *pstmsg = *((i2c_msg_t **)ctx);
    uint32_t tmpval = BL_RD_REG(I2C_BASE, I2C_INT_STS);

    //  Increment the Interrupt Counters
    count_int++;  //  Overall interrupts
    if (BL_IS_REG_BIT_SET(tmpval,I2C_RXF_INT)) {
        //  Rx FIFO Ready
        count_rfx++;
        pstmsg->event = EV_I2C_RXF_INT;
        //  Should not return
    } else if (BL_IS_REG_BIT_SET(tmpval, I2C_END_INT)) {
        //  Transfer End
        count_end++;
        pstmsg->event = EV_I2C_END_INT;
        test_i2c_callback(pstmsg);
        return;  //  Stop now
    } else if (BL_IS_REG_BIT_SET(tmpval, I2C_NAK_INT)) {
        //  I2C NACK
        count_nak++;  
        pstmsg->event = EV_I2C_NAK_INT;
        test_i2c_callback(pstmsg);
        return;  //  Stop now
    } else if (BL_IS_REG_BIT_SET(tmpval, I2C_TXF_INT)) {
        //  Tx FIFO Ready
        count_txf++;  
        pstmsg->event = EV_I2C_TXF_INT;
        //  Should not return
    } else if (BL_IS_REG_BIT_SET(tmpval, I2C_ARB_INT)) {
        //  Arbitration Lost
        count_arb++;  
        pstmsg->event = EV_I2C_ARB_INT;
        test_i2c_callback(pstmsg);
        return;  //  Stop now
    } else if (BL_IS_REG_BIT_SET(tmpval,I2C_FER_INT)) {
        //  FIFO Error
        count_fer++;  
        pstmsg->event = EV_I2C_FER_INT;
        test_i2c_callback(pstmsg);
        return;  //  Stop now
    } else {
        //  Unknown Error
        count_unk++;  
        pstmsg->event = EV_I2C_UNKNOW_INT; 
        test_i2c_callback(pstmsg);
        //  Should not return
    }

    //  For Rx FIFO Ready and Tx FIFO Ready, transfer 32 bits of data
    test_i2c_transferbytes(pstmsg);
}

/// Dump the I2C Interrupt Counters
static void test_i2c_status(char *buf, int len, int argc, char **argv)
{
    printf("Interrupts: %d\r\n", count_int);
    printf("Trans End:  %d\r\n", count_end);
    printf("Tx Ready:   %d\r\n", count_txf);
    printf("Rx Ready:   %d\r\n", count_rfx);
    printf("NACK:       %d\r\n", count_nak);
    printf("Arb Lost:   %d\r\n", count_arb);
    printf("FIFO Error: %d\r\n", count_fer);
    printf("Unknown:    %d\r\n", count_unk);
}

/// Init I2C functions. Based on hal_i2c_init in hal_i2c.c
static void test_i2c_init(char *buf, int len, int argc, char **argv)
{
    //  Use I2C Port 0
    const int i2cx = 0;

    //  Init I2C Port 0 to GPIO 3 and 4
    i2c_gpio_init(i2cx);

    //  Set I2C Port 0 to 500 kbps
    i2c_set_freq(500, i2cx);

    //  Disable I2C Port 0
    I2C_Disable(i2cx);    

    //  Enable I2C interrupts   
    bl_irq_enable(I2C_IRQn);
    I2C_IntMask(i2cx, I2C_INT_ALL, MASK);
 
    //  Register the I2C Interrupt Handler
    bl_irq_register_with_ctx(I2C_IRQn, test_i2c_interrupt_entry, &gpstmsg);
}

static void test_i2c_clear_status(char *buf, int len, int argc, char **argv)
{
    //  Clear status for I2C Port 0
    i2c_clear_status(0);
}

#ifdef NOTUSED
static void test_i2c_start_write(char *buf, int len, int argc, char **argv)
{
    //  Start writing data to I2C device
    int data_len = 1;
    send_buf[0] = 0xd0;  //  BME280 Chip ID Register

    //  Init the I2C message
    //  send_msg.addr = 0x76;  //  BME280 I2C Primary Address
    send_msg.addr = 0x77;      //  BME280 I2C Secondary Address
    send_msg.subflag = 0;      //  TODO: Was 1
    send_msg.subaddr = 0;      //  TODO: Was 0x04
    send_msg.sublen  = 0;      //  TODO: Was 2
    send_msg.buf     = send_buf;
    send_msg.len     = data_len;
    send_msg.direct  = I2C_M_WRITE;
    send_msg.block   = I2C_M_BLOCK;
    send_msg.idex    = 0;      //  Index of next byte to be written from buf
    recv_msg.i2cx    = 0;      //  I2C Port

    //  Prepare to write 4 bytes of data to I2C device
    //  TODO: Is this needed?
    //  do_write_data(&send_msg);

    //  Start the I2C transfer and enable I2C interrupts
    gpstmsg = &send_msg;
    i2c_transfer_start(&send_msg);

    //  do_write_data will be called to send data in the Interrupt Handler (test_i2c_transferbytes)
}
#endif  //  NOTUSED

#ifdef NOTUSED
static void test_i2c_stop_write(char *buf, int len, int argc, char **argv)
{
    //  Stop the I2C transfer on I2C Port 0
    I2C_Disable(0);
}
#endif  //  NOTUSED

static void test_i2c_start_read(char *buf, int len, int argc, char **argv)
{
    //  Start reading data from I2C device
    //  Expect result 0x60 for BME280, 0x58 for BMP280
    int data_len = 1;  //  Bytes to be read
    memset(recv_buf, 0, sizeof(recv_buf));

    //  Init the I2C message
    //  recv_msg.addr = 0x76;  //  BME280 I2C Primary Address
    recv_msg.addr = 0x77;      //  BME280 I2C Secondary Address
    recv_msg.subflag = 1;      //  Enable Register Address
    recv_msg.subaddr = 0xd0;   //  Register Address (BME280 Chip ID)
    recv_msg.sublen  = 1;      //  Length of Register Address (bytes)
    recv_msg.buf     = recv_buf;
    recv_msg.len     = data_len;
    recv_msg.direct  = I2C_M_READ;
    recv_msg.block   = I2C_M_BLOCK;
    recv_msg.idex    = 0;      //  Index of next byte to be read into buf
    recv_msg.i2cx    = 0;      //  I2C Port

    //  Start the I2C transfer and enable I2C interrupts
    gpstmsg = &recv_msg;
    i2c_transfer_start(&recv_msg);

    //  do_read_data will be called to receive data in the I2C Interrupt Handler (test_i2c_transferbytes)
}

static void test_i2c_stop_read(char *buf, int len, int argc, char **argv)
{
    //  Stop the I2C transfer on I2C Port 0
    I2C_Disable(0);

    //  Dump the data received
    for (int i = 0; i < recv_msg.len; i++) {
        printf("%02x\n", recv_buf[i]);
    }
}

// STATIC_CLI_CMD_ATTRIBUTE makes this(these) command(s) static
const static struct cli_command cmds_user[] STATIC_CLI_CMD_ATTRIBUTE = {
    {"test_i2c", "test i2c", test_i2c_api},
    {"i2c_status", "I2C status", test_i2c_status},
    {"i2c_init", "Init I2C", test_i2c_init},
    {"i2c_clear_status", "Clear I2C Port status", test_i2c_clear_status},
#ifdef NOTUSED
    {"i2c_start_write", "Start writing I2C data", test_i2c_start_write},
    {"i2c_stop_write", "Stop writing I2C data", test_i2c_stop_write},
#endif  //  NOTUSED
    {"i2c_start_read", "Start reading I2C data", test_i2c_start_read},
    {"i2c_stop_read", "Stop reading I2C data", test_i2c_stop_read},
};                                                                                   

int i2c_cli_init(void)
{
    // static command(s) do NOT need to call aos_cli_register_command(s) to register.
    // However, calling aos_cli_register_command(s) here is OK but is of no effect as cmds_user are included in cmds list.
    // XXX NOTE: Calling this *empty* function is necessary to make cmds_user in this file to be kept in the final link.
    //return aos_cli_register_commands(cmds_user, sizeof(cmds_user)/sizeof(cmds_user[0]));          
    return 0;
}

//  TODO: For Linux only
//  int _stat(const char *file, void *pstat) { return 0; }

#ifdef NOTUSED

BME280 has I2C Device ID 0x77. We want to read 
Register 0xd0 (Chip ID).

BME280 was tested with this Bus Pirate I2C command...
    [0xee 0xd0] [0xef r]

Which means...
    <Start> 0xee 0xd0 <Stop>
    <Start> 0xef <Read> <Stop>

In which...
    0xee = (0x77 * 2) + 0, for writing
    0xef = (0x77 * 2) + 1, for reading
    0xd0 = Register to be read (Chip ID)

We need to reproduce on BL602 the two 
<Start> ... <Stop> transactions, 
plus sending 3 bytes, and 
receiving 1 byte.

The byte received should be 0x60.

#endif  //  NOTUSED
