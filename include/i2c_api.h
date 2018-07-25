/* i2c_api.h
 *  * Author: fsysky
 *   * mail: fsysky@163.com
 *    */
#include <linux/types.h>


int i2c_open(char *dev);
int i2c_close(void);
int i2c_write(__u16 addr, __u8 * buf, int len);
int i2c_read(__u16 addr, __u8 * buf, int len);
int i2c_read_reg(__u16 addr, __u32 reg, int reg_size, __u8* buf, int len);
int i2c_write_reg(__u16 addr, __u32 reg, int reg_size, __u32 data, int data_size);
