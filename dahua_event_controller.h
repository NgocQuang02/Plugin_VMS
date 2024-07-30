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

#include "../i_event_controller.h"
#include "../../Logger.h"

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {

					class DahuaEventController : public IEventController
					{
					public:
						DahuaEventController();
						 ~DahuaEventController();

					public:
						void capture(DEV_EVENT_FACERECOGNITION_INFO* pstInfo, BYTE* pBuffer, DWORD dwBufSize, LDWORD dwUser);

						void saveFaceDetection(DEV_EVENT_FACERECOGNITION_INFO* pstInfo);

						void saveFaceRecognition(CANDIDATE_INFOEX* pstCandidate);
					
					private:
					};

				} // namespace sample
			}
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx