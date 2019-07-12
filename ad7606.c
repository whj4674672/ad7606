/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-24     whj4674672   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_spi.h"
#include "drv_gpio.h"
#include "ad7606.h"

//#define DRV_DEBUG
#define LOG_TAG              "ad7606"
#include <drv_log.h>


void ad7606_read(ad7606_device_t dev,rt_int8_t* buf)
{
    while(rt_pin_read(dev->gpio.busy))
    {
      rt_thread_delay(1);
    }
    
    rt_spi_transfer(dev->device,RT_NULL,buf,16);
    
    /*convst*/
    rt_pin_write(dev->gpio.conv , PIN_LOW);
    rt_pin_write(dev->gpio.conv , PIN_LOW);
    rt_pin_write(dev->gpio.conv , PIN_LOW);
    
    rt_pin_write(dev->gpio.conv , PIN_HIGH);
}

ad7606_device_t ad7606_init(ad7606_device_t dev)
{
    /* set CONV RST RANGE mode to output */
    rt_pin_mode(dev->gpio.conv,   PIN_MODE_OUTPUT);
    rt_pin_mode(dev->gpio.rst,    PIN_MODE_OUTPUT);
    
#ifdef AD7606_RANGE_SET
    rt_pin_mode(dev->gpio.range,  PIN_MODE_OUTPUT);
#endif
    
#ifdef AD7606_RESOLUUTION_SET
    /*set resolution mode to output*/
    rt_pin_mode(dev->gpio.os0,    PIN_MODE_OUTPUT);
    rt_pin_mode(dev->gpio.os1,    PIN_MODE_OUTPUT);
    rt_pin_mode(dev->gpio.os2,    PIN_MODE_OUTPUT);
#endif
    
    /* set BUSY mode to input */
    rt_pin_mode(dev->gpio.busy,       PIN_MODE_INPUT);
    
    dev->device = (struct rt_spi_device *)rt_device_find(dev->device_name);
    if(dev->device == RT_NULL)
    {
        LOG_E("please attach device");
    
        return RT_NULL;
    }
    
    /* config spi */
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_3 | RT_SPI_MSB;
        cfg.max_hz = 12 * 1000 *1000;
    
        rt_spi_configure(dev->device, &cfg);
    }
    
#ifdef AD7606_RANGE_SET
    /*set range*/
    rt_pin_write(dev->gpio.rst , AD7606_RANGE_PIN);
#endif

#ifdef AD7606_RESOLUUTION_SET
    /*set sampling resolution*/
    if(AD7606_RESOLUUTION == 2)
    {
      rt_pin_write(dev->gpio.os0 , PIN_HIGH);
      rt_pin_write(dev->gpio.os1 , PIN_LOW);
      rt_pin_write(dev->gpio.os2 , PIN_LOW);
    }
    else if(AD7606_RESOLUUTION == 4)
    {
      rt_pin_write(dev->gpio.os0 , PIN_LOW);
      rt_pin_write(dev->gpio.os1 , PIN_HIGH);
      rt_pin_write(dev->gpio.os2 , PIN_LOW);
    }
    else if(AD7606_RESOLUUTION == 8)
    {
      rt_pin_write(dev->gpio.os0 , PIN_HIGH);
      rt_pin_write(dev->gpio.os1 , PIN_HIGH);
      rt_pin_write(dev->gpio.os2 , PIN_LOW);
    }
    else if(AD7606_RESOLUUTION == 16)
    {
      rt_pin_write(dev->gpio.os0 , PIN_LOW);
      rt_pin_write(dev->gpio.os1 , PIN_LOW);
      rt_pin_write(dev->gpio.os2 , PIN_HIGH);
    }
    else if(AD7606_RESOLUUTION == 32)
    {
      rt_pin_write(dev->gpio.os0 , PIN_HIGH);
      rt_pin_write(dev->gpio.os1 , PIN_LOW);
      rt_pin_write(dev->gpio.os2 , PIN_HIGH);
    }
    else if(AD7606_RESOLUUTION == 64)
    {
      rt_pin_write(dev->gpio.os0 , PIN_LOW);
      rt_pin_write(dev->gpio.os1 , PIN_HIGH);
      rt_pin_write(dev->gpio.os2 , PIN_HIGH);
    }
    else
    {
      rt_pin_write(dev->gpio.os0 , PIN_LOW);
      rt_pin_write(dev->gpio.os1 , PIN_LOW);
      rt_pin_write(dev->gpio.os2 , PIN_LOW);
    }
#endif
    
    /*reset ad7606*/
    rt_pin_write(dev->gpio.rst , PIN_LOW);
    
    rt_pin_write(dev->gpio.rst , PIN_HIGH);
    rt_pin_write(dev->gpio.rst , PIN_HIGH);
    rt_pin_write(dev->gpio.rst , PIN_HIGH);
    
    rt_pin_write(dev->gpio.rst , PIN_LOW);
    
    /* convst */
    rt_pin_write(dev->gpio.conv , PIN_HIGH);
    
    return dev;
}

void ad7606_deinit(ad7606_device_t dev)
{
    RT_ASSERT(dev);
    
    rt_free(dev);
}
