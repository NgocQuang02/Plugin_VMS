#include "oryza_event_controller.h"
#include "oryza_camera.h"
#ifdef _WIN32
#else
#include "dlfcn.h"
#include <unistd.h>
#endif
inline std::string DateFormat(int year, int month, int day)
{
	char buf[128] = { 0 };
	snprintf(buf, sizeof(buf), "%02d-%02d-%04d",
		day,
		month,
		year);
	return std::string(buf);
}

inline std::string NetTimeEx2StrEx(const NET_TIME_EX& netTime)
{
	char buf[128] = { 0 };
	snprintf(buf, sizeof(buf), "%04d%02d%02d%02d%02d%02d%03d",
		netTime.dwYear,
		netTime.dwMonth,
		netTime.dwDay,
		netTime.dwHour,
		netTime.dwMinute,
		netTime.dwSecond,
		netTime.dwMillisecond);
	return std::string(buf);
}



namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {

					OryzaEventController::OryzaEventController() {

					}
					OryzaEventController::~OryzaEventController() {

					}

					void OryzaEventController::capture(BYTE* pBuffer, DWORD dwBufSize, LDWORD User) {
					//	std::string strStoreImgPath;
					//	int lastSlashPos;
					//	std::string folderImage;
					//	LOG("CREATE IMAGE!!!!!!!!!");
					//#ifdef WIN32
					//	HMODULE hModule = NULL;

					//	hModule = ::GetModuleHandle("detection_analytics_plugin.dll");
					//	LOG("IMAGE1");
					//	char szFullDllPath[_MAX_PATH] = { 0 };
					//	char szModuleDir[_MAX_PATH] = { 0 };

					//	::GetModuleFileName(hModule, szFullDllPath, _MAX_PATH);
					//	char* lastBackslash = strrchr(szFullDllPath, '\\');
					//	size_t lastBackslashPos = lastBackslash - szFullDllPath + 1;
					//	strncpy(szModuleDir, szFullDllPath, lastBackslashPos);
					//	strStoreImgPath = std::string(szModuleDir);
					//	strStoreImgPath.append("StoreImageFace\\");
					//	LOG(strStoreImgPath);
					//#else
					//	char buffer[4096];
					//	if (getcwd(buffer, sizeof(buffer)) != nullptr) {
					//		std::string currentDirectory(buffer);
					//		memset(buffer, 0, sizeof(buffer));
					//		strStoreImgPath = currentDirectory.append("/StoreImageFace/");
					//	}
					//#endif
					//	LOG("IMAGE4");
					//	auto currentTime = std::chrono::system_clock::now();
					//	auto timePoint = std::chrono::system_clock::to_time_t(currentTime);
					//	auto timeInfo = std::localtime(&timePoint);

					//	int day = timeInfo->tm_mday;
					//	int month = timeInfo->tm_mon + 1;
					//	int year = timeInfo->tm_year + 1900;

					//	int hours = timeInfo->tm_hour;
					//	int minutes = timeInfo->tm_min;
					//	int seconds = timeInfo->tm_sec;
					//	auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
					//		currentTime.time_since_epoch()
					//	).count();
					//	miliseconds %= 1000;

					//	std::string currentTimeStr = std::to_string(year) + std::to_string(month) + std::to_string(day) + std::to_string(hours) + std::to_string(minutes) + std::to_string(seconds) + std::to_string(miliseconds);
					//	std::string strJpgFile = strStoreImgPath + std::string(currentTimeStr) + std::string(".jpg");
					//	LOG("IMAGE5");
					//	reinterpret_cast<OryzaCamera*>(User)->setImgpath(strJpgFile);
					//	LOG("IMAGE6");
					//	LOG(strJpgFile);
					//	FILE* fp = fopen(strJpgFile.c_str(), "wb");

					//	if (fp == NULL)
					//	{
					//		Logger::instance()->writeLogServer(__func__, "Save picture failed!");
					//		return;
					//	}
					//	LOG("IMAGE7");
					//	fwrite(pBuffer, dwBufSize, 1, fp);
					//	fclose(fp);
					//	Logger::instance()->writeLogServer(__func__, "create image success : " + strJpgFile);
					}

					void OryzaEventController::saveFaceRecognition(std::vector<std::string> pstInfo) {
						this->setRecognitionDataInfo(pstInfo);
					}
				}
			} // namespace sample
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx