#ifndef _MY_LOGGING_H_
#define _MY_LOGGING_H_

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
using namespace boost::log::trivial;
namespace MyRedis {

class logAdapter {
public:

 enum elogLevel {
   eLOG_LEVEL_DISABLE = 0,
   eLOG_LEVEL_FATAL = 1,
   eLOG_LEVEL_ERROR = 2,
   eLOG_LEVEL_WARN = 3,
   eLOG_LEVEL_INFO = 4,
   eLOG_LEVEL_DEBUG = 5,
   eLOG_LEVEL_TRACE = 6,
   eLOG_LEVEL_LEVEL_NUM = 7
 };

 public:
      ~logAdapter();
      static logAdapter& getLogInstance();
      void setLogLevel(elogLevel logLevel);
      elogLevel getLogLevel();
      void setLogFileNumAndSize(int logNum, int sizeOfPerFile);
      src::severity_logger<boost::log::trivial::severity_level>& getSeverityLogger() {
            return m_severityLogger;
      }

 private:
      logAdapter();
      elogLevel m_logLevel;
      std::string m_logFile;
      src::severity_logger<boost::log::trivial::severity_level> m_severityLogger;
      typedef sinks::synchronous_sink<sinks::text_file_backend> logSink_t;
      boost::shared_ptr<logSink_t> m_logSink;
};

#define ALOG_ADAPTER logAdapter::getLogInstance()

#define AGENT_LOGGER ALOG_ADAPTER.getSeverityLogger()

/*class LogUtil {
 public:
     static void VLogError(boost::log::trivial::severity_level level, const char* format, ...);
     static void VLogError(boost::log::trivial::severity_level level, const char* format, ...) {
            va_list arg_ptr;
            va_start(arg_ptr, format);
            boost::scoped_array<char> formattedString(new char[1024]);
            vsnprintf(formattedString.get(), 1024, format, arg_ptr);
            BOOST_LOG_SEV(AGENT_LOGGER, level) << formattedString.get();
            va_end(arg_ptr);
      }

};
*/
void VLogError(boost::log::trivial::severity_level level, const char* format, ...);
/*
#define LOG_FATAL(format, args...) LogUtil::VLogError(boost::log::trivial::fatal, format, ##args)
#define LOG_ERROR(format, args...) LogUtil::VLogError(boost::log::trivial::error, format, ##args)
#define LOG_WARN(format, args...) LogUtil::VLogError(boost::log::trivial::warning, format, ##args)
#define LOG_INFO(format, args...) LogUtil::VLogError(boost::log::trivial::info, format, ##args)
#define LOG_DEBUG(format, args...) LogUtil::VLogError(boost::log::trivial::debug, format, ##args)
*/
#define LOG_FATAL(format, args...) VLogError(boost::log::trivial::fatal, format, ##args)
#define LOG_ERROR(format, args...) VLogError(boost::log::trivial::error, format, ##args)
#define LOG_WARN(format, args...)  VLogError(boost::log::trivial::warning, format, ##args)
#define LOG_INFO(format, args...)  VLogError(boost::log::trivial::info, format, ##args)
#define LOG_DEBUG(format, args...) VLogError(boost::log::trivial::debug, format, ##args)


}
#endif //_MY_LOGGING_H_


