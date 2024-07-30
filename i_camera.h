#ifndef ICAMERA_H
#define ICAMERA_H

#include <functional>
#include <string>
#include <cstring>
#include <vector>
#include <memory>

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				class ICamera {
				public:
					 virtual void initialize() = 0;
					 virtual void configure() = 0;
					 virtual bool login() = 0;
					 virtual bool logout() = 0;
					 virtual bool subscribe() = 0;
					 virtual bool unsubscribe() = 0;
					 virtual void doAction() = 0;

					 virtual std::string deviceId() const = 0;
					 virtual void setDeviceId(const std::string deviceId) = 0;

					 virtual void setCallBack(const bool& callback) = 0;
					 virtual bool callback() const = 0;

					 virtual void setCallBackEvent(const bool& callbackEvent) = 0;
					 virtual bool callbackEvent() const = 0;

					 virtual std::string StatusCode() const = 0; 
					 virtual void setStatusCode(const std::string& StatusCode) = 0;

					 virtual std::string Points() const = 0; 
					 virtual void setPoints(const std::string& Points) = 0;

					 virtual void setIsRecognized(const bool& isRecognized) = 0;
					 virtual bool isRecognized() const = 0;

					 virtual std::string imgpath() const = 0;
					 virtual void setImgpath(const std::string& imgpath) = 0;

					 virtual std::string ipClient() const = 0;
					 virtual void setIpClient(const std::string& ipClient) = 0;

					 virtual std::string ipServer() const = 0;
					 virtual void setIpServer(const std::string& ipServer) = 0;

					virtual std::string timeAlarm() const = 0;
					 virtual void setTimeAlarm(const std::string& ipServer) = 0;

					 virtual std::string Cam() const = 0;
					 virtual void setCam(const std::string& Cam) = 0;

					 virtual std::string host() const = 0;
					 virtual void setHost(const std::string& host) = 0;

					 virtual std::string port() const = 0;
					 virtual void setPort(const std::string& host) = 0;

					 virtual std::string username() const = 0;
					 virtual void setUsername(const std::string& username) = 0;

					 virtual std::string password() const = 0;
					 virtual void setPassword(const std::string& password) = 0;

					 virtual std::string url() const = 0;
					 virtual void setUrl(const std::string& url) = 0;

					 virtual std::vector<std::string> faceDataInfo() const = 0;
					 virtual void setFaceDataInfo(const std::vector<std::string> faceDataInfo) = 0;

					 virtual std::vector<std::string> recognitionDataInfo() const = 0;
					 virtual void setRecognitionDataInfo(const std::vector<std::string> faceRecognitionInfo) = 0;

					 virtual std::vector<std::string> trafficInfo() const = 0;
					 virtual void setTrafficInfo(const std::vector<std::string> trafficInfo) = 0;

					 virtual int Mincnt() const = 0;
					 virtual void setMincnt(const int Min) = 0;

				protected:
					std::string m_deviceId;
				};
			} // namespace stub
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx

#endif // ICAMERA_H
