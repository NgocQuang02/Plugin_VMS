#include "WorkerThread.h"
#include <ctime>
#include <chrono>
#include "oryza_camera.h"
#include <boost/asio.hpp>

#define MSG_EXIT_THREAD 1
#define MSG_POST_USER_DATA 2
#define MSG_TIMER 3
#define MSG_POST_USER_DATA_2 4
#define MSG_CREATE_SOCKET 5
#define MSG_CALL_HTTP 6

#define IP_TEST "192.168.111.50"
// #define PORT_TEST "8082"
#define PORT_TEST "8080"

#define IP_HTTP "192.168.111.53"
#define PORT_HTTP "8000"
#define PORT_SOCKET 2208
#define IP_HTTP_RTSP "192.168.111.60"
#define PORT_HTTP_RTSP "8000"

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
					// using json = nlohmann::json;
					int count_data = 0;

					struct ThreadMsg
					{
						ThreadMsg(int i, std::shared_ptr<void> m)
						{
							id = i;
							msg = m;
						}
						int id;
						std::shared_ptr<void> msg;
					};

					//----------------------------------------------------------------------------
					// WorkerThread
					//----------------------------------------------------------------------------
					WorkerThread::WorkerThread() : m_thread(nullptr), m_timerExit(false)
					{
					}

					//----------------------------------------------------------------------------
					// ~WorkerThread
					//----------------------------------------------------------------------------
					WorkerThread::~WorkerThread()
					{
						ExitThread();
						m_thread = nullptr;
					}

					//----------------------------------------------------------------------------
					// CreateThread
					//----------------------------------------------------------------------------
					bool WorkerThread::CreateThread()
					{
						if (!m_thread)
						{
							m_thread = std::unique_ptr<std::thread>(new std::thread(&WorkerThread::Process, this));
						}
						return true;
					}

					//----------------------------------------------------------------------------
					// ExitThread
					//----------------------------------------------------------------------------
					void WorkerThread::ExitThread()
					{
						if (!m_thread)
							return;

						// Create a new ThreadMsg
						std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(MSG_EXIT_THREAD, 0));

						// Put exit thread message into the queue
						{
							std::lock_guard<std::mutex> lock(m_mutex);
							m_queue.push(threadMsg);
							m_cv.notify_one();
						}

						m_thread->join();
						m_thread = nullptr;
					}
					//----------------------------------------------------------------------------
					// Close Thread
					//----------------------------------------------------------------------------
					void WorkerThread::setSocketExiss(bool socketExiss)
					{
						m_socketExiss = socketExiss;
					}

					bool WorkerThread::socketExiss()
					{
						return m_socketExiss;
					}

					//----------------------------------------------------------------------------
					// PostMsg
					//----------------------------------------------------------------------------
					void WorkerThread::PostMsg(std::shared_ptr<UserData> data)
					{
						if (m_thread)
							;

						// Create a new ThreadMsg
						std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(MSG_POST_USER_DATA, data));

						// Add user data msg to queue and notify worker thread
						std::unique_lock<std::mutex> lk(m_mutex);
						m_queue.push(threadMsg);
						m_cv.notify_one();
					}

					void WorkerThread::PostMsg_2(std::shared_ptr<UserData> data)
					{
						if (m_thread)
							;
						// Create a new ThreadMsg
						std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(MSG_POST_USER_DATA_2, data));

						// Add user data msg to queue and notify worker thread
						std::unique_lock<std::mutex> lk(m_mutex);
						m_queue.push(threadMsg);
						m_cv.notify_one();
					}

					//----------------------------------------------------------------------------
					// call Http
					//----------------------------------------------------------------------------
					void WorkerThread::callHttp(std::shared_ptr<UserData> data)
					{
						if (m_thread)
							;

						// Create a new ThreadMsg
						std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(MSG_CALL_HTTP, data));

						// Add user data msg to queue and notify worker thread
						std::unique_lock<std::mutex> lk(m_mutex);
						m_queue.push(threadMsg);
						m_cv.notify_one();
					}

					//----------------------------------------------------------------------------
					// CreateSocket
					//----------------------------------------------------------------------------
					void WorkerThread::CreateSocket()
					{
						if (!m_thread)
							return;
						// Create a new ThreadMsg
						std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(MSG_CREATE_SOCKET, 0));

						// Put create socket thread
						{
							std::lock_guard<std::mutex> lock(m_mutex);
							m_queue.push(threadMsg);
							m_cv.notify_one();
						}
					}

					//----------------------------------------------------------------------------
					// TimerThread
					//----------------------------------------------------------------------------
					void WorkerThread::TimerThread()
					{
						while (!m_timerExit)
						{
							// Sleep for 250mS then put a MSG_TIMER into the message queue
							std::this_thread::sleep_for(std::chrono::milliseconds(100));

							std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(MSG_TIMER, 0));

							// Add timer msg to queue and notify worker thread
							std::unique_lock<std::mutex> lk(m_mutex);
							m_queue.push(threadMsg);
							m_cv.notify_one();
						}
					}

					//----------------------------------------------------------------------------
					// Check socket live
					//----------------------------------------------------------------------------
					bool WorkerThread::isSocketServerAlive(int serverPort)
					{
#ifdef _WIN32
						WSADATA wsaData;
						if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
						{
							std::cerr << "WSAStartup failed." << std::endl;
							return false;
						}

						SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
						if (clientSocket == INVALID_SOCKET)
						{
							WSACleanup();
							return false;
						}
						LOG("check socket live");
						SOCKADDR_IN serverAddr;

						std::string address;

						boost::asio::io_service io_service;
						boost::asio::ip::tcp::resolver resolver(io_service);
						boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");

						boost::system::error_code ec;
						boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query, ec);

						if (!ec)
						{
							while (it != boost::asio::ip::tcp::resolver::iterator())
							{
								boost::asio::ip::address addr = (it++)->endpoint().address();
								if (addr.is_v4())
								{
									address = addr.to_string();
								}
							}
						}

						serverAddr.sin_family = AF_INET;
						serverAddr.sin_port = htons(serverPort);
						inet_pton(AF_INET, address.c_str(), &(serverAddr.sin_addr));

						if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) == 0)
						{
							// Connection successful
							closesocket(clientSocket);
							WSACleanup();
							return true;
						}
						else
						{
							// Connection failed
							closesocket(clientSocket);
							WSACleanup();
							return false;
						}
#else
						int server_fd, new_socket, valread;
						struct sockaddr_in address;
						int opt = 1;
						int addrlen = sizeof(address);
						char buffer[1000000] = {0};

						// Creating socket file descriptor
						if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
						{
							exit(EXIT_FAILURE);
						}

						address.sin_family = AF_INET;
						address.sin_addr.s_addr = INADDR_ANY;
						address.sin_port = htons(serverPort);

						int result = connect(server_fd, (struct sockaddr *)&address, sizeof(address));
						if (result == 0)
						{
							close(server_fd);
							return true;
						}
						else
						{
							close(server_fd);
							return false;
						}
#endif
					}

					//----------------------------------------------------------------------------
					// Process
					//----------------------------------------------------------------------------
					void WorkerThread::Process()
					{
						m_timerExit = false;
						std::thread timerThread(&WorkerThread::TimerThread, this);

						while (1)
						{
							std::shared_ptr<ThreadMsg> msg;
							{
								// Wait for a message to be added to the queue
								std::unique_lock<std::mutex> lk(m_mutex);
								while (m_queue.empty())
									m_cv.wait(lk);

								if (m_queue.empty())
									continue;

								msg = m_queue.front();
								m_queue.pop();
							}

							switch (msg->id)
							{
							case MSG_CREATE_SOCKET:
							{
								if (isSocketServerAlive(PORT_SOCKET))
								{
									Logger::instance()->writeLogServer(__func__, "server is live");
									setSocketExiss(true);
								}
								else
								{
									p_socket.reset(new Socket());
									p_socket->createSocket();
								}
								break;
							}

							case MSG_POST_USER_DATA:
							{
								try
								{
									auto userData = std::static_pointer_cast<UserData>(msg->msg);
									std::string err;
									const auto msgJson = json11::Json::parse(userData->msg, err);

									std::string id = "";
									std::string id_cam = "";
									std::string path = "";
									std::string id_camera = "";

									id_camera = msgJson["address"].string_value();
									const char *user_str = id_camera.c_str();
									std::uint64_t address_value = std::strtoull(user_str, nullptr, 16);
									OryzaCamera *oryza_cam = reinterpret_cast<OryzaCamera *>(address_value);

									LOG("IP SERVER BACKEND: " + oryza_cam->ipServer());

									boost::asio::io_service io_service;
									// Resolve the server address and port
									tcp::resolver resolver(io_service);
									// tcp::resolver::query query(IP_HTTP_RTSP, PORT_HTTP_RTSP);
									tcp::resolver::query query(oryza_cam->ipServer(), PORT_TEST);
									tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

									// Establish a connection to the server
									tcp::socket socket(io_service);
									boost::asio::connect(socket, endpoint_iterator);

									// Create the HTTP POST request
									std::string request = "POST /api/get_info_camera HTTP/1.1\r\n"
										"Host: " + oryza_cam->ipServer() + ":8082\r\n"
										"Authorization: Bearer W+wBZ1TcC-RMcQM4ywXg\r\n"
										"Content-Type: application/json\r\n"
										"Content-Length: " +std::to_string(userData->msg.length()) + "\r\n"
										"Connection: close\r\n"
										"\r\n" + userData->msg;

										
									// std::string request = body;

									// Send the POST request
									boost::asio::write(socket, boost::asio::buffer(request));

									// Read and print the response from the server
									boost::asio::streambuf response;
									boost::asio::read_until(socket, response, "\r\n");

									// Convert response streambuf to string
									std::string response_str;
									std::istream response_stream(&response);
									while (!response_stream.eof())
									{
										char buffer[1000000];
										response_stream.read(buffer, sizeof(buffer));
										response_str.append(buffer, response_stream.gcount());
										memset(buffer, 0, sizeof(buffer));
									}

									LOG(response_str);

									// lay status code = 200 de call login API
									size_t status_code_pos = response_str.find("HTTP/1.1");
									std::string status_code = "";
									if (status_code_pos != std::string::npos)
									{
										size_t code_start = status_code_pos + 9;
										size_t code_end = response_str.find(' ', code_start);
										if (code_end != std::string::npos)
										{
											status_code = response_str.substr(code_start, code_end - code_start);
										}
									}
									oryza_cam->setStatusCode(status_code);

									// lay username, password tu responese cua HTTP
									size_t json_start = response_str.find("{");

									std::string json_data = response_str.substr(json_start);

									json11::Json data = json11::Json::parse(json_data, err);
									std::string username = data["username"].string_value();
									std::string password = data["password"].string_value();

									oryza_cam->setUsername(username);
									oryza_cam->setPassword(password);
								}

								catch (std::exception &e)
								{
									LOG("Exception: " + std::string(e.what()) + "\n");
								}
								break;
							}

							case MSG_POST_USER_DATA_2:
							{
								try
								{
									std::string err;
									auto userData = std::static_pointer_cast<UserData>(msg->msg);
									const auto msgJson = json11::Json::parse(userData->msg, err);

									std::string id = "";
									std::string id_cam = "";
									std::string path = "";
									std::string id_camera = "";

									id_camera = msgJson["address"].string_value();
									const char *user_str = id_camera.c_str();
									std::uint64_t address_value = std::strtoull(user_str, nullptr, 16);
									OryzaCamera *oryza_cam = reinterpret_cast<OryzaCamera *>(address_value);

									LOG("IP SERVER BACKEND: " + oryza_cam->ipServer());

									boost::asio::io_service io_service;
									// Resolve the server address and port
									tcp::resolver resolver(io_service);
									// tcp::resolver::query query(IP_HTTP_RTSP, PORT_HTTP_RTSP);
									tcp::resolver::query query(oryza_cam->ipServer(), PORT_TEST);
									tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

									// Establish a connection to the server
									tcp::socket socket(io_service);
									boost::asio::connect(socket, endpoint_iterator);

									std::string request = "POST /process_stream HTTP/1.1\r\n"
									//std::string request = "POST / HTTP/1.0\r\n"
										// "Host: " + oryza_cam->ipServer() + ":8082\r\n"
										"Host: " + oryza_cam->ipServer() + ":8080\r\n"
										"Authorization: Bearer W+wBZ1TcC-RMcQM4ywXg\r\n"
										"Content-Type: application/json\r\n"
										"Content-Length: " + std::to_string(userData->msg.length()) + "\r\n"
										"Connection: close\r\n"
										"\r\n" + userData->msg;


									// std::string request = "POST / HTTP/1.1\r\n"
									// "Host: " + oryza_cam->ipServer() + ":8000\r\n"
									// //"Authorization: Bearer W+wBZ1TcC-RMcQM4ywXg\r\n"
									//  "Content-Type: application/json\r\n"
									//  "Content-Length: " +std::to_string(4) + "\r\n"
									//  "Connection: close\r\n"
									//  "\r\n khoi";



									// std::string request = body;

									// Send the POST request
									boost::asio::write(socket, boost::asio::buffer(request));

									// Read and print the response from the server
									boost::asio::streambuf response;
									boost::asio::read_until(socket, response, "\r\n");
									// Convert response streambuf to string
									std::string response_str;
									std::istream response_stream(&response);
									while (!response_stream.eof())
									{
										char buffer[1000000];
										response_stream.read(buffer, sizeof(buffer));
										response_str.append(buffer, response_stream.gcount());
										memset(buffer, 0, sizeof(buffer));
									}

									LOG(response_str);

									// hien thi thong bao dang nhap thanh cong len Event

									// size_t status_code_pos = response_str.find("HTTP/1.0");
									// std::string status_code = "";
									// if (status_code_pos != std::string::npos)
									// {
									// 	size_t code_start = status_code_pos + 9;
									// 	size_t code_end = response_str.find(' ', code_start);
									// 	if (code_end != std::string::npos)
									// 	{
									// 		status_code = response_str.substr(code_start, code_end - code_start);
									// 	}
									// }


									if (response_str.find("200") != std::string::npos)
									{
										//oryza_cam->setCallBackEvent(TRUE);
										oryza_cam->setStatusCode("200");
									}
									else if (response_str.find("Could not open the RTSP stream") != std::string::npos)
									{
										oryza_cam->setStatusCode("0");
									}
									else
									{
										oryza_cam->setStatusCode("400");
									}
								}

								catch (std::exception &e)
								{
									LOG("Exception: " + std::string(e.what()) + "\n");
								}
								break;
							}

							case MSG_TIMER:
								// Logger::instance()->writeLogServer(__func__, "Timer expired on ");
								break;

							case MSG_EXIT_THREAD:
							{
								m_timerExit = true;
								timerThread.join();
								return;
							}

							case MSG_CALL_HTTP:
							{
								try
								{
									std::string id = "";
									std::string id_cam = "";
									std::string path = "";

									boost::asio::io_service io_service;

									// Resolve the server address and port
									tcp::resolver resolver(io_service);
									tcp::resolver::query query(IP_HTTP, "http");
									tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
									tcp::resolver::iterator end;

									// Establish a connection to the server
									tcp::socket socket(io_service);
									boost::system::error_code error = boost::asio::error::host_not_found;
									while (error && endpoint_iterator != end)
									{
										socket.close();
										socket.connect(*endpoint_iterator++, error);
									}
									// if (!socket.lowest_layer().is_open()) {
									// 	// Tạo kết nối mới nếu cần
									// 	boost::asio::connect(socket.lowest_layer(), endpoint_iterator);
									// }
									// boost::asio::connect(socket, endpoint_iterator);

									// Load the image data
									auto userData = std::static_pointer_cast<UserData>(msg->msg);
									std::string err;
									const auto msgJson = json11::Json::parse(userData->msg, err);

									path = msgJson["file"].string_value();
									id = msgJson["id"].string_value();
									id_cam = msgJson["id_cam"].string_value();

									std::ifstream image_file(path, std::ios::binary | std::ios::ate);
									std::streamsize image_size = image_file.tellg();
									image_file.seekg(0, std::ios::beg);
									char *buff = new char[image_size];
									image_file.read(buff, image_size);
									image_file.close();
									std::string ret(buff, image_size);
									delete[] buff;
									std::vector<char> image_data(image_size);


									// Create the POST request body (multipart/form-data)
									std::string boundary = "------WebKitFormBoundary7MA4YWxkTrZu0gW";
									std::string body = "--" + boundary + "\r\n"
																		 "Content-Disposition: form-data; name=\"id\"\r\n\r\n" +
													   id + "\r\n"
															"--" +
													   boundary + "\r\n"
																  "Content-Disposition: form-data; name=\"id_cam\"\r\n\r\n" +
													   id_cam + "\r\n"
																"--" +
													   boundary + "\r\n"
																  "Content-Disposition: form-data; name=\"file\"; filename=\"image.jpg\"\r\n"
																  "Content-Type: image/jpg\r\n\r\n";
									body += ret + "\r\n";
									body += "--" + boundary + "\r\n";

									// Create the HTTP POST request
									std::string request = "POST /api/v1/face_ai HTTP/1.1\r\n"
														  "Host: 192.168.111.53:8000\r\n"
														  "Content-Type: multipart/form-data; boundary=" +
														  boundary + "\r\n"
																	 "Content-Length: " +
														  std::to_string(body.length()) + "\r\n"
																						  "Connection: close\r\n\r\n" +
														  body;
									// std::string request = body;
									// Send the POST request
									boost::asio::write(socket, boost::asio::buffer(request));

									// Read and print the response from the server
									boost::asio::streambuf response;
									boost::asio::read_until(socket, response, "\r\n");

									// Convert response streambuf to string
									std::string response_str;
									std::istream response_stream(&response);
									while (!response_stream.eof())
									{
										char buffer[1000000];
										response_stream.read(buffer, sizeof(buffer));
										response_str.append(buffer, response_stream.gcount());
										memset(buffer, 0, sizeof(buffer));
									}
									// socket.lowest_layer().close();
									LOG(response_str);
								}
								catch (std::exception &e)
								{
									LOG("Exception: " + std::string(e.what()) + "\n");
								}
							}
							default:
							{
								break;
							}
							}
						}
					}
				}
			}
		}
	}
}
