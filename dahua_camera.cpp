#include <nx/sdk/analytics/helpers/consuming_device_agent.h>
#ifdef __linux__
#include <dlfcn.h>
#endif

#include "dahua_camera.h"
#include "../oryza_camera/WorkerThread.h"
#include "../../json11.hpp"

using namespace nx::vms_server_plugins::analytics::stub::CrowdDetection;

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {

					DahuaCamera::DahuaCamera() {
						workerThread = new WorkerThread();
					}

					DahuaCamera::~DahuaCamera() {
						delete workerThread;
					}

					void DahuaCamera::initialize()
					{
						p_ev_cntrl.reset(new DahuaEventController());
						p_connection.reset(new DahuaSDKConnection());
					}

					void DahuaCamera::configure()
					{
					}

					bool DahuaCamera::login()
					{
						p_connection->login((LDWORD)this);
						return p_connection->loginSuccess();
					}

					bool DahuaCamera::logout()
					{
						p_connection->logout((LDWORD)this);
						return true;
					}

					bool DahuaCamera::subscribe()
					{
						p_connection->subscribe((LDWORD)this);
						return p_connection->subscribeSuccess();
					}

					bool DahuaCamera::unsubscribe()
					{
						p_connection->unsubscribe((LDWORD)this);
						return true;
					}

					void DahuaCamera::doAction()
					{

					}

					void DahuaCamera::capture(DEV_EVENT_FACERECOGNITION_INFO* pstInfo, BYTE* pBuffer, DWORD dwBufSize , LDWORD dwUser)
					{
						p_ev_cntrl->capture(pstInfo, pBuffer, dwBufSize, dwUser);
					}

					void DahuaCamera::setCallBack(const bool& callback)
					{
						p_ev_cntrl->setCallback(callback);
					}

					bool DahuaCamera::isRecognized() const
					{
						return p_ev_cntrl->isRecognized();
					}

					void DahuaCamera::setIsRecognized(const bool& isRecognized)
					{
						p_ev_cntrl->setIsRecognized(isRecognized);
					}

					bool DahuaCamera::callback() const
					{
						return p_ev_cntrl->callback();
					}

					std::string DahuaCamera::deviceId() const
					{
						return m_deviceId;
					}

					void DahuaCamera::setDeviceId(const std::string deviceId)
					{
						m_deviceId = deviceId;
					}

					std::string DahuaCamera::imgpath() const
					{
						return p_ev_cntrl->imgpath();
					}

					void DahuaCamera::setImgpath(const std::string& imgpath)
					{
						p_ev_cntrl->setImgpath(imgpath);
					}

					std::string DahuaCamera::host() const
					{
						return p_ev_cntrl->host();
					}

					void DahuaCamera::setPort(const std::string& port)
					{
						p_ev_cntrl->setPort(port);
					}

					std::string DahuaCamera::port() const
					{
						return p_ev_cntrl->port();
					}

					void DahuaCamera::setHost(const std::string& host)
					{
						p_ev_cntrl->setHost(host);
					}

					std::string DahuaCamera::username() const
					{
						return p_ev_cntrl->username();
					}

					void DahuaCamera::setUsername(const std::string& username)
					{
						p_ev_cntrl->setUsername(username);
					}

					std::string DahuaCamera::password() const
					{
						return p_ev_cntrl->password();
					}

					void DahuaCamera::setPassword(const std::string& password)
					{
						p_ev_cntrl->setPassword(password);
					}

					std::string DahuaCamera::url() const
					{
						return p_ev_cntrl->url();
					}

					void DahuaCamera::setUrl(const std::string& url)
					{
						p_ev_cntrl->setUrl(url);
					}

					std::string DahuaCamera::Points() const
					{
						return p_ev_cntrl->Points();
					}

					void DahuaCamera::setPoints(const std::string& Points)
					{
						p_ev_cntrl->setPoints(Points);
					}

					std::vector<std::string> DahuaCamera::faceDataInfo() const
					{
						return p_ev_cntrl->faceDataInfo();
					}

					void DahuaCamera::setFaceDataInfo(const std::vector<std::string> faceDataInfo)
					{
						p_ev_cntrl->setFaceDataInfo(faceDataInfo);
					}

					std::vector<std::string> DahuaCamera::recognitionDataInfo() const
					{
						return p_ev_cntrl->recognitionDataInfo();
					}

					void DahuaCamera::setRecognitionDataInfo(const std::vector<std::string> faceRecognitionInfo)
					{
						p_ev_cntrl->setRecognitionDataInfo(faceRecognitionInfo);
					}

					std::vector<std::string> DahuaCamera::trafficInfo() const
					{
						return std::vector<std::string>();
					}

					void DahuaCamera::setTrafficInfo(const std::vector<std::string> trafficInfo)
					{

					}

					void DahuaCamera::saveFaceDetection(DEV_EVENT_FACERECOGNITION_INFO* pstInfo)
					{
						p_ev_cntrl->saveFaceDetection(pstInfo);
					}

					void DahuaCamera::saveFaceRecognition(CANDIDATE_INFOEX* pstInfo)
					{
						p_ev_cntrl->saveFaceRecognition(pstInfo);
					}

					void DahuaCamera::setDataToBackend(CANDIDATE_INFOEX* pstInfo)
					{
						FACERECOGNITION_PERSON_INFOEX* pstPersonInfo = &(pstInfo->stPersonInfo);
						std::shared_ptr<UserData> sendData(new UserData());

						json11::Json my_json_msg = json11::Json::object{
						{ "id",pstPersonInfo->szID},
						{ "file", this->imgpath()},
						{ "id_cam", this->deviceId()},
						};
						sendData->msg = my_json_msg.dump();
						workerThread->CreateThread();
						workerThread->callHttp(sendData);
						workerThread->ExitThread();
					}

					void CALLBACK DisconnectCallback(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser)
					{

					}

					void CALLBACK ReconnectCallback(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser)
					{

					}

					int CALLBACK DataCallback(LLONG lAnalyzerHandle, DWORD dwAlarmType, void* pAlarmInfo, BYTE* pBuffer, DWORD dwBufSize, LDWORD dwUser, int nSequence, void* reserved)
					{
						DahuaCamera* dahua_cam = (DahuaCamera*)dwUser;

						DEV_EVENT_FACERECOGNITION_INFO* pstInfo = (DEV_EVENT_FACERECOGNITION_INFO*)pAlarmInfo;
						memcpy(pstInfo, (DEV_EVENT_FACEDETECT_INFO*)pAlarmInfo, sizeof(DEV_EVENT_FACEDETECT_INFO));

						dahua_cam->saveFaceDetection(pstInfo);
						CANDIDATE_INFOEX* pstCandidate = &(pstInfo->stuCandidatesEx[0]);
						if (pstInfo->nCandidateNum) 
						{
							for (int i = 0; i < pstInfo->nCandidateNum; i++)
							{
								if (NULL != pstCandidate)
								{
									if (pstInfo->stuCandidatesEx[i].bySimilarity > pstCandidate->bySimilarity)
									{
										pstCandidate = &(pstInfo->stuCandidatesEx[i]);
									}
								}
							}

							dahua_cam->saveFaceRecognition(pstCandidate);
							dahua_cam->setIsRecognized(true);
							
						}

						// Store Images
						dahua_cam->capture(pstInfo, pBuffer, dwBufSize, dwUser);
						dahua_cam->setCallBack(true);
						dahua_cam->setDataToBackend(pstCandidate);

						return 0;
					}

				};
			} // namespace sample
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx