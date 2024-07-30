#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
#include "define_var.h"


namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {
					class IEventController
					{
					public:
						explicit IEventController();
						 ~IEventController();
					
					public:
						bool callback();

						void setCallback(bool newCallback);

						bool callbackEvent();

						void setCallBackEvent(bool newCallback);

						std::string StatusCode();

						void setStatusCode(std::string StatusCode);

						bool isRecognized();

						void setIsRecognized(bool isRecognized);

						std::string ipServer();

						void setIpServer(std::string ipServer);

						std::string ipClient();

						void setIpClient(std::string ipClient);

						std::string timeAlarm();

						void setTimeAlarm(std::string timeAlarm);

						std::string Cam();

						void setCam(std::string Cam);

						std::vector<std::string> faceDataInfo();

						void setFaceDataInfo(std::vector<std::string> faceDataInfo);

						std::vector<std::string> recognitionDataInfo();

						void setRecognitionDataInfo(std::vector<std::string> recognitionDataInfo);

						std::string username();

						void setUsername(std::string username);

						std::string password();

						void setPassword(std::string passowrd);

						std::string host();

						void setHost(std::string host);

						std::string imgpath();

						void setImgpath(std::string imgpath);

						std::string port();

						void setPort(std::string port);

						std::string deviceId();

						void setdeviceId(std::string deviceId);

						std::string url();

						void setUrl(std::string url);

						std::string Points();

						void setPoints(std::string Points);

						int Mincnt();

                        void setMincnt(int Min);

					private:
						bool m_callback = 0;
						bool m_callbackevent = 0;
						bool m_isRecognized = 0;
						std::string m_port;
						std::vector<std::string> m_facedataInfo;
						std::vector<std::string> m_recognitionDataInfo;
						std::string m_username;
						std::string m_password;
						std::string m_host;
						std::string m_imgpath;
						std::string m_deviceId;
						std::string m_statuscode;
						std::string m_url;
						std::string m_ipServer;
						std::string m_cam;
						std::string m_points;
						std::string m_timeAlarm;
						std::string m_ipClient;
						int m_min;
					};
				} // namespace sample
			}
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx
