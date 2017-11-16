#include "my_logging.h"
#define BOOST_DATE_TIME_SOURCE

namespace MyRedis {

logAdapter::~logAdapter() {
    logging::core::get()->remove_all_sinks();
}

logAdapter& logAdapter::getLogInstance() {
      static logAdapter alogInstance;
      return alogInstance;
}

logAdapter::logAdapter() : m_logLevel(eLOG_LEVEL_INFO) {
  //string homeDir(UtilAll::getHomeDirectory());
  std::string homeDir("./");
  homeDir.append("/logs");
  m_logFile += homeDir;
  std::string fileName = "my_redis.log";
  m_logFile += fileName;

  // boost::log::expressions::attr<
  // boost::log::attributes::current_thread_id::value_type>("ThreadID");
  boost::log::register_simple_formatter_factory<
      boost::log::trivial::severity_level, char>("Severity");
  m_logSink = logging::add_file_log(
      keywords::file_name = m_logFile,
      keywords::rotation_size = 500  * 1024 * 1024,
      keywords::time_based_rotation =
          sinks::file::rotation_at_time_point(0, 0, 0),
      keywords::format = "[%TimeStamp%][%ThreadID%](%Severity%):%Message%",
      keywords::min_free_space = 300 * 1024 * 1024, keywords::target = homeDir,
      keywords::max_size = 20 * 1024 * 1024,  // max keep 3 log file defaultly
      keywords::auto_flush = true);
  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::info);

  logging::add_common_attributes();
  //logging::core::get()->add_thread_attribute("Scope", attrs::named_scope());

}

void logAdapter::setLogLevel(elogLevel logLevel) {
  m_logLevel = logLevel;
  switch (logLevel) {
    case eLOG_LEVEL_DISABLE:
      logging::core::get()->set_filter(logging::trivial::severity >
                                       logging::trivial::fatal);

      break;
    case eLOG_LEVEL_FATAL:
      logging::core::get()->set_filter(logging::trivial::severity >=
                                       logging::trivial::fatal);
      break;
    case eLOG_LEVEL_ERROR:
      logging::core::get()->set_filter(logging::trivial::severity >=
                                       logging::trivial::error);

      break;
    case eLOG_LEVEL_WARN:
      logging::core::get()->set_filter(logging::trivial::severity >=
                                       logging::trivial::warning);

      break;
    case eLOG_LEVEL_INFO:
      logging::core::get()->set_filter(logging::trivial::severity >=
                                       logging::trivial::info);

      break;
    case eLOG_LEVEL_DEBUG:
      logging::core::get()->set_filter(logging::trivial::severity >=
                                       logging::trivial::debug);

      break;
    case eLOG_LEVEL_TRACE:
      logging::core::get()->set_filter(logging::trivial::severity >=
                                       logging::trivial::trace);

      break;
    default:
      logging::core::get()->set_filter(logging::trivial::severity >=
                                       logging::trivial::info);

      break;
  }
}

logAdapter::elogLevel logAdapter::getLogLevel() { return m_logLevel; }

void logAdapter::setLogFileNumAndSize(int logNum, int sizeOfPerFile) {
  //string homeDir(UtilAll::getHomeDirectory());
  std::string homeDir("./");
  homeDir.append("/logs");
  m_logSink->locked_backend()->set_file_collector(sinks::file::make_collector(
      keywords::target = homeDir,
      keywords::max_size = logNum * sizeOfPerFile * 1024 * 1024));
}

void VLogError(boost::log::trivial::severity_level level, const char* format, ...) {
      va_list arg_ptr;
      va_start(arg_ptr, format);
      boost::scoped_array<char> formattedString(new char[1024]);
      vsnprintf(formattedString.get(), 1024, format, arg_ptr);
      BOOST_LOG_SEV(AGENT_LOGGER, level) << formattedString.get();
      va_end(arg_ptr);
}

}
