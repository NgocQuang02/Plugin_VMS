#include "Rece_data_socket.h"

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <map>
#include <chrono>
#include <thread>
#include <sstream>

#define UNKNOWN "Unknown"
#define TRAFFIC_PLATE_PLUGIN 0
#define FACE_DETECTION_PLUGIN 1

namespace nx {
    namespace vms_server_plugins {
        namespace analytics {
            namespace stub {
                void DataSocket::dataSocket(int ai_module, std::string dataSocket, std::string User)
                {
                    clock_t start = clock();

                    std::ostringstream oss;
                    oss << std::this_thread::get_id();  
                    std::string threadAddress = oss.str();
                    LOG("Address Thread Receive");
                    LOG(threadAddress);

                    switch (ai_module)
                    {

                    case FACE_DETECTION_PLUGIN:
                    {
                        
                        std::string err;
                        const auto msgJson = json11::Json::parse(dataSocket, err);

                        std::string name = "";
                        std::string id = "";
                        std::string traffic_plate = "";
                        std::string base64_1 = "";
                        std::string user = "";
                        std::string base64_2 = "";
                        std::string verify = "";
                        int ai_module;
                        std::string is_running = "";

                        name = (!msgJson["name"].string_value().empty() ? msgJson["name"].string_value() : UNKNOWN);
                        id = (!msgJson["id"].string_value().empty() ? msgJson["id"].string_value() : UNKNOWN);
                        traffic_plate = (!msgJson["traffic_plate"].string_value().empty() ? msgJson["traffic_plate"].string_value() : UNKNOWN);
                        verify = (!msgJson["verify"].string_value().empty() ? msgJson["verify"].string_value() : UNKNOWN);
                        base64_1 = msgJson["image"].string_value();
                        base64_2 = msgJson["base64_2"].string_value();
                        user = msgJson["address"].string_value();
                        ai_module = msgJson["ai_module"].int_value();
                        is_running = msgJson["is_running"].string_value();
                    
                        if (user != "")
                        {
                            const char* user_str = user.c_str();
                            std::uint64_t address_value = std::strtoull(user_str, nullptr, 16);

                            //CrowdDetection::Oryza::OryzaCamera* oryza_cam = reinterpret_cast<CrowdDetection::Oryza::OryzaCamera*>(address_value);
                            ICamera* oryza_cam = reinterpret_cast<ICamera*>(address_value);

                            std::vector<std::string> data = { name , id , traffic_plate, verify};

                            oryza_cam->setRecognitionDataInfo(data);

                            if (!base64_1.empty() && !base64_2.empty()) 
                            {
                                doubleImage(base64_1, base64_2, address_value);
                            }
                            else if (!base64_1.empty()) 
                            {
                                singleImage(base64_1, address_value);
                            }
                            else if (!base64_2.empty()) 
                            {
                                singleImage(base64_2, address_value);
                            }

                            oryza_cam->setCallBack(TRUE);
                            oryza_cam->setIsRecognized(TRUE);

                            if (is_running.empty())
                            {
                                return; 
                            }
                            else 
                            {
                                oryza_cam->setStatusCode("201");
                                user_pass_camera(oryza_cam->deviceId(), is_running);
                                oryza_cam->setCallBackEvent(TRUE);
                            }
                        }
                        break;
                    }
                    }
                }
                
                void DataSocket::user_pass_camera(std::string deviceId, std::string status)
                {
                    LOG("Wrong User/Pass Camera START");
                    std::ofstream file;
                    file.open("/opt/networkoptix-metavms/mediaserver/bin/wrongdb.txt", std::ios_base::app);
                    time_t now = time(0);
                    struct tm* timeinfo = localtime(&now);
                    char dt[20];
                    strftime (dt, sizeof(dt),"%Y/%m/%d %H:%M:%S", timeinfo);
                    file << dt << " " << deviceId  << " " << status;
                    file << "\n";
                    LOG("Wrong User/Pass Camera END");
                }

                void DataSocket::singleImage(const std::string& base64_string, LDWORD User) {
                    std::string base64String = base64_string;

                    std::vector<unsigned char> bytes = base64_decode_to_bytes(base64String);

                    DWORD result_dword = get_buffer_size(base64String);
                    
                    captureSingleImage(bytes.data(), result_dword, User);
                }

