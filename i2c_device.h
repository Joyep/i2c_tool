#ifndef __I2C_DEVICE_H__
#define __I2C_DEVICE_H__

#include <linux/types.h>

typedef struct i2c_device i2c_t;

struct i2c_device {
	int fd;
	int addr;
	int reg_size;
	int data_size;
	int (*write)(i2c_t *i2c, __u8 * buf, int len);
	int (*read)(i2c_t *i2c, __u8 * buf, int len);
	int (*write_reg)(i2c_t *i2c, __u32 reg, __u32 data);
	int (*read_reg)(i2c_t *i2c, __u32 reg, __u8* data);
	int (*close)(i2c_t *i2c);
};

extern i2c_t* i2c_open(const char *dev, __u16 addr, __u8 reg_size, __u8 data_size);

#endif

