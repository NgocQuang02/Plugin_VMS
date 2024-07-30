#include "camera_manager.h"
#include "dahua_camera/dahua_camera.h"
#include "oryza_camera/oryza_camera.h"

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {
					std::shared_ptr<ICamera> CameraManager::createInstance(const std::string& deviceID, const std::string& ip) {
						// if (deviceID.find("Dahua") != std::string::npos) 
						// {
						// 	return std::make_shared<DahuaCamera>();
						// }
						// else 
						// {
							// LOG("Camera Oryza created");
							return std::make_shared<OryzaCamera>();
						// }
					}

					std::map<CameraManager::CameraKey, std::shared_ptr<ICamera>>& CameraManager::getCameraMap() {
						static std::map<CameraKey, std::shared_ptr<ICamera>> cameraMap;
						return cameraMap;
					}

					std::shared_ptr<ICamera> CameraManager::getInstance(const std::string& deviceID, const std::string& ip) {
						auto& cameraMap = getCameraMap();
						CameraKey key = std::make_tuple(deviceID, ip);

						if (cameraMap.find(key) == cameraMap.end()) {
							std::shared_ptr<ICamera> newCameraInstance = createInstance(deviceID, ip);
							cameraMap[key] = newCameraInstance;
						}

						return cameraMap[key];
					}

				}
			}
		}
	}
}