                void DataSocket::doubleImage(const std::string& base64_1, const std::string& base64_2, LDWORD User) {
                    std::string base64String = base64_1;
                    std::string base64String2 = base64_2;

                    std::vector<unsigned char> bytes1 = base64_decode_to_bytes(base64String);
                    std::vector<unsigned char> bytes2 = base64_decode_to_bytes(base64String2);

                    DWORD result_dword = get_buffer_size(base64String);
                    DWORD result_dword2 = get_buffer_size(base64String2);

                    captureDoubleImage(bytes1.data(), result_dword, User, bytes2.data(), result_dword2);
                }

                //----------------------------------------------------------------------------
                // Base64 to byte
                //---------------------------------------------------------------------------- 
                std::vector<unsigned char> DataSocket::base64_decode_to_bytes(const std::string& base64_string) {
                    typedef boost::archive::iterators::transform_width<
                        boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6> base64_dec;

                    // Calculate the size of the decoded binary data
                    size_t padding = std::count(base64_string.begin(), base64_string.end(), '=');
                    size_t decoded_size = (base64_string.size() * 6) / 8 - padding;

                    // Resize the output vector to the proper size
                    std::vector<unsigned char> decoded_data;
                    decoded_data.reserve(decoded_size);

                    try {
                        std::copy(base64_dec(base64_string.begin()), base64_dec(base64_string.end()), std::back_inserter(decoded_data));
                    }
                    catch (std::exception& e) {
                        // Catch any exceptions during decoding (invalid Base64 string)
                    }

                    return decoded_data;
                };


                //----------------------------------------------------------------------------
                // Base64 to dword
                //---------------------------------------------------------------------------- 
                DWORD DataSocket::get_buffer_size(const std::string& base64_string) {
                    size_t padding = std::count(base64_string.begin(), base64_string.end(), '=');
                    return static_cast<DWORD>((base64_string.size() * 6) / 8 - padding);
                };


                void DataSocket::captureSingleImage(BYTE* pBuffer, DWORD dwBufSize, LDWORD User)
                {
                    LOG("CAPTURE IMAGE");
                    std::string strStoreImgPath;
                    int lastSlashPos;
                    std::string folderImage;
                #ifdef WIN32
                    HMODULE hModule = NULL;

                    hModule = ::GetModuleHandle("detection_analytics_plugin.dll");
                    LOG("IMAGE1");
                    char szFullDllPath[_MAX_PATH] = { 0 };
                    char szModuleDir[_MAX_PATH] = { 0 };

                    ::GetModuleFileName(hModule, szFullDllPath, _MAX_PATH);
                    char* lastBackslash = strrchr(szFullDllPath, '\\');
                    size_t lastBackslashPos = lastBackslash - szFullDllPath + 1;0x7f72b8003250
                    strncpy(szModuleDir, szFullDllPath, lastBackslashPos);
                    strStoreImgPath = std::string(szModuleDir);
                    strStoreImgPath.append("StoreImageFace\\");
                    LOG(strStoreImgPath);
                #else
                    char buffer[4096];
                    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
                        std::string currentDirectory(buffer);
                        memset(buffer, 0, sizeof(buffer));
                        strStoreImgPath = currentDirectory.append("/StoreImageFace/");
                    }
                #endif
                    auto currentTime = std::chrono::system_clock::now();
                    auto timePoint = std::chrono::system_clock::to_time_t(currentTime);
                    auto timeInfo = std::localtime(&timePoint);

                    int day = timeInfo->tm_mday;
                    int month = timeInfo->tm_mon + 1;
                    int year = timeInfo->tm_year + 1900;

                    int hours = timeInfo->tm_hour;
                    int minutes = timeInfo->tm_min;
                    int seconds = timeInfo->tm_sec;
                    auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                        currentTime.time_since_epoch()
                    ).count();
                    miliseconds %= 1000;

                    std::string currentTimeStr = std::to_string(year) + std::to_string(month) + std::to_string(day) + std::to_string(hours) + std::to_string(minutes) + std::to_string(seconds) + std::to_string(miliseconds);
                    std::string strJpgFile1 = strStoreImgPath + std::string(currentTimeStr) + std::string("_.jpg");

                    reinterpret_cast<ICamera*>(User)->setImgpath(strJpgFile1);

                    // picture 1
                    FILE* fp1 = fopen(strJpgFile1.c_str(), "wb");

                    if (fp1 == NULL)
                    {
                        Logger::instance()->writeLogServer(__func__, "Save picture failed!");
                        return;
                    }
                    fwrite(pBuffer, dwBufSize, 1, fp1);
                    fclose(fp1);

                    Logger::instance()->writeLogServer(__func__, "create image success : " + strJpgFile1);
                }

