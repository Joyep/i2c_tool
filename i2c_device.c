/* i2c_api.c
 * Auther: fsysky
 * E-mail: fsysky@163.com
 * Description:
 * I2C user space control
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "i2c_device.h"

#include "log.h"
#define LOG_TAG "i2c_api"
static int log_level = LOG_LEVEL_ERROR;


static void print_bytes(__u8* buf, int size)
{
	int i;
	log("0x");
	for(i=0; i<size; i++){
		log("%02x", buf[i]);
	}
}

static void print_result(char* name, __u8* reg, int reg_size, __u8* data, int data_size)
{

	if(!could_debug()) {
		return;
	}
	log("%s", name);
	print_bytes(reg, reg_size);
	log(" = ");
	if(data_size <= 0) {
		log("\n");
		return;
	}
	print_bytes(data, data_size);
	log("\n");
}



static int i2c_close(i2c_t *i2c)
{
	close(i2c->fd);
	free(i2c);
	return 0;
}

static int i2c_write(i2c_t* i2c, __u8 * buf, int len)
{
	struct i2c_msg msgs[1];
	struct i2c_rdwr_ioctl_data ioctl_data;
	int ret;

	msgs[0].addr = i2c->addr;
	msgs[0].flags = 0;
	msgs[0].buf = buf;
	msgs[0].len = len;
	//msgs[0].scl_rate=100 * 1000;

	ioctl_data.nmsgs = 1;
	ioctl_data.msgs = msgs;
	ret = ioctl(i2c->fd, I2C_RDWR, &ioctl_data);
	if (ret < 0) {
		LOG_E("ioctl failed (%d)", ret);
	}
	return ret;
}


static int i2c_read(i2c_t* i2c, __u8 * buf, int len)
{
	struct i2c_msg msgs[1];
	struct i2c_rdwr_ioctl_data ioctl_data;
	int ret;

	msgs[0].addr = i2c->addr;
	msgs[0].flags = I2C_M_RD;
	msgs[0].buf = buf;
	msgs[0].len = len;
	//msgs[0].scl_rate=100 * 1000;
	//
	ioctl_data.nmsgs = 1;
	ioctl_data.msgs = msgs;
	ret = ioctl(i2c->fd, I2C_RDWR, &ioctl_data);
	if (ret < 0) {
		LOG_E("ioctl failed (%d)", ret);
	}
	return ret;
}


static int i2c_write_reg(i2c_t *i2c, __u32 reg, __u32 data)
{
	__u8 buf[8];
	int i;
	int ret;
	int reg_size = i2c->reg_size;
	int data_size = i2c->data_size;

	if(reg_size > 4 || reg_size < 1) {
		LOG_E("reg_size error!");
		return -1;
	}

	if(data_size > 4 || data_size < 1) {
		LOG_E("data_size error!");
		return -1;
	}

	for(i = 0; i < reg_size; i++) {
		buf[i] = reg >> ((reg_size-i-1)*8) & 0xff;
	}
	for(i = 0; i < data_size; i++) {
		buf[reg_size+i] = data >> (i*8) & 0xff;
	}

	print_result(">>> ", buf, reg_size, buf+reg_size, data_size);

	ret = i2c_write(i2c, buf, reg_size+data_size);
	if(ret < 0) {
		LOG_E("write reg failed!(%d)", ret);
		return ret;
	}


}

static int i2c_read_reg(i2c_t* i2c, __u32 reg, __u8* buf)
{
	__u8 reg_buf[4];
	int i;
	int ret;
	int reg_size = i2c->reg_size;

	if(reg_size > 4 || reg_size < 1) {
		LOG_E("reg_size error!\n");
		return -1;
	}

	for(i = 0; i < reg_size; i++) {
		reg_buf[i] = reg >> ((reg_size-i-1)*8) & 0xff;
	}

	ret = i2c_write(i2c, reg_buf, reg_size);
	if(ret < 0){
		print_result("!!! ", reg_buf, reg_size, NULL, 0);
		LOG_E("write reg failed!(%d)", ret);
		return ret;
	}
	ret = i2c_read(i2c, buf, i2c->data_size);
	if(ret < 0) {
		print_result("!!! ", reg_buf, reg_size, NULL, 0);
		LOG_E("read reg failed!(%d)", ret);
		return ret;
	}

	print_result("*** ", reg_buf, reg_size, buf, i2c->data_size);

	return 0;
}


static int i2c_set_timeout(i2c_t* i2c, int timeout)
{
	int ret = ioctl(i2c->fd, I2C_TIMEOUT, timeout);
	if(ret < 0) {
		LOG_E("ioctl failed");
	}
	return ret;
}

static int i2c_set_retries(i2c_t* i2c, int retry)
{
	int ret = ioctl(i2c->fd, I2C_RETRIES, retry);
	if(ret < 0) {
		LOG_E("ioctl failed");
	}
	return ret;
}

static void i2c_config(i2c_t* i2c, __u8 reg_size, __u8 data_size)
{
	i2c->reg_size = reg_size > 4 ? 4 : reg_size;
	i2c->data_size = data_size > 4 ? 4 : data_size;
}

i2c_t* i2c_open(const char* dev, __u16 addr)
{
	int fd = open(dev, O_RDWR);
	i2c_t *i2c;

	if (fd < 0) {
		LOG_E("open %s failed (%d)", dev, fd);
		return NULL;
	}

	i2c = (i2c_t*)malloc(sizeof(i2c_t));
	if(!i2c) {
		LOG_E("malloc i2c device error!");
		close(fd);
		return NULL;
	}

	i2c->fd = fd;
	i2c->addr = addr;
	i2c->reg_size = 1;//reg_size > 4 ? 4 : reg_size;
	i2c->data_size = 1;//data_size > 4 ? 4 : data_size;

	i2c->config = i2c_config;
	i2c->close = i2c_close;

	i2c->write = i2c_write;
	i2c->read = i2c_read;
	i2c->write_reg = i2c_write_reg;
	i2c->read_reg = i2c_read_reg;

	return i2c;
}


