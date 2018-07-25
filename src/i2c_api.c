/* i2c_api.c
 * Auther: fsysky
 * E-mail: fsysky@163.com
 * Description:
 * I2C user space control
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>


static int fd = -1;

int i2c_open(char *dev)
{
	fd = open(dev, O_RDWR);
	if (fd < 0) {
		printf("open %s failed\n", dev);
		return -1;
	}
	return 0;
}

int i2c_close(void)
{
	if(fd != -1) {
		fd = -1;
		return close(fd);
	}
	printf("i2c not open, skip!\n");
	return 0;
}

int i2c_write(__u16 addr, __u8 * buf, int len)
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
		printf("%s: ioctl return: %d\n", __func__, ret);
	}
	return ret;
}


int i2c_read(__u16 addr, __u8 * buf, int len)
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
		printf("%s: ioctl return: %d\n", __func__, ret);
	}
	return ret;
}


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
	print_bytes(data, data_size);
	printf("\n");
}


int i2c_write_reg(__u16 addr, __u32 reg, int reg_size, __u32 data, int data_size)
{
	__u8 buf[8];
	int i;

	if(reg_size > 4 || reg_size < 1) {
		printf("reg_size error!\n");
		return -1;
	}

	if(data_size > 4 || data_size < 1) {
		printf("data_size error!\n");
		return -1;
	}

	for(i = 0; i < reg_size; i++) {
		buf[i] = reg >> ((reg_size-i-1)*8) & 0xff;
	}
	for(i = 0; i < data_size; i++) {
		buf[reg_size+i] = data >> (i*8) & 0xff;
	}

	print_result(">>> ", buf, reg_size, buf+reg_size, data_size);

	return i2c_write(addr, buf, reg_size+data_size);

}

int i2c_read_reg(__u16 addr, __u32 reg, int reg_size, __u8* buf, int len)
{
	__u8 reg_buf[4];
	int i;
	int ret;

	if(reg_size > 4 || reg_size < 1) {
		printf("reg_size error!\n");
		return -1;
	}

	for(i = 0; i < reg_size; i++) {
		reg_buf[i] = reg >> ((reg_size-i-1)*8) & 0xff;
	}

	i2c_write(addr, reg_buf, reg_size);
	ret = i2c_read(addr, buf, len);
	if(ret) {
		print_result("*** ", reg_buf, reg_size, buf, len);
		printf("read failed!");
		return ret;
	}

	print_result("*** ", reg_buf, reg_size, buf, len);

	return 0;
}