                void DataSocket::captureDoubleImage(BYTE* pBuffer1, DWORD dwBufSize1, LDWORD User, BYTE* pBuffer2, DWORD dwBufSize2) 
                {
                    LOG("CAPTURE IMAGE");
                    std::string strStoreImgPath;
                    int lastSlashPos;
                    std::string folderImage;
                #ifdef WIN32
                    HMODULE hModule = NULL;

                    hModule = ::GetModuleHandle("detection_analytics_plugin.dll");
                    LOG("IMAGE1");
                    char szFullDllPath[_MAX_PATH] = { 0 };
                    char szModuleDir[_MAX_PATH] = { 0 };

                    ::GetModuleFileName(hModule, szFullDllPath, _MAX_PATH);
                    char* lastBackslash = strrchr(szFullDllPath, '\\');
                    size_t lastBackslashPos = lastBackslash - szFullDllPath + 1;0x7f72b8003250
                    strncpy(szModuleDir, szFullDllPath, lastBackslashPos);
                    strStoreImgPath = std::string(szModuleDir);
                    strStoreImgPath.append("StoreImageFace\\");
                    LOG(strStoreImgPath);
                #else
                    char buffer[4096];
                    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
                        std::string currentDirectory(buffer);
                        memset(buffer, 0, sizeof(buffer));
                        strStoreImgPath = currentDirectory.append("/StoreImageFace/");
                    }
                #endif
                    auto currentTime = std::chrono::system_clock::now();
                    auto timePoint = std::chrono::system_clock::to_time_t(currentTime);
                    auto timeInfo = std::localtime(&timePoint);

                    int day = timeInfo->tm_mday;
                    int month = timeInfo->tm_mon + 1;
                    int year = timeInfo->tm_year + 1900;

                    int hours = timeInfo->tm_hour;
                    int minutes = timeInfo->tm_min;
                    int seconds = timeInfo->tm_sec;
                    auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                        currentTime.time_since_epoch()
                    ).count();
                    miliseconds %= 1000;

                    std::string currentTimeStr = std::to_string(year) + std::to_string(month) + std::to_string(day) + std::to_string(hours) + std::to_string(minutes) + std::to_string(seconds) + std::to_string(miliseconds);
                    std::string strJpgFile1 = strStoreImgPath + std::string(currentTimeStr) + std::string("_1.jpg");
                    std::string strJpgFile2 = strStoreImgPath + std::string(currentTimeStr) + std::string("_2.jpg");
                    std::string strJpgFile3 = strStoreImgPath + std::string(currentTimeStr) + std::string("_merge.jpg");
                   
                    reinterpret_cast<ICamera*>(User)->setImgpath(strJpgFile3);

                    // LOG("IMAGE5");
                    // reinterpret_cast<ICamera*>(User)->setImgpath(strJpgFile1);
                    // LOG("IMAGE6");
                    // LOG(strJpgFile1);

                    // picture 1
                    FILE* fp1 = fopen(strJpgFile1.c_str(), "wb");

                    if (fp1 == NULL)
                    {
                        Logger::instance()->writeLogServer(__func__, "Save picture failed!");
                        return;
                    }
                    fwrite(pBuffer1, dwBufSize1, 1, fp1);
                    // fwrite(pBuffer, dwBufSize, 1, fp1);
                    fclose(fp1);

                    //picture 2
                    FILE* fp2 = fopen(strJpgFile2.c_str(), "wb");

                    if (fp2 == NULL)
                    {
                        Logger::instance()->writeLogServer(__func__, "Save picture failed!");
                        return;
                    }
                    fwrite(pBuffer2, dwBufSize2, 1, fp2);
                    fclose(fp2);

                    //picture 3
                    // Mat3b img1 = imread(strJpgFile1);
                    // Mat3b img2 = imread(strJpgFile2);
                    // LOG("IMAGE10");
                    // int rows = max(img1.rows, img2.rows);
                    // int cols = img1.cols + img2.cols;

                    // Mat3b res(rows, cols, Vec3b(0,0,0));
                    // LOG("IMAGE11");

                    // img1.copyTo(res(Rect(0, 0, img1.cols, img1.rows)));
                    // img2.copyTo(res(Rect(img1.cols, 0, img2.cols, img2.rows)));
                    // LOG("IMAGE12");

                    
                    // cv::imwrite(strJpgFile3, res);
                    // Logger::instance()->writeLogServer(__func__, "create image success : " + strJpgFile1);
                }
            }
        }
    }
}