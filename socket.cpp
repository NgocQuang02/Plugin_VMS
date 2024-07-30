#include "socket.h"
#include "oryza_camera.h"
#include "../../Rece_data_socket.h"
//#include "WorkerThread.h"
#ifdef _WIN32
#else
#include <future>
#endif

#define PORT_SOCKET 2208

namespace nx {
    namespace vms_server_plugins {
        namespace analytics {
            namespace stub {
                namespace CrowdDetection {
                    #define UNKNOWN "Unknown"
                   // using nx::kit::Json;

                    //----------------------------------------------------------------------------
                   // Get message from socket
                   //----------------------------------------------------------------------------
                    #ifdef _WIN64
					void Socket::GetMsgSocket(SOCKET client)
                    {
                        c_mutex.lock();
                        LOG("GET MSG START");
                        std::string dataSocket = "";

                        int iResult;
                        char buffer[1000000];

                        LOG("RECEIVE DATA");
                        iResult = recv(client, buffer, 1000000, 0);
                        LOG(std::to_string(sizeof(buffer)));
                        LOG(std::to_string(iResult));
                        if (iResult > 10)
                        {
                            for (int i = 0; i < iResult; i++)
                            {
                                dataSocket += buffer[i];
                            }
                            memset(buffer, 0, sizeof(buffer));
                            
                            std::string err;
                            const auto msgJson = json11::Json::parse(dataSocket, err);
                            DataSocket sendData;
                            std::string user = "";
                            std::string ai_module = "";

                            user = msgJson["address"].string_value();
                            ai_module = msgJson["ai_module"].string_value();

                            sendData.dataSocket(std::stoi(ai_module), dataSocket, user);
                        }
						closesocket(client);  
                        c_mutex.unlock();
                        LOG("Receive data success!!!!!!!");
                    }
					#else
					void Socket::GetMsgSocket(int new_socket)
					{
                        clock_t start = clock();

                        std::ostringstream oss;
                        oss << std::this_thread::get_id();  
                        std::string threadAddress = oss.str();

                        std::string dataSocket = "";

                        int iResult = 0;
                        int result = 0;
                        char buffer[1000000] = {0};

                        

                        while (1)
                        {
                            iResult = read(new_socket, buffer, 1000000);
                            for (int i = 0; i < iResult; i++)
                            {
                                dataSocket += buffer[i];
                            }
                            memset(buffer, 0, sizeof(buffer));
                            if (iResult == 0) break; 
                            result += iResult;                          
                        }

                        LOG("RECEIVE DATA SUCCESS");

                        if (result > 10)
                        {
                           
                            std::string err;
                            const auto msgJson = json11::Json::parse(dataSocket, err);
                            DataSocket sendData;
                            std::string user = "";
                            int ai_module = 0;

                            user = msgJson["address"].string_value();
                            ai_module = msgJson["ai_module"].int_value();
                            sendData.dataSocket(1, dataSocket, user);
                        }
                        
						close(new_socket);
                        
                        clock_t end = clock();
                        double duration = (static_cast<double>(end - start) / CLOCKS_PER_SEC) * 1000.0;
                    };

                    // void Socket::ReceiveData(int new_socket) 
                    // {
                    //     c_mutex.lock();
                    //     LOG("RECEIVE DATA");
                        
                    //     int iResult = 0;
                    //     char buffer[1000000] = {0};
                    //     dataSocket = "";

                    //     while (1)
                    //     {                            
                    //         iResult = read(new_socket, buffer, 1000000);
                    //         LOG(std::to_string(iResult));
                    //         for (int i = 0; i < iResult; i++)
                    //         {
                    //             dataSocket += buffer[i];
                    //         }
                    //         memset(buffer, 0, sizeof(buffer));
                    //         if (iResult == 0) break; 
                    //         result += iResult;                          
                    //     }
                        
                    //     // c_mutex_socket.lock();
                    //     // dataQueue.push(dataSocket);
                    //     // c_mutex_socket.unlock();

                    //     enqueueData(dataSocket);

                    //     LOG(std::to_string(sizeof(buffer)));
                    //     LOG("RECEIVE DATA SUCCESS");

                    //     c_mutex.unlock();
                    //     ProcessData(new_socket);
                    // };

                    // void Socket::enqueueData(const std::string& dataSocket)
                    // {
                    //     std::lock_guard<std::mutex> lock(c_mutex);
                    //     dataQueue.push(dataSocket);
                    // }
                
                    // void Socket::ProcessData(int new_socket)
                    // {
                    //     c_mutex.lock();
                    //     // std::lock_guard<std::mutex> lock(c_mutex);
                    //     LOG("GET MSG START");

