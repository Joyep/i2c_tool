#ifndef __LOG_H__
#define __LOG_H__

#define LOG_LEVEL_SILENCE 				(0)
#define LOG_LEVEL_ERROR 				(1)
#define LOG_LEVEL_INFO 					(2)
#define LOG_LEVEL_DEBUG 				(3)

#define debug_level(level, fmt, arg...) do {\
	if (level <= log_level) {\
		if (level == LOG_LEVEL_ERROR) {\
			printf("[E] %s: %s() +%d: "fmt"\n", LOG_TAG, __func__, __LINE__, ##arg);\
		}\
		else if (level == LOG_LEVEL_INFO) {\
			printf("[I] %s: "fmt"\n", LOG_TAG, __func__, __LINE__, ##arg);\
		}\
		else if (level == LOG_LEVEL_DEBUG) {\
			printf("[D] %s: "fmt"\n", LOG_TAG, __func__, __LINE__, ##arg);\
		}\
		else {\
			printf("[?] %s: %s() +%d: "fmt"\n", LOG_TAG, __func__, __LINE__, ##arg);\
		}\
	}\
} while (0)

#define LOG_E(fmt, arg...) debug_level(LOG_LEVEL_ERROR, fmt, ##arg)
#define LOG_I(fmt, arg...) debug_level(LOG_LEVEL_INFO, fmt, ##arg)
#define LOG_D(fmt, arg...) debug_level(LOG_LEVEL_DEBUG, fmt, ##arg)


#define could_error() (LOG_LEVEL_ERROR <= log_level)
#define could_info() (LOG_LEVEL_INFO <= log_level)
#define could_debug() (LOG_LEVEL_DEBUG <= log_level)

#define log(fmt, arg...)  printf(fmt, ##arg)
#define log_e(fmt, arg...) if(could_error()) log(fmt, ##arg)
#define log_i(fmt, arg...) if(could_info()) log(fmt, ##arg)
#define log_d(fmt, arg...) if(could_debug()) log(fmt, ##arg)



#endif

