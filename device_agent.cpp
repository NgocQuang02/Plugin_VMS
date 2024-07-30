// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/
#include <nx/sdk/analytics/helpers/consuming_device_agent.h>
#include <nx/sdk/helpers/settings_response.h>
#include <ctime>
#include <iostream>

#include "stub_analytics_plugin_FaceDetection_ini.h"

#define NX_DEBUG_ENABLE_OUTPUT true
#include "device_agent.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <thread>
#include <atomic>
#include "../Logger.h"
#include "../json11.hpp"
#include "oryza_camera/WorkerThread.h"

using namespace nx::sdk;

using namespace nx::sdk::analytics;

namespace nx
{
	namespace vms_server_plugins
	{
		namespace analytics
		{
			namespace stub
			{
				namespace CrowdDetection
				{

					using namespace nx::sdk;
					using namespace nx::sdk::analytics;

					Ptr<IMetadataPacket> metadataPacket = nullptr;

					/**
					 * @param deviceInfo Various information about the related device, such as its id, vendor, model,
					 *     etc.
					 */
					DeviceAgent::DeviceAgent(const nx::sdk::IDeviceInfo *deviceInfo) : // Call the DeviceAgent helper class constructor telling it to verbosely report to stderr.
																					   ConsumingDeviceAgent(deviceInfo, /*enableOutput*/ true)
					{
						m_deviceId = deviceInfo->logicalId();
						const std::string ip = extractHostFromURL(deviceInfo->url());
						const std::string port = extractPortFromURL_string(deviceInfo->url());
						std::string url(deviceInfo->url());
						std::string deviceId = deviceInfo->id();

						exitFlag.store(false);
						std::string check1 = (exitFlag ? "true" : "false");

						camera = CameraManager::getInstance(checkCam(deviceId) + deviceId, ip);

						camera->initialize();

						camera->setHost(ip);

						camera->setPort(port);

						camera->setDeviceId(deviceId);

						camera->setUrl(url); // send RTSP url

#ifdef WIN32
						createImageStorage("StoreImageFace\\");
#else
						createImageStorage("/StoreImageFace/");
#endif
					}

					DeviceAgent::~DeviceAgent()
					{

						exitFlag.store(true);
						std::string check3 = (exitFlag ? "true" : "false");
						//if (LoginThread.joinable()) {
						//	LoginThread.join();
						//}
						
								camera->unsubscribe();
								camera->logout();
						
						
					}

					/**
					 *  @return JSON with the particular structure. Note that it is possible to fill in the values
					 * that are not known at compile time, but should not depend on the DeviceAgent settings.
					 */
					std::string DeviceAgent::manifestString() const
					{
						// Tell the Server that the plugin can generate the events and objects of certain types.
						// Id values are strings and should be unique. Format of ids:
						// `{vendor_id}.{plugin_id}.{event_type_id/object_type_id}`.
						//
						// See the plugin manifest for the explanation of vendor_id and plugin_id.
						return /*suppress newline*/ 1 + (const char *)R"json(
{
        "objectTypes": 
		[
			{
				"id": ")json" + kNewFaceDetectionObjectType + R"json(",
				"name": "Crowd Detected"
			}
		]
}
)json";

					}

