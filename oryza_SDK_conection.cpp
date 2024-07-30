#include "oryza_SDK_connection.h"
#include "oryza_camera.h"
#include "oryza_event_controller.h"
#include "WorkerThread.h"
#include <chrono>

#define PORT_HOST "2208"

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {
					OryzaSDKConnection::OryzaSDKConnection() {
					
					socketThread = new WorkerThread();
						//workerthread(new WorkerThread());
					}

					OryzaSDKConnection::~OryzaSDKConnection() {
						CLIENT_Cleanup();
						
					}

					void OryzaSDKConnection::login(LDWORD dwUser = NULL) 
					{
						clock_t start = clock();
						OryzaCamera* oryza_cam = reinterpret_cast<OryzaCamera*>(dwUser);

						std::stringstream ss_this;
						ss_this << oryza_cam;
						std::string Address_this = ss_this.str();
						LOG("ID thread device Agent: -- " + Address_this);
						std::string cmd = "";
						std::string ip_host = ""; //ip host bn?
						LOG("IPV4");
						try
						{
							boost::asio::io_service io_service;

							std::string endpoint_ip_str = "192.168.0.1";

							auto endpoint_ip = inet_network(endpoint_ip_str.c_str());
							auto endpoint_port = 8000;
							auto host_port = 8001;

							const boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address_v4(endpoint_ip), endpoint_port);
							boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), host_port));
							
							socket.connect(endpoint);
							ip_host = socket.local_endpoint().address().to_string();
							LOG(socket.local_endpoint().address().to_string());
						}
						catch (std::exception& e)
						{
						}
						std::string idCam = oryza_cam->deviceId();
						idCam.erase(std::remove(idCam.begin(), idCam.end(), '{'), idCam.end());
    					idCam.erase(std::remove(idCam.begin(), idCam.end(), '}'), idCam.end());
						std::string rtsp = "rtsp://" + oryza_cam->ipClient() + ":7001/" + idCam;
						
						if (oryza_cam->url().find("rtsp") != std::string::npos) 
						{
							json11::Json my_json = json11::Json::object{ 
							{ "username", oryza_cam->username() },
							{ "password",  oryza_cam->password() },
							{ "status", true },
							{ "address", Address_this},
							{ "ip_host", oryza_cam->ipClient()},
							{ "port_host", PORT_HOST},
							{ "rtsp", rtsp},
							{ "boundary", oryza_cam->Points()},
							{ "min_human_count", 100},
							{ "min_neighbours", oryza_cam->Mincnt()},
							{ "min_dense_clusters", 1},
							{ "waiting_time_to_start_alarm", std::stoi(oryza_cam->timeAlarm())},
							{ "waiting_time_for_next_alarm", 30},
							};
							cmd = my_json.dump();
							LOG(cmd);
						}
						else
						{
							json11::Json my_json = json11::Json::object{ 
							{ "username", oryza_cam->username() },
							{ "password",  oryza_cam->password() },
							{ "status", true },
							{ "address", Address_this},
							{ "ip_host", oryza_cam->ipClient()},
							{ "port_host", PORT_HOST},
							{ "rtsp", rtsp},
							{ "boundary", oryza_cam->Points()},
							{ "min_human_count", 100},
							{ "min_neighbours", oryza_cam->Mincnt()},
							{ "min_dense_clusters", 1},
							{ "waiting_time_to_start_alarm", std::stoi(oryza_cam->timeAlarm())},
							{ "waiting_time_for_next_alarm", 30},
							};
							cmd = my_json.dump();
							LOG(cmd);		
						}
						// ADDR(Address_this);
						// Pust RTSP to Websocket
						workerthread = new WorkerThread();

						workerthread->CreateThread();
						std::shared_ptr<UserData> sendData(new UserData());
						sendData->msg = cmd;
						
						workerthread->PostMsg_2(sendData);
						workerthread->ExitThread();

						delete workerthread;
						
						clock_t end = clock();
						double duration = (static_cast<double>(end - start) / CLOCKS_PER_SEC) * 1000.0;
					}

					void OryzaSDKConnection::logout(LDWORD dwUser = NULL) {
						m_lLoginID = 0;
					}

					void OryzaSDKConnection::subscribe(LDWORD dwUser = NULL) 
					{
						if (!m_lLoginID) return;
						socketThread->CreateThread();
						socketThread->CreateSocket();
						if (socketThread->socketExiss())
						{
							LOG("SOCKET IS LIVE - CLOSE THREAD");
							//socketThread->ExitThread();
							delete socketThread;
						}
					}

					void OryzaSDKConnection::unsubscribe(LDWORD dwUser = NULL) {
						OryzaCamera* oryza_cam_close = reinterpret_cast<OryzaCamera*>(dwUser);
						
						LOG(oryza_cam_close->host());
						LOG(oryza_cam_close->port());
						LOG(oryza_cam_close->username());
						LOG(oryza_cam_close->password());
						LOG(oryza_cam_close->deviceId());
						std::stringstream ss_this;
						ss_this << oryza_cam_close;
						std::string Address_this = ss_this.str();
						std::string cmd_close;

						std::string ip_host = "";
						try
						{
							boost::asio::io_service io_service;

							std::string endpoint_ip_str = "192.168.0.1";

							auto endpoint_ip = inet_network(endpoint_ip_str.c_str());
							auto endpoint_port = 8000;
							auto host_port = 8001;

							const boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address_v4(endpoint_ip), endpoint_port);
							boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), host_port));
							
							socket.connect(endpoint);
							ip_host = socket.local_endpoint().address().to_string();
						}
						catch (std::exception& e)
						{
						}

						std::string idCam = oryza_cam_close->deviceId();
						idCam.erase(std::remove(idCam.begin(), idCam.end(), '{'), idCam.end());
    					idCam.erase(std::remove(idCam.begin(), idCam.end(), '}'), idCam.end());
						std::string rtsp = "rtsp://" + oryza_cam_close->ipClient() + ":7001/" + idCam;

						if (oryza_cam_close->url().find("rtsp") != std::string::npos) 
						{
							json11::Json my_json_close = json11::Json::object{
							{ "ip", oryza_cam_close->host()},
							{ "port", oryza_cam_close->port() },
							{ "username", oryza_cam_close->username() },
							{ "password",  oryza_cam_close->password() },
							{ "status", false },
							{ "id_camera", oryza_cam_close->deviceId()},
							{ "ai_module", 1 },
							{ "address", Address_this},
							{ "ip_host", oryza_cam_close->ipClient()},
							{ "port_host", PORT_HOST},
							{ "rtsp", rtsp},
							{ "boundary", oryza_cam_close->Points()},
							{ "min_human_count", 100},
							{ "min_neighbours", oryza_cam_close->Mincnt()},
							{ "min_dense_clusters", 1},
							{ "waiting_time_to_start_alarm", std::stoi(oryza_cam_close->timeAlarm())},
							{ "waiting_time_for_next_alarm", 5},
								};
							cmd_close = my_json_close.dump();
							LOG(cmd_close);
						}
						else
						{
							if (oryza_cam_close->port() == "")
							{
								json11::Json my_json_close = json11::Json::object{
								{ "ip", oryza_cam_close->host()},
								{ "username", oryza_cam_close->username() },
								{ "password",  oryza_cam_close->password() },
								{ "status", false},
								{ "id_camera", oryza_cam_close->deviceId()},
								{ "ai_module", 1 },
								{ "address", Address_this},
								{ "ip_host", oryza_cam_close->ipClient()},
								{ "port_host", PORT_HOST},
								{ "rtsp", rtsp},
								{ "boundary", oryza_cam_close->Points()},
								{ "min_human_count", 100},
								{ "min_neighbours", oryza_cam_close->Mincnt()},
								{ "min_dense_clusters", 1},
								{ "waiting_time_to_start_alarm", std::stoi(oryza_cam_close->timeAlarm())},
								{ "waiting_time_for_next_alarm", 5},
								};
								cmd_close = my_json_close.dump();
								LOG(cmd_close);
							}
							else
							{
								json11::Json my_json_close = json11::Json::object{
								{ "ip", oryza_cam_close->host()},
								{ "port", oryza_cam_close->port() },
								{ "username", oryza_cam_close->username() },
								{ "password",  oryza_cam_close->password() },
								{ "status", false },
								{ "id_camera", oryza_cam_close->deviceId()},
								{ "ai_module", 1 },
								{ "address", Address_this},
								{ "ip_host", oryza_cam_close->ipClient()},
								{ "port_host", PORT_HOST},
								{ "rtsp", rtsp},
								{ "boundary", oryza_cam_close->Points()},
								{ "min_human_count", 100},
								{ "min_neighbours", oryza_cam_close->Mincnt()},
								{ "min_dense_clusters", 1},
								{ "waiting_time_to_start_alarm", std::stoi(oryza_cam_close->timeAlarm())},
								{ "waiting_time_for_next_alarm", 5},
								};
								cmd_close = my_json_close.dump();
								LOG(cmd_close);
							}
						}
						workerthread = new WorkerThread();

						workerthread->CreateThread();
						std::shared_ptr<UserData> sendData_close(new UserData());
						sendData_close->msg = cmd_close;
						workerthread->PostMsg_2(sendData_close);
						workerthread->ExitThread();

						m_lRealPicHandle = 0;
						
						delete workerthread;
						//delete socketThread;

					}

					bool OryzaSDKConnection::loginSuccess(LDWORD dwUser = NULL)
					{
						OryzaCamera* oryza_cam = reinterpret_cast<OryzaCamera*>(dwUser);

						if (oryza_cam->StatusCode() == "200")
						{
							LOG("SUCCESS TO CONNECT");
							m_lLoginID = 1;
						}
						else
						{
							LOG("FAIL TO CONNECT");
							m_lLoginID = 0;
						}
						return m_lLoginID != 0;
					}

					bool OryzaSDKConnection::subscribeSuccess()
					{
						return m_lRealPicHandle != 0;
					}
				};

			} // namespace sample
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx