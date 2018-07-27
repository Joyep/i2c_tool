/* i2c_tool.c
 * Author: Daniel Peng
 * mail: yudingp@163.com
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "i2c_api.h"

#include "log.h"
#define LOG_TAG "i2c_tool"
static int log_level = LOG_LEVEL_DEBUG;


#define I2C_BUS "/dev/i2c-"

void print_help()
{
	log_i("******\n");
	log_i("i2ctool version v0.1\n");
	log_i("******\n");
	log_i("Usage:\n");
	log_i("i2ctool -i 0 -a 0x0f -r 1 -d 1  0x1122 0x11223344\n");
	log_i("-i: i2c adapter bus index\n");
	log_i("-a: i2c device address\n");
	log_i("-r: register byte width\n");
	log_i("-d: data byte width\n");
	log_i("-n: read number, default 1\n");
}

int main(int argc,char *argv[])
{
    int opt=0;
	char i2c_device_name[20];

	/*
	int i = 0;
	int a = 0;
	int r = 0;
	int d = 0;

	char iv[OPT_LENGTH];
	char av[OPT_LENGTH];
	char rv[OPT_LENGTH];
	char dv[OPT_LENGTH];
	*/

	unsigned int i2c_index = 0;
	unsigned int i2c_addr = 0x0f;//0x0f
	unsigned int reg_size = 1;
	unsigned int data_size = 1;

	unsigned int read_number = 1;


	unsigned int reg = 0;
	unsigned int data = 0;
	int to_write = 0;

	char buf[8];
	int ret;
	int fd;


	//1, parse params
    while((opt=getopt(argc,argv,"i:a:r:d:n:"))!=-1) {
        switch(opt) {
            case 'a':
				i2c_addr = strtoul(optarg, 0, 0);
				break;
			case 'i':
				i2c_index = strtoul(optarg, 0, 0);
				break;
			case 'r':
				reg_size = strtoul(optarg, 0, 0);
				break;
			case 'd':
				data_size = strtoul(optarg, 0, 0);
				break;
			case 'n':
				read_number = strtoul(optarg, 0, 0);
				break;
        }
    }

	if(argc-optind == 2) {
		//write
		reg = strtoul(argv[optind], 0, 0);
		data = strtoul(argv[optind+1], 0, 0);
		to_write = 1;

	} else if (argc-optind == 1) {
		//read
		reg = strtoul(argv[optind], 0, 0);
		to_write = 0;
	} else {
		//error
		LOG_E("params error");
		print_help();
		return -1;
	}

	log_i("i2c_index:\t%d\n", i2c_index);
	log_i("i2c_addr:\t0x%02x\b", i2c_addr);
	log_i("reg_size:\t%d\n", reg_size);
	log_i("data_size:\t%d\n", data_size);



	//open
	snprintf(i2c_device_name, 20, "%s%d", I2C_BUS, i2c_index);
	fd = i2c_open(i2c_device_name);
	if(fd < 0) {
		return fd;
	}

	//read or write
	if(to_write) {
		//log_i("Write: (0x%02x) = 0x%02x ...\n", reg, data);
		ret = i2c_write_reg(fd, i2c_addr, reg, reg_size, data, data_size);
	} else {
		//log_i("Read: (0x%02x) for %d number\n", reg, read_number);
		ret = i2c_read_reg(fd, i2c_addr, reg, reg_size, buf, data_size);
	}

	if(ret < 0) {
		LOG_E("error!");
	}

	i2c_close(fd);

    return ret;
}

