#include "dahua_SDK_connection.h"
#include "dahua_camera.h"
#include "dahua_event_controller.h"
#include "../../Logger.h"


namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {
					DahuaSDKConnection::DahuaSDKConnection() {
					}

					DahuaSDKConnection::~DahuaSDKConnection() {
						CLIENT_Cleanup();
					}

					void DahuaSDKConnection::login(LDWORD dwUser = NULL) {

						DahuaCamera* dahua_cam = (DahuaCamera*)dwUser;

						m_bNetSDKInitFlag = CLIENT_Init(DisconnectCallback, NULL);
						if (!m_bNetSDKInitFlag) return;

						CLIENT_SetAutoReconnect(ReconnectCallback, NULL);

						int nWaitTime = 5000; // Timeout is 5 seconds.
						int nTryTimes = 3; // If timeout, it will try to log in three times.

						CLIENT_SetConnectTime(nWaitTime, nTryTimes);

						NET_PARAM stuNetParm = { 0 };
						stuNetParm.nConnectTime = 3000; // The timeout of connection when login.
						CLIENT_SetNetworkParam(&stuNetParm);

						NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY stInparam;

						memset(&stInparam, 0, sizeof(stInparam));
						stInparam.dwSize = sizeof(stInparam);

						//string g_szHostStr = extractHostFromURL(deviceInfo->url());


						// Dynamic login
						std::string sz_host = dahua_cam->host();
						std::string sz_username = dahua_cam->username();
						std::string sz_password = dahua_cam->password();


						LOG("HOST: " + sz_host);
						LOG("USERNAME: " + sz_username);
						LOG("sz_password: " + sz_password);


						const char* host = sz_host.c_str();
						WORD g_nPort = 80;
						const char* username = sz_username.c_str();
						const char* password = sz_password.c_str();
						strncpy(stInparam.szIP, host, sizeof(stInparam.szIP) - 1);
						strncpy(stInparam.szPassword, password, sizeof(stInparam.szPassword) - 1);
						strncpy(stInparam.szUserName, username, sizeof(stInparam.szUserName) - 1);
						stInparam.nPort = g_nPort;
						stInparam.emSpecCap = EM_LOGIN_SPEC_CAP_TCP;

						NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY stOutparam;

						memset(&stOutparam, 0, sizeof(stOutparam));
						stOutparam.dwSize = sizeof(stOutparam);

						m_lLoginID = CLIENT_LoginWithHighLevelSecurity(&stInparam, &stOutparam);
						if (m_lLoginID)
							LOG("SUCCESS TO CONNECT");
						else
							LOG("FAIL TO CONNECT");
					}

					void DahuaSDKConnection::logout(LDWORD dwUser = NULL) {
						if (m_lLoginID) {
							CLIENT_Logout(m_lLoginID);
							m_lLoginID = 0;
							LOG("SUCCESS TO LOGOUT");
						}
					}

					void DahuaSDKConnection::subscribe(LDWORD dwUser = NULL) {
						if (!m_lLoginID) return;
						m_lRealPicHandle = CLIENT_RealLoadPictureEx(m_lLoginID, 0, (DWORD)EVENT_IVS_FACERECOGNITION, TRUE, DataCallback, (LDWORD)dwUser, NULL);
						if (m_lRealPicHandle) {
							LOG("SUCCESS TO SUBSCRIBE");
						}
					}

					void DahuaSDKConnection::unsubscribe(LDWORD dwUser = NULL) {
						if (m_lRealPicHandle) {
							CLIENT_StopLoadPic(m_lRealPicHandle);
							LOG("SUCCESS TO UNSUBSCRIBE");
							m_lRealPicHandle = 0;
						}
					}

					bool DahuaSDKConnection::loginSuccess()
					{
						return m_lLoginID != 0;
					}

					bool DahuaSDKConnection::subscribeSuccess()
					{
						return m_lRealPicHandle != 0;
					}
				};

			} // namespace sample
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx