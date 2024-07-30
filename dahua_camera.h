#pragma once
#include <dhnetsdk.h>
#ifdef WIN64
#pragma comment(lib, "dhnetsdk.lib")
#endif
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "../../i_camera.h"

#include "dahua_event_controller.h"
#include "dahua_SDK_connection.h"

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {
					int CALLBACK DataCallback(LLONG lAnalyzerHandle, DWORD dwAlarmType, void* pAlarmInfo, BYTE* pBuffer, DWORD dwBufSize, LDWORD dwUser, int nSequence, void* reserved);

					void CALLBACK DisconnectCallback(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser);

					void CALLBACK ReconnectCallback(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser);

					class WorkerThread;

					class DahuaCamera : public ICamera {
					public:
						DahuaCamera();
						~DahuaCamera();

					public:
						void initialize() override;
						void configure() override;
						bool login() override;
						bool logout() override;
						bool subscribe() override;
						bool unsubscribe() override;
						void doAction() override;

						bool callback() const override;
						void setCallBack(const bool& callback) override;

						bool isRecognized() const override;
						void setIsRecognized(const bool& isRecognized) override;

						std::string deviceId() const override;
						void setDeviceId(const std::string deviceId) override;

						std::string imgpath() const override;
						void setImgpath(const std::string& imgpath) override;

						std::string host() const override;
						void setHost(const std::string& host) override;

						std::string port() const override;
						void setPort(const std::string& port) override;

						std::string username() const override;
						void setUsername(const std::string& username) override;

						std::string password() const override;
						void setPassword(const std::string& password) override;

						std::string url() const override;
						void setUrl(const std::string& url) override;

						std::string Points() const override;
						void setPoints(const std::string& Points) override;

						std::vector<std::string> faceDataInfo() const override;
						void setFaceDataInfo(const std::vector<std::string> faceDataInfo) override;

						std::vector<std::string> recognitionDataInfo() const override;
						void setRecognitionDataInfo(const std::vector<std::string> faceRecognitionInfo) override;

						std::vector<std::string> trafficInfo() const override;
						void setTrafficInfo(const std::vector<std::string> trafficInfo) override;

						void saveFaceDetection(DEV_EVENT_FACERECOGNITION_INFO* pstInfo);

						void saveFaceRecognition(CANDIDATE_INFOEX* pstInfo);

						void capture(DEV_EVENT_FACERECOGNITION_INFO* pstInfo, BYTE* pBuffer, DWORD dwBufSize, LDWORD dwUser);

						void setDataToBackend(CANDIDATE_INFOEX* pstInfo);

					private:
						std::shared_ptr<DahuaEventController> p_ev_cntrl;

						std::shared_ptr<DahuaSDKConnection> p_connection;

					private:
						WorkerThread* workerThread;
					};
				} // namespace sample
			}
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx