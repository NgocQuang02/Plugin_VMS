#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include "crowd_detection/i_event_controller.h"
#include <vector>
#include <string>
#include "Logger.h"
#include "json11.hpp"
#include "i_camera.h"
// #include <opencv2/opencv.hpp>

#ifdef _WIN32
#define DWORD       unsigned long
#else
#define DWORD       unsigned int
#endif

// using namespace cv;

namespace nx {
    namespace vms_server_plugins {
        namespace analytics {
            namespace stub {
         

                //  class WorkerThread;
                class DataSocket {

                public:
                    void user_pass_camera(std::string deviceId, std::string status);
                public:
                    void dataSocket(int ai_module, std::string dataSocket, std::string User);
                    // base64 to byte and dword
                    std::vector<unsigned char> base64_decode_to_bytes(const std::string& base64_string);
                    DWORD get_buffer_size(const std::string& base64_string);
                    void captureDoubleImage(BYTE* pBuffer1, DWORD dwBufSize1, LDWORD User, BYTE* pBuffer2, DWORD dwBufSize2);
                    void captureSingleImage(BYTE* pBuffer, DWORD dwBufSize, LDWORD User);
                    void singleImage(const std::string& base64_string, LDWORD User);
                    void doubleImage(const std::string& base64_1, const std::string& base64_2, LDWORD User);
                private:

                };
            }
        }
    }
}