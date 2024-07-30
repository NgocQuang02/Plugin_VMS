#pragma once
#include <dhnetsdk.h>
#ifdef WIN64
#pragma comment(lib, "dhnetsdk.lib")
#endif
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <map>
#include <chrono>
#include "../i_event_controller.h"

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {

					class OryzaEventController : public IEventController
					{
					public:
						OryzaEventController();
						~OryzaEventController();

					public:
						void capture(BYTE* pBuffer, DWORD dwBufSize, LDWORD User);

						void saveFaceRecognition(std::vector<std::string> pstInfo);
					};

				} // namespace sample
			}
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx