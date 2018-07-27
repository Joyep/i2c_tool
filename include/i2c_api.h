/* i2c_api.h
 *  * Author: fsysky
 *   * mail: fsysky@163.com
 *    */
#include <linux/types.h>

int i2c_open(char *dev);
int i2c_close(int fd);
int i2c_write(int fd, __u16 addr, __u8 * buf, int len);
int i2c_read(int fd, __u16 addr, __u8 * buf, int len);
int i2c_write_reg(int fd, __u16 addr, __u32 reg, int reg_size, __u32 data, int data_size);
int i2c_read_reg(int fd, __u16 addr, __u32 reg, int reg_size, __u8* buf, int len);
