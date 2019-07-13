/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-24     whj4674672   first version
 */

#include "ad7606.h"

//#ifdef PKG_USING_AD7606

#define DRV_DEBUG
#define LOG_TAG              "ad7606"
#include <drv_log.h>

static rt_err_t ad7606_gpio_init(ad7606_device_t dev)
{
    RT_ASSERT(dev);
    
    /* set CONV RST RANGE mode to output */
    rt_pin_mode(dev->config.conv,   PIN_MODE_OUTPUT);
    rt_pin_mode(dev->config.rst,    PIN_MODE_OUTPUT);
    
#ifdef AD7606_RANGE_SET
    rt_pin_mode(dev->config.range,  PIN_MODE_OUTPUT);
#endif
    
#ifdef AD7606_RESOLUUTION_SET
    /*set resolution mode to output*/
    rt_pin_mode(dev->config.os0,    PIN_MODE_OUTPUT);
    rt_pin_mode(dev->config.os1,    PIN_MODE_OUTPUT);
    rt_pin_mode(dev->config.os2,    PIN_MODE_OUTPUT);
#endif

    /* set BUSY mode to input */
    rt_pin_mode(dev->config.busy,   PIN_MODE_INPUT);
    
#ifdef AD7606_RANGE_SET
    /*set range*/
    rt_pin_write(dev->config.rst , AD7606_RANGE_PIN);
#endif

#ifdef AD7606_RESOLUUTION_SET
    /*set sampling resolution*/
    if(AD7606_RESOLUUTION == 2)
    {
      rt_pin_write(dev->config.os0 , PIN_HIGH);
      rt_pin_write(dev->config.os1 , PIN_LOW);
      rt_pin_write(dev->config.os2 , PIN_LOW);
    }
    else if(AD7606_RESOLUUTION == 4)
    {
      rt_pin_write(dev->config.os0 , PIN_LOW);
      rt_pin_write(dev->config.os1 , PIN_HIGH);
      rt_pin_write(dev->config.os2 , PIN_LOW);
    }
    else if(AD7606_RESOLUUTION == 8)
    {
      rt_pin_write(dev->config.os0 , PIN_HIGH);
      rt_pin_write(dev->config.os1 , PIN_HIGH);
      rt_pin_write(dev->config.os2 , PIN_LOW);
    }
    else if(AD7606_RESOLUUTION == 16)
    {
      rt_pin_write(dev->config.os0 , PIN_LOW);
      rt_pin_write(dev->config.os1 , PIN_LOW);
      rt_pin_write(dev->config.os2 , PIN_HIGH);
    }
    else if(AD7606_RESOLUUTION == 32)
    {
      rt_pin_write(dev->config.os0 , PIN_HIGH);
      rt_pin_write(dev->config.os1 , PIN_LOW);
      rt_pin_write(dev->config.os2 , PIN_HIGH);
    }
    else if(AD7606_RESOLUUTION == 64)
    {
      rt_pin_write(dev->config.os0 , PIN_LOW);
      rt_pin_write(dev->config.os1 , PIN_HIGH);
      rt_pin_write(dev->config.os2 , PIN_HIGH);
    }
    else
    {
      rt_pin_write(dev->config.os0 , PIN_LOW);
      rt_pin_write(dev->config.os1 , PIN_LOW);
      rt_pin_write(dev->config.os2 , PIN_LOW);
    }
#endif

    /*reset ad7606*/
    rt_pin_write(dev->config.rst , PIN_LOW);
    
    rt_pin_write(dev->config.rst , PIN_HIGH);
    rt_pin_write(dev->config.rst , PIN_HIGH);
    rt_pin_write(dev->config.rst , PIN_HIGH);
    
    rt_pin_write(dev->config.rst , PIN_LOW);
    
    /* convst */
    rt_pin_write(dev->config.conv , PIN_HIGH);
    
    return RT_EOK;
}

rt_size_t ad7606_read(ad7606_device_t dev,rt_int8_t* buf)
{
    RT_ASSERT(dev);

    /*convst*/
    rt_pin_write(dev->config.conv , PIN_LOW);
    rt_pin_write(dev->config.conv , PIN_LOW);
    rt_pin_write(dev->config.conv , PIN_LOW);
    
    rt_pin_write(dev->config.conv , PIN_HIGH);
    
    while(rt_pin_read(dev->config.busy))
    {
      rt_thread_delay(1);
    }

    /* receive data */
    return rt_spi_transfer(dev->device,RT_NULL,buf,16);

}

ad7606_device_t rt_hw_ad7606_init(const char *device_name, struct ad7606_config *config)
{
    RT_ASSERT(device_name);
    struct ad7606_device *dev;
    struct rt_spi_configuration cfg;
    
    dev = RT_NULL;
    /* create ad7606 device */
    dev = (struct ad7606_device *)rt_calloc(1, sizeof(struct ad7606_device));
    if (dev == RT_NULL)
    {
        LOG_E("can't allocate memory for ad7606_device device on '%s' ", device_name);
        goto __exit;
    }
    
    dev->device = (struct rt_spi_device *)rt_device_find(device_name);
    if(dev->device == RT_NULL)
    {
        LOG_E("can't find device:'%s'", device_name);
        goto __exit;
    }
    
    /* config spi */
    {
        cfg.data_width = 8;
        cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_3 | RT_SPI_MSB;
        cfg.max_hz = 12 * 1000 *1000; /* Set spi max speed */
    
        rt_spi_configure(dev->device, &cfg);
    }
    
    /* config ad7606 gpio*/
    dev->config.busy  = config->busy;
    dev->config.rst   = config->rst;
    dev->config.conv  = config->conv;
    
#ifdef AD7606_RANGE_SET
    dev->config.range = config->range;
#endif

#ifdef AD7606_RESOLUUTION_SET
    dev->config.os0   = config->os0;
    dev->config.os1   = config->os1;
    dev->config.os2   = config->os2;
#endif
    /* init gpio */
    ad7606_gpio_init(dev);
    
__exit:
    if (dev == RT_NULL)
    {
        rt_free(dev);
        LOG_E("init ad7606 failed\n");
    }
    
    return dev;
}

//#endif /* PKG_USING_AD7606 */

/**************** end of file ******************/
