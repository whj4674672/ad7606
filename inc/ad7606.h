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
 
#ifndef __AD7606_H
#define __AD7606_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_spi.h"
#include "drv_gpio.h"

//#define AD7606_RANGE_SET
//#define AD7606_RANGE_PIN    PIN_LOW

//#define AD7606_RESOLUUTION_SET
//#define AD7606_RESOLUUTION  0

struct ad7606_config
{
    rt_base_t busy; /* busy   */
    rt_base_t rst;  /* reset  */
    rt_base_t conv; /* convst */

#ifdef AD7606_RANGE_SET
    rt_base_t range;/* range  */
#endif

#ifdef AD7606_RESOLUUTION_SET
    rt_base_t os0;  /* os0  */
    rt_base_t os1;  /* os1  */
    rt_base_t os2;  /* os2  */
#endif
};

struct ad7606_device
{
    struct rt_spi_device *device;
    struct ad7606_config config;
};
typedef struct ad7606_device *ad7606_device_t;

/**
 * This function initialize the ad7606.
 */
ad7606_device_t rt_hw_ad7606_init(const char *device_name, struct ad7606_config *config);

/**
 * This function will read the data form ad7606.
 */
void ad7606_read(ad7606_device_t dev,rt_int8_t* buf);

#ifdef __cplusplus
}
#endif

#endif

/**************** end of file ******************/
