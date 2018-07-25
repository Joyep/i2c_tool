/* i2c_tool.c
 * Author: fsysky
 * mail: fsysky@163.com
 */
#include "i2c_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "d2dp_cmds.h"

#define IIC_BUS "/dev/i2c-2"
#define IIC_ADDR 0x0f


int main(int argc, char *argv[])
{
        int ret;
		unsigned int i;
        int reg, value, addr;
		__u8 buf[8];
		int cmds_index = 0;
		struct cmd_list* list;
		struct cmd_item* cmds;

		if(argc == 2) {
           	cmds_index = strtoul(argv[1], 0, 0);
		}
		list = &cmds_group[cmds_index];
		cmds = list->cmds;

		printf("user cmds name = %s\n", list->name);

		printf("iic bus = %s, addr=0x%x\n", IIC_BUS, IIC_ADDR);
        IIC_Open(IIC_BUS);

		for(i = 0; i < list->len; i++) {
			if(CMD_WRITE == cmds[i].type) {
				buf[0] = cmds[i].reg >> 8 & 0xff;
				buf[1] = cmds[i].reg & 0xff;

				buf[5] = cmds[i].value >> 24 & 0xff;
				buf[4] = cmds[i].value >> 16 & 0xff;
				buf[3] = cmds[i].value >> 8 & 0xff;
				buf[2] = cmds[i].value & 0xff;

				printf(">>> 0x%02x%02x = 0x%02x%02x%02x%02x\n", buf[0], buf[1], buf[5], buf[4], buf[3], buf[2]);
				iic_write(IIC_ADDR, buf, 6);
				/*
				iic_write(IIC_ADDR, buf, 2);
				iic_write(IIC_ADDR, buf+2, 4);
				*/
				usleep(20 * 1000);

			} else if(CMD_READ == cmds[i].type){
				iic_read_reg(IIC_ADDR, cmds[i].reg, 2, buf, 4);
				printf("*** 0x%04x = 0x%02x%02x%02x%02x\n", cmds[i].reg, buf[3], buf[2], buf[1], buf[0]);
				usleep(20 * 1000);
			} else if(CMD_DELAY == cmds[i].type) {
				//usleep(cmds[i].value * 1000);
				//printf("=== delay %dms\n", cmds[i].value);
			}


		}
		printf("user cmds name = %s\n", list->name);

        IIC_Close();
        return 0;
}