					void DeviceAgent::Ping2Server(std::string ipserver)
					{
						int temp = 0 ;
						while (!exitFlag.load())
						{
							try
							{

							//std::string check2 = (exitFlag ? "true" : "false");
							//LOG("EXIT FLAG PING 2 SERVER: ");
							boost::asio::io_service io_service;
							tcp::resolver resolver(io_service);
							tcp::resolver::query query(camera->ipServer(), "8080");
							tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

							tcp::socket socket(io_service);
							boost::asio::connect(socket, endpoint_iterator);

							// Create the HTTP POST request
							std::string request = "POST /ping HTTP/1.1\r\n"
									"Host: " + camera->ipServer() + ":8080\r\n"
									"Authorization: Bearer W+wBZ1TcC-RMcQM4ywXg\r\n"
									 "Content-Type: application/json\r\n"
									 "Content-Length: " +std::to_string(4) + "\r\n"
									 "Connection: close\r\n"
									 "\r\nkhoi";
							//LOG("REQUEST CALL API: \n");

							boost::asio::write(socket, boost::asio::buffer(request));

							boost::asio::streambuf response;
							boost::asio::read_until(socket, response, "\r\n");

							std::string response_str;
							std::istream response_stream(&response);
							while (!response_stream.eof())
							{
								char buffer[1000000];
								response_stream.read(buffer, sizeof(buffer));
								response_str.append(buffer, response_stream.gcount());
								memset(buffer, 0, sizeof(buffer));
							}

							LOG("HTTP Response_postUserData:\n");
							LOG(response_str);

							size_t status_code_pos = response_str.find("200");

							if (status_code_pos != std::string::npos)
							{
								LOG("STATUS_CODE_PING2SERVER: " + camera->StatusCode());
							}
							else
							{
								LOG("Server dead.");
								camera->setStatusCode("400");
								RecallLogin();
								break;
							}
							}
							catch (std::exception &e)
								{
									LOG("Exception: " + std::string(e.what()) + "\n");
									camera->setStatusCode("400");
									RecallLogin();
									break;
								}



							std::this_thread::sleep_for(std::chrono::seconds(5));
						}
					}

					void DeviceAgent::RecallLogin()
					{
						const auto settingsResponse = new sdk::SettingsResponse();
						while (!exitFlag.load())
						{
							LOG("RECALL LOGIN");
							if (camera->login() && camera->subscribe())
							{
								LOG("LOGIN THANH CONG");
								camera->setCallBackEvent(TRUE);
								// settingsResponse->setModel(kLoginSuccessSettingsModel);
								std::thread PingThread = std::thread(&DeviceAgent::Ping2Server, this, camera->ipServer());
								PingThread.detach();
								pushManifest(manifestString());
								break;
							}
							else if (camera->StatusCode() == "0")
							{
								LOG("KHONG MO DUOC RTSP");
								camera->setCallBackEvent(TRUE);
								pushManifest(manifestString());
								break;
							}
							else
							{
								LOG("LOGIN KHONG THANH CONG");
								//settingsResponse->setModel(kLoginSettingsModel);
							}
							std::this_thread::sleep_for(std::chrono::seconds(5));
						}
						disconnected = true;
					}

					nx::sdk::Result<const nx::sdk::ISettingsResponse *> DeviceAgent::settingsReceived()
					{
						const auto settingsResponse = new sdk::SettingsResponse();

						std::string username = settingValue(kUsernameSetting);
						std::string password = settingValue(kPasswordSetting);
						std::string ip = settingValue(kIpSetting);
						std::string ipRecv = settingValue(kIpRecv);
						std::string timeAlarm = settingValue(kTime);
						std::string mincnt = settingValue("MinCount");
						//std::string cam = settingValue("testGermanRadioButtonGroup");
						const std::string jsonString = settingValue("excludedArea.figure");
						std::string findStr = "points";
						std::string point = "";
						size_t tokenPos = jsonString.find(findStr);
						if (tokenPos != std::string::npos) {
							size_t valueStart = tokenPos + findStr.size(); // Move to the start of the token value
							size_t valueEnd = jsonString.find_first_of("}", valueStart);
							if (valueEnd != std::string::npos) {
								// Extract the token value
								point =  jsonString.substr(valueStart + 2, valueEnd - valueStart-2);
							}
						}
						

						camera->setIpServer(ip);
						camera->setMincnt(stoi(mincnt));
						camera->setPoints(point);
						camera->setTimeAlarm(timeAlarm);
						camera->setIpClient(ipRecv);

						if (username == camera->username() && password == camera->password())
							return nullptr;
						camera->setUsername(username);
						camera->setPassword(password);
						
						// if (username == camera->username() && password == camera->password())
						// 	return nullptr;
						if (!exitFlag.load())
						{
							LoginThread = std::thread(&DeviceAgent::RecallLogin, this);
							LoginThread.detach();
						}
						



						// if (camera->login() && camera->subscribe())
						// 	{
						// 		LOG("STATUS_CODE_LOGIN: " + camera->StatusCode());
						// 		LOG("LOGIN THANH CONG");
						// 		// settingsResponse->setModel(kLoginSuccessSettingsModel);
						// 		// std::thread PingThread = std::thread(&DeviceAgent::Ping2Server, this, camera->ipServer());
						// 		// PingThread.detach();
						// 	}
						// 	else
						// 	{
						// 		LOG("LOGIN KHONG THANH CONG");
						// 	}



						return settingsResponse;
					}
					/**
					 * Called when the Server sends a new uncompressed frame from a camera.
					 */
					bool DeviceAgent::pushUncompressedVideoFrame(const IUncompressedVideoFrame *videoFrame)
					{
						++m_frameIndex;
						m_lastVideoFrameTimestampUs = videoFrame->timestampUs();
						if (!camera->callbackEvent())
							return true;

						camera->setCallBackEvent(false);
						return true;
						auto eventMetadataPacket = generateEventMetadataPacket();
						if (eventMetadataPacket)
						{
							// Send generated metadata packet to the Server.
							pushMetadataPacket(eventMetadataPacket.releasePtr());
						}
						return true; //< There were no errors while processing the video frame.
					}

					/**
					 * Serves the similar purpose as pushMetadataPacket(). The differences are:
					 * - pushMetadataPacket() is called by the plugin, while pullMetadataPackets() is called by Server.
					 * - pushMetadataPacket() expects one metadata packet, while pullMetadataPacket expects the
					 *     std::vector of them.
					 *
					 * There are no strict rules for deciding which method is "better". A rule of thumb is to use
					 * pushMetadataPacket() when you generate one metadata packet and do not want to store it in the
					 * class field, and use pullMetadataPackets otherwise.
					 */
					bool DeviceAgent::pullMetadataPackets(std::vector<IMetadataPacket *> *metadataPackets)
					{
						// Lấy thời gian hiện tại (real time) tại điểm đó trong hàm
						auto currentTime = std::chrono::system_clock::now();

						if (!camera->callback())
							return true;

						mutex.lock();
						camera->setCallBack(false);
						*metadataPackets = generateMetdataWithBestShot();
						mutex.unlock();

						// Chuyển thời gian hiện tại thành milliseconds
						auto currentTimeMillis = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime);

						// Lấy giá trị milliseconds
						auto timeSinceEpoch = currentTimeMillis.time_since_epoch();
						long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceEpoch).count();

						return true; //< There were no errors while filling metadataPackets.
					}

					void DeviceAgent::doSetNeededMetadataTypes(
						nx::sdk::Result<void> * /*outValue*/,
						const nx::sdk::analytics::IMetadataTypes * /*neededMetadataTypes*/)
					{
					}

					//-------------------------------------------------------------------------------------------------
					// private

					Ptr<IMetadataPacket> DeviceAgent::generateEventMetadataPacket()
					{
						// Generate event every kTrackFrameCount'th frame.

						// EventMetadataPacket contains arbitrary number of EventMetadata.
						const auto eventMetadataPacket = makePtr<EventMetadataPacket>();
						// Bind event metadata packet to the last video frame using a timestamp.

						// eventMetadataPacket->setTimestampUs(m_lastVideoFrameTimestampUs);
						eventMetadataPacket->setTimestampUs(m_lastVideoFrameTimestampUs);

						// Zero duration means that the event is not sustained, but momental.
						eventMetadataPacket->setDurationUs(0);

						// EventMetadata contains an information about event.
						const auto eventMetadata = makePtr<EventMetadata>();
						// Set all required fields.

						eventMetadata->setTypeId(kNewFaceDetectionEventType);

						eventMetadata->setIsActive(true);
						eventMetadata->setCaption("Alert");

						std::string idCam = camera->host();
						if (camera->StatusCode() == "200")
						{
							eventMetadata->setDescription("Dang nhap thanh cong!");
						}
						else
						{
							eventMetadata->setDescription("Sai tai khoan hoac mat khau!");
						}
						eventMetadataPacket->addItem(eventMetadata.get());

						// Generate index and track id for the next track.
						++m_trackIndex;
						m_trackId = nx::sdk::UuidHelper::randomUuid();

						return eventMetadataPacket;
					}

					Ptr<IMetadataPacket> DeviceAgent::generateObjectMetadataPacket()
					{
						LOG("PUSH OBJECT TO NX CLIENT");
						// ObjectMetadataPacket contains arbitrary number of ObjectMetadata.
						const auto objectMetadataPacket = makePtr<ObjectMetadataPacket>();

						// Bind the object metadata to the last video frame using a timestamp.

						objectMetadataPacket->setTimestampUs(m_lastVideoFrameTimestampUs);
						objectMetadataPacket->setDurationUs(0);
						// ObjectMetadata contains information about an object on the frame.
						const auto objectMetadata = makePtr<ObjectMetadata>();
						// Set all required fields.
						if (camera->isRecognized())
						{
							objectMetadata->setTypeId(kNewFaceDetectionObjectType);
							std::shared_ptr<FaceRecognitionObj> faceRecognitionObject_ptr(new FaceRecognitionObj(camera->recognitionDataInfo()));
							objectMetadata->setTrackId(m_trackId);
							objectMetadata->addAttributes(faceRecognitionObject_ptr->attributes());
							camera->setIsRecognized(false);
						}
						else
						{
							objectMetadata->setTypeId(kNewFaceDetectionObjectType);

							std::shared_ptr<FaceDetectionObj> faceDetectionObject_ptr(new FaceDetectionObj(camera->faceDataInfo()));

							objectMetadata->setTrackId(m_trackId);

							objectMetadata->addAttributes(faceDetectionObject_ptr->attributes());

						}
						objectMetadataPacket->addItem(objectMetadata.get());
						return objectMetadataPacket;
					}

					Ptr<IObjectTrackBestShotPacket> DeviceAgent::generateImageBestShot(Uuid trackId)
					{
						LOG("PUSH IMAGE TO NX CLIENT");
						auto bestShotPacket = makePtr<ObjectTrackBestShotPacket>(trackId, m_lastVideoFrameTimestampUs);
						bestShotPacket->setImageDataFormat(imageFormatFromPath(camera->imgpath()));
						bestShotPacket->setImageData(loadFile(camera->imgpath()));
						return bestShotPacket;
					}

					std::vector<IMetadataPacket *> DeviceAgent::generateMetdataWithBestShot()
					{
						clock_t start = clock();
						auto bestShot = generateImageBestShot(m_trackId).releasePtr();

						auto objectMetadata = generateObjectMetadataPacket().releasePtr();

						m_trackId = nx::sdk::UuidHelper::randomUuid();

						clock_t end = clock();
						double duration = (static_cast<double>(end - start) / CLOCKS_PER_SEC) * 1000.0;

						return {objectMetadata, bestShot};
					}

					// login
					//  std::stringstream ss_this;
					//  ss_this << camera;
					//  std::string Address_this = ss_this.str();

					// json11::Json my_json = json11::Json::object
					// {
					// 	{"id_camera", checkCamInOut(camera->deviceId())},
					// 	{"address", Address_this}
					// };

					// std::string body = my_json.dump();

					// workerthread = new WorkerThread();
					// workerthread->CreateThread();
					// std::shared_ptr<UserData> sendData(new UserData());
					// sendData->msg = body;
					// workerthread->PostMsg(sendData);
					// workerthread->ExitThread();

					// if (camera->StatusCode() == "200") //status cua call API device_Info
					// {
					// 	LOG("TRUE");

					// 	if (camera->login() && camera->subscribe())
					// 	{
					// 		LOG("LOGIN THANH CONG");
					// 		settingsResponse->setModel(kLoginSuccessSettingsModel);
					// 	}
					// 	else
					// 	{
					// 		LOG("LOGIN KHONG THANH CONG");
					// 		settingsResponse->setModel(kLoginSettingsModel);
					// 	}

					// 	pushManifest(manifestString());
					// 	return settingsResponse;
					// }
					// else
					// {
					// LOG("FALSE");

				} // namespace sample
			}
		} // namespace analytics
	}	  // namespace vms_server_plugins
} // namespace nx
