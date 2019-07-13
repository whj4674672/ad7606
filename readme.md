# ad7606软件驱动包

## 简介
本软件包是为Analog Devices 公司的8路AD采样芯片 ad7606 提供的软件驱动包。

## 支持情况

| **包含设备** | **AD采样** |
| ------------ | ---------- |
|   通讯接口   |            |
|      SPI     |      √     |
|   工作模式   |            |
|    轮询      |      √     |
|   测量范围   |            |
|     ±5V      |      √     |
|     ±10V     |      √     |


## 使用说明

### 依赖


### 获取软件包


### 使用软件包

初始化示例

```bash
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
```
## 注意事项
1. 量程需要用户自行通过控制RANGE电平来选择±5V（低电平）还是±10V（高电平）
2. 采样分辨率需要用户自行通过OS0 OS1 OS2进行设置
|   OS2 OS1 OS0   |   过采样模式   |
|       000       |    无过采样    |
|       001       |    2倍过采样   |
|       010       |    4倍过采样   |
|       011       |    8倍过采样   |
|       100       |   16倍过采样   |
|       101       |   32倍过采样   |
|       110       |   64倍过采样   |

## 联系人信息

维护人:

- [whj4674672](https://github.com/whj4674672) 