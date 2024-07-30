#ifdef _WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  
#include <unistd.h> 
#endif
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <ctime>
#include <chrono>
#include <queue>


#include "../../Logger.h"
#include "../../json11.hpp"
#include <future>

#ifdef _WIN32
#else
#define DWORD       unsigned int
#endif
// #ifndef LDWORD
// #ifdef _WIN64
// #define LDWORD  INT64
// #define DWORD       unsigned int
// #else 
// #define DWORD       unsigned int
// #define LDWORD  DWORD
// #endif
// #endif

namespace nx {
    namespace vms_server_plugins {
        namespace analytics {
            namespace stub {
                namespace CrowdDetection {
                    class OryzaCamera;
                    class WorkerThread;
                    class Socket {

                    public:
                      
                    public:
                        void createSocket();

                        /// Receice a message from socket
                        #ifdef _WIN64
                        void GetMsgSocket(SOCKET client);
                        #else
                        void GetMsgSocket(int new_socket);
                        // void ReceiveData(int new_socket);
                        // void ProcessData(int new_socket);
                        // void enqueueData(const std::string& dataSocket);
                        #endif

                    private:
                        std::mutex c_mutex;
                        std::mutex c_mutex_socket;
                        // std::string dataSocket = "";
                        // int result = 0;
                        // std::queue<std::string> dataQueue;       
                    };
                }
            }
        }
    }
}