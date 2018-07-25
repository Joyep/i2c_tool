/* i2c_tool.c
 * Author: Daniel Peng
 * mail: yudingp@163.com
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "i2c_api.h"

#define I2C_BUS "/dev/i2c-"

#define OPT_LENGTH 10


void print_help()
{
	printf("******\n");
	printf("i2ctool version v0.1\n");
	printf("******\n");
	printf("Usage:\n");
	printf("i2ctool -i 2 -a 0x0f -r 2 -d 4  0x1122 0x11223344\n");
	printf("-i: i2c adapter bus index\n");
	printf("-a: i2c device address\n");
	printf("-r: register byte width\n");
	printf("-d: data byte width\n");
	printf("-n: read number, default 1\n");
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
		printf("params error\n");
		print_help();
		return -1;
	}

	printf("i2c_index:\t%d\ni2c_addr:\t0x%02x\nreg_size:\t%d\ndata_size:\t%d\n", i2c_index, i2c_addr, reg_size, data_size);



	//open
	snprintf(i2c_device_name, 20, "%s%d", I2C_BUS, i2c_index);
	ret = i2c_open(i2c_device_name);
	if(ret) {
		return ret;
	}


	//read or write
	if(to_write) {
		//printf("Write: (0x%02x) = 0x%02x ...\n", reg, data);
		ret = i2c_write_reg(i2c_addr, reg, reg_size, data, data_size);
	} else {
		//printf("Read: (0x%02x) for %d number\n", reg, read_number);
		ret = i2c_read_reg(i2c_addr, reg, reg_size, buf, data_size);
	}

	if(ret) {
		printf("error!\n");
	}

	i2c_close();

    return ret;
}