                    //     // if (!dataQueue.empty()) {
                    //     //     std::string data = dataQueue.front();
                    //     //     dataQueue.pop(); 

                    //     while (!dataQueue.empty()) 
                    //     {
                    //         dataSocket = dataQueue.front();
                    //         dataQueue.pop();

                    //         if (result > 10)
                    //         {                            
                    //             std::string err;
                    //             const auto msgJson = json11::Json::parse(dataSocket, err);
                    //             DataSocket sendData;
                    //             std::string user = "";
                    //             int ai_module = 0;

                    //             user = msgJson["address"].string_value();
                    //             ai_module = msgJson["ai_module"].int_value();
                    //             LOG(std::to_string(ai_module));
                    //             LOG(user);
                    //             LOG(dataSocket);
                    //             sendData.dataSocket(1, dataSocket, user);
                    //         }
                    //     }
                    //     close(new_socket);
                    //     c_mutex.unlock();
                    //     LOG("Receive data success!!!!!!!");
                    // };


                    #endif
                    
                    //----------------------------------------------------------------------------
                    // Create Socket
                    //----------------------------------------------------------------------------
                    void Socket::createSocket()
                    {
                        std::ostringstream oss;
                        oss << std::this_thread::get_id();  
                        std::string threadAddress = oss.str();


                        #ifdef _WIN32
                        WSADATA wsa_data;
                        SOCKADDR_IN server_addr, client_addr;
                        WSAStartup(MAKEWORD(2, 2), &wsa_data);


                        SOCKET server;ProcessData(dataSocket);
                        int client_addr_size;
                        server = socket(AF_INET, SOCK_STREAM, 0);

                        server_addr.sin_addr.s_addr = INADDR_ANY;
                        server_addr.sin_family = AF_INET;
                        server_addr.sin_port = htons(PORT_SOCKET);

                        ::bind(server, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr));
                        listen(server, 0);


                        client_addr_size = sizeof(client_addr);
                        Logger::instance()->writeLogServer(__func__, "Listening for incoming connections...");

                        while (1)ProcessData(dataSocket);
                        {
                            SOCKET client;

                            if ((client = accept(server, reinterpret_cast<SOCKADDR*>(&client_addr), &client_addr_size)) != INVALID_SOCKET)
                            {
                                c_mutex_socket.lock();
                                LOG("HAVE DATA TO RECEIVE");_
                                    
                                //auto fut = async(launch::async, &WorkerThread::GetMsgSocket, this, client, idcam);
                                std::thread thread_rece;
                                thread_rece = std::thread(&Socket::GetMsgSocket, this, client);

                                thread_rece.join();
                                c_mutex_socket.unlock();
                                LOG("LISTEN SUCCESS!!!!!!");
                            }

                            const auto last_error = WSAGetLastError();

                            if (last_error > 0)
                            {
                            }
                        }
                        #else
                        int server_fd, new_socket, valread;
                        struct sockaddr_in address;
                        int opt = 1;
                        int addrlen = sizeof(address);
                        char buffer[1000000] = { 0 };

                        // Creating socket file descriptor
                        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                            exit(EXIT_FAILURE);
                        }

                        // Forcefully attaching socket to the port 8080
                        if (setsockopt(server_fd, SOL_SOCKET,
                                    SO_REUSEADDR | SO_REUSEPORT, &opt,
                                    sizeof(opt))) {
                            exit(EXIT_FAILURE);
                        }
                        address.sin_family = AF_INET;
                        address.sin_addr.s_addr = INADDR_ANY;
                        address.sin_port = htons(PORT_SOCKET);


                        // Forcefully attaching socket to the port 8080
                        if (bind(server_fd, (struct sockaddr*)&address,
                                sizeof(address))
                            < 0) {

                            exit(EXIT_FAILURE);
                        }
                        if (listen(server_fd, 3) < 0) {
                            exit(EXIT_FAILURE);
                        }

                        LOG("LISTENING...");
                        while(1)
                        {
                            if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) >= 0) 
                            {
                                LOG("HAVE DATA TO RECEIVE");

                                // Lấy thời gian hiện tại (real time) tại điểm đó trong hàm
                                auto currentTime = std::chrono::system_clock::now();

                                auto fut = std::async(std::launch::async, &Socket::GetMsgSocket, this, new_socket);  

                                fut.wait();

                                // Chuyển thời gian hiện tại thành milliseconds
                                auto currentTimeMillis = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime);

                                // Lấy giá trị milliseconds
                                auto timeSinceEpoch = currentTimeMillis.time_since_epoch();
                                long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceEpoch).count();
                                
                            }
                            
                        }
						#endif
                    };
                }
            }
        }
    }
}