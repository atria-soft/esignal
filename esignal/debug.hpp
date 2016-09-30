/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <elog/log.hpp>

namespace esignal {
	int32_t getLogId();
};
#define ESIGNAL_BASE(info,data) ELOG_BASE(esignal::getLogId(),info,data)

#define ESIGNAL_PRINT(data)         ESIGNAL_BASE(-1, data)
#define ESIGNAL_CRITICAL(data)      ESIGNAL_BASE(1, data)
#define ESIGNAL_ERROR(data)         ESIGNAL_BASE(2, data)
#define ESIGNAL_WARNING(data)       ESIGNAL_BASE(3, data)
#ifdef DEBUG
	#define ESIGNAL_INFO(data)          ESIGNAL_BASE(4, data)
	#define ESIGNAL_DEBUG(data)         ESIGNAL_BASE(5, data)
	#define ESIGNAL_VERBOSE(data)       ESIGNAL_BASE(6, data)
	#define ESIGNAL_TODO(data)          ESIGNAL_BASE(4, "TODO : " << data)
#else
	#define ESIGNAL_INFO(data)          do { } while(false)
	#define ESIGNAL_DEBUG(data)         do { } while(false)
	#define ESIGNAL_VERBOSE(data)       do { } while(false)
	#define ESIGNAL_TODO(data)          do { } while(false)
#endif

#define ESIGNAL_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			ESIGNAL_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)

