#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#define LOG(var) Logger::instance()->writeLogServer(__func__, var)
// #define ADDR(var) Logger::instance()->writeAddr(__func__, var)

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
					class Logger
					{
					public:
						explicit Logger();
						virtual ~Logger();

						static Logger* instance();

						void writeLogServer(std::string func = "[Funtion]", std::string logText = "\n");

					private:
						static Logger* m_instance;

					};
				} // namespace sample
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx
