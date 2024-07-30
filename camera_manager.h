#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <map>
#include <memory>
#include <string>
#include "../i_camera.h"

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {
					class CameraManager {
					private:
						CameraManager() {}

						static std::shared_ptr<ICamera> createInstance(const std::string& deviceID, const std::string& ip);

						typedef std::tuple<std::string, std::string> CameraKey;
						static std::map<CameraKey, std::shared_ptr<ICamera>>& getCameraMap();

					public:
						static std::shared_ptr<ICamera> getInstance(const std::string& deviceID, const std::string& ip);

						
					};
				}
			}
		}
	}
}
#endif // CAMERAMANAGER_H

