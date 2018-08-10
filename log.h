#ifndef __LOG_H__
#define __LOG_H__

#ifdef LOG_TO_LOGCAT
#include <log/log.h>
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_ERROR
#endif

#define LOG_LEVEL_SILENCE 				(0)
#define LOG_LEVEL_ERROR 				(1)
#define LOG_LEVEL_INFO 					(2)
#define LOG_LEVEL_DEBUG 				(3)

/*
#define LOG_TO_TERM (0)
#define LOG_TO_LOGCAT (1)

#ifdef LOG_TO_TERM
#define PRINTE(fmt, arg...) do {\
	printf("[E] %s: +%d %s(): "fmt, LOG_TAG, __LINE__, __func__, ##arg);\
} while(0)
#define PRINTI(fmt, arg...) do {\
	printf("[I] %s: +%d %s(): "fmt, LOG_TAG, __LINE__, __func__, ##arg);\
} while(0)
#define PRINTD(fmt, arg...) do {\
	printf("[D] %s: +%d %s(): "fmt, LOG_TAG, __LINE__, __func__, ##arg);\
} while(0)
#define log printf
#endif
*/

#ifdef LOG_TO_LOGCAT

#define PRINTE(fmt, arg...) do {\
	ALOGE(" +%d %s(): "fmt, __LINE__, __func__, ##arg);\
} while(0)
#define PRINTI(fmt, arg...) do {\
	ALOGI(" +%d %s(): "fmt, __LINE__, __func__, ##arg);\
} while(0)
#define PRINTD(fmt, arg...) do {\
	ALOGD(" +%d %s(): "fmt, __LINE__, __func__, ##arg);\
} while(0)
#define log ALOGI

#else

#define PRINTE(fmt, arg...) do {\
	printf("[E] %s: +%d %s(): "fmt, LOG_TAG, __LINE__, __func__, ##arg);\
} while(0)
#define PRINTI(fmt, arg...) do {\
	printf("[I] %s: "fmt, LOG_TAG, ##arg);\
} while(0)
#define PRINTD(fmt, arg...) do {\
	printf("[D] %s: "fmt, LOG_TAG, ##arg);\
} while(0)
#define log printf


#endif


#define debug_level(level, fmt, arg...) do {\
	if (level <= LOG_LEVEL) {\
		if (level == LOG_LEVEL_ERROR) {\
			PRINTE(fmt, ##arg); \
		} else if (level == LOG_LEVEL_INFO) {\
			PRINTI(fmt, ##arg); \
		} else if (level == LOG_LEVEL_DEBUG) {\
			PRINTD(fmt, ##arg); \
		}\
	}\
} while (0)


#define LOG_E(fmt, arg...) debug_level(LOG_LEVEL_ERROR, fmt, ##arg)
#define LOG_I(fmt, arg...) debug_level(LOG_LEVEL_INFO, fmt, ##arg)
#define LOG_D(fmt, arg...) debug_level(LOG_LEVEL_DEBUG, fmt, ##arg)


#define could_error() (LOG_LEVEL_ERROR <= LOG_LEVEL)
#define could_info() (LOG_LEVEL_INFO <= LOG_LEVEL)
#define could_debug() (LOG_LEVEL_DEBUG <= LOG_LEVEL)

#define log_e(fmt, arg...) if(could_error()) log(fmt, ##arg)
#define log_i(fmt, arg...) if(could_info()) log(fmt, ##arg)
#define log_d(fmt, arg...) if(could_debug()) log(fmt, ##arg)



#endif

