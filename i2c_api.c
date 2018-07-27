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


#include "log.h"
#define LOG_TAG "i2c_api"
static int log_level = LOG_LEVEL_DEBUG;


void print_bytes(__u8* buf, int size)
{
	int i;
	printf("0x");
	for(i=0; i<size; i++){
		printf("%02x", buf[i]);
	}
}

void print_result(char* sign, __u8* reg, int reg_size, __u8* data, int data_size)
{
	printf("%s", sign);
	print_bytes(reg, reg_size);
	printf(" = ");
	if(data_size <= 0) {
		printf("\n");
		return;
	}
	print_bytes(data, data_size);
	log("\n");
}


int i2c_open(char *dev)
{
	int fd = open(dev, O_RDWR);
	if (fd < 0) {
		LOG_E("open %s failed (%d)", dev, fd);
		return fd;
	}
	return fd;
}

int i2c_close(int fd)
{
	if(fd != -1) {
		fd = -1;
		return close(fd);
	}
	LOG_E("i2c not open, skip!");
	return 0;
}

int i2c_write(int fd, __u16 addr, __u8 * buf, int len)
{
	struct i2c_msg msgs[1];
	struct i2c_rdwr_ioctl_data ioctl_data;
	int ret;

	msgs[0].addr = addr;
	msgs[0].flags = 0;
	msgs[0].buf = buf;
	msgs[0].len = len;
	//msgs[0].scl_rate=100 * 1000;

	ioctl_data.nmsgs = 1;
	ioctl_data.msgs = msgs;
	ret = ioctl(fd, I2C_RDWR, &ioctl_data);
	if (ret < 0) {
		LOG_E("ioctl failed (%d)", ret);
	}
	return ret;
}


int i2c_read(int fd, __u16 addr, __u8 * buf, int len)
{
	struct i2c_msg msgs[1];
	struct i2c_rdwr_ioctl_data ioctl_data;
	int ret;

	msgs[0].addr = addr;
	msgs[0].flags = I2C_M_RD;
	msgs[0].buf = buf;
	msgs[0].len = len;
	//msgs[0].scl_rate=100 * 1000;
	//
	ioctl_data.nmsgs = 1;
	ioctl_data.msgs = msgs;
	ret = ioctl(fd, I2C_RDWR, &ioctl_data);
	if (ret < 0) {
		LOG_E("ioctl failed (%d)", ret);
	}
	return ret;
}



int i2c_write_reg(int fd, __u16 addr, __u32 reg, int reg_size, __u32 data, int data_size)
{
	__u8 buf[8];
	int i;
	int ret;

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

	if(could_debug()) {
		print_result(">>> ", buf, reg_size, buf+reg_size, data_size);
	}

	ret = i2c_write(fd, addr, buf, reg_size+data_size);
	if(ret < 0) {
		LOG_E("write reg failed!(%d)", ret);
		return ret;
	}


}

int i2c_read_reg(int fd, __u16 addr, __u32 reg, int reg_size, __u8* buf, int len)
{
	__u8 reg_buf[4];
	int i;
	int ret;

	if(reg_size > 4 || reg_size < 1) {
		LOG_E("reg_size error!\n");
		return -1;
	}

	for(i = 0; i < reg_size; i++) {
		reg_buf[i] = reg >> ((reg_size-i-1)*8) & 0xff;
	}

	ret = i2c_write(fd, addr, reg_buf, reg_size);
	if(ret < 0){
		print_result("*** ", reg_buf, reg_size, NULL, 0);
		LOG_E("write reg failed!(%d)", ret);
		return ret;
	}
	ret = i2c_read(fd, addr, buf, len);
	if(ret < 0) {
		print_result("*** ", reg_buf, reg_size, NULL, 0);
		LOG_E("read reg failed!(%d)", ret);
		return ret;
	}

	print_result("*** ", reg_buf, reg_size, buf, len);

	return 0;
}
