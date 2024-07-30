#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <sstream>
#include <future>
#include <fstream>

#include "../../utils.h" 
#include "../../Logger.h"
#include "Websocket.h"
//#include <cpr/cpr.h>
#include "socket.h"

namespace nx {
    namespace vms_server_plugins {
        namespace analytics {
            namespace stub {
                namespace CrowdDetection {
                    class OryzaCamera;

                    struct UserData
                    {
                        std::string msg;
                        std::string address;
                    };

                    struct ThreadMsg;
                    
                    class WorkerThread
                    {
                    public:
                        WorkerThread();

                        ~WorkerThread();

                        /// Create the worker thread. Return True when create success
                        bool CreateThread();

                        /// Exit the worker thread
                        void ExitThread();

                        /// Add a message to the thread queue
                        void PostMsg(std::shared_ptr<UserData> msg);

                        void PostMsg_2(std::shared_ptr<UserData> msg);

                        /// Create a socket 
                        void CreateSocket();

                        /// Entry point for timer thread
                        void TimerThread();

                        // call http
                        void callHttp(std::shared_ptr<UserData> data);

                        void setSocketExiss(bool socketExiss);
                        bool socketExiss();
                        
                        bool isSocketServerAlive(int serverPort);
                    private:
                        /* WorkerThread(const WorkerThread&) = delete;
                            WorkerThread& operator=(const WorkerThread&) = delete;*/

                        // Entry point for the worker thread
                        void Process();

                        std::unique_ptr<std::thread> m_thread;
                        std::queue<std::shared_ptr<ThreadMsg>> m_queue;
                        std::mutex m_mutex;
                        std::condition_variable m_cv;
                        std::atomic<bool> m_timerExit;
                        std::mutex c_mutex;
                        std::mutex c_mutexMsg;
                        bool m_socketExiss = false;

                        std::shared_ptr<Socket> p_socket;

                    };
                }
            }
        }
    }
}
