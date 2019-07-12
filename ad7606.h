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

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"

#define AD7606_RANGE_SET
#define AD7606_RANGE_PIN    PIN_LOW

#define AD7606_RESOLUUTION_SET
#define AD7606_RESOLUUTION  0

struct ad7606_gpio
{
    rt_uint32_t busy; /* busy   */
    rt_uint32_t rst;  /* reset  */
    rt_uint32_t conv; /* convst */

#ifdef AD7606_RANGE_SET
    rt_uint32_t range;/* range  */
#endif

#ifdef AD7606_RESOLUUTION_SET
    rt_uint32_t os0;  /* os0  */
    rt_uint32_t os1;  /* os1  */
    rt_uint32_t os2;  /* os2  */
#endif
};

struct ad7606_device
{
    struct rt_spi_device *device;
    struct ad7606_gpio gpio;
    const char *device_name;
};
typedef struct ad7606_device *ad7606_device_t;

/**
 * This function initialize the ad7606 device.
 */
ad7606_device_t ad7606_init(ad7606_device_t device);

/**
 * This function releases memory
 */
void ad7606_deinit(ad7606_device_t device);

/**
 * This function read the data of ad7606.
 */
void ad7606_read(ad7606_device_t dev,rt_int8_t* buf);

#endif
