/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-31     whj4674672   first vison
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_spi.h"
#include "ad7606.h"

//#define DRV_DEBUG
#define LOG_TAG              "ad7606"
#include <drv_log.h>

#define SPI_BUS    "spi2"
#define AD7606_SPI_DEVICE_NAME  "spi20"

#define AD7606_BUSY             GET_PIN(B, 9)
#define AD7606_CONV             GET_PIN(B, 8)
#define AD7606_RST              GET_PIN(D, 2)

#ifdef AD7606_RANGE_SET
#define AD7606_RANGE            GET_PIN(A, 1) //低电平的量程是±5V 高电平是±10V
#else
#define AD7606_RANGE            PIN_LOW
#endif

#define AD7606_CS_GPIO          GPIOC
#define AD7606_CS_PIN           GPIO_PIN_8

#ifdef  AD7606_RESOLUUTION_SET
#define AD7606_OS0              GET_PIN(A, 2)
#define AD7606_OS1              GET_PIN(A, 3)
#define AD7606_OS2              GET_PIN(A, 4)
#endif

ad7606_device_t dev;

int sample_ad7606(void)
{
    rt_uint8_t i;
    rt_int16_t adcbuff_now[8] = {0};
    rt_int8_t adc_buff[16] = {0};

    ad7606_read(dev,adc_buff);

    for(i=0;i<8;i++)
    {
     adcbuff_now[i] = (rt_int16_t)adc_buff[i*2] * 256 + (rt_int16_t)adc_buff[i*2+1] ;

     if(AD7606_RANGE)
     {
       /*range is 10V*/
       adcbuff_now[i] = adcbuff_now[i] * 10000 / 32767;
     }
     else
     {
       /*range is 5V*/
       adcbuff_now[i] = adcbuff_now[i] * 5000 / 32767;
     }
     
     rt_thread_delay(100);
     rt_kprintf("ad7606 ch%d is value %d mV!\r\n",i+1,adcbuff_now[i]);
    }
    
    return RT_EOK;
}
MSH_CMD_EXPORT(sample_ad7606, sample ad7606);

int rt_hw_ad7606_port(void)
{
    struct ad7606_config config;
    
    /*attach device*/
    rt_hw_spi_device_attach(SPI_BUS, AD7606_SPI_DEVICE_NAME, AD7606_CS_GPIO, AD7606_CS_PIN);
    
    {
      config.busy = AD7606_BUSY;
      config.rst  = AD7606_RST;
      config.conv = AD7606_CONV;

#ifdef AD7606_RANGE_SET
      config.range= AD7606_RANGE;
#endif

#ifdef AD7606_RESOLUUTION_SET
      config.os0  = AD7606_OS0;
      config.os1  = AD7606_OS1;
      config.os2  = AD7606_OS2;
#endif
    }
    
    /*init ad7606*/
    dev = rt_hw_ad7606_init(AD7606_SPI_DEVICE_NAME,&config);
    if(dev == RT_NULL)
    {
     LOG_E("ad7606 initialiation faild!");
    
     return RT_ERROR;
    }
    
    return RT_EOK;
}
INIT_APP_EXPORT(rt_hw_ad7606_port);
