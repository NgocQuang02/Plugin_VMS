#include "oryza_camera.h"

using namespace nx::vms_server_plugins::analytics::stub;
namespace nx {
    namespace vms_server_plugins {
        namespace analytics {
            namespace stub {
                namespace CrowdDetection {

                    OryzaCamera::OryzaCamera() {

                    }

                    OryzaCamera::~OryzaCamera() {

                    }
                    void OryzaCamera::initialize() {
                        LOG("Oryza camera initialized.");
                       
                        p_ev_cntrl.reset(new OryzaEventController());
                        p_connection.reset(new OryzaSDKConnection());
                    }

                    void OryzaCamera::configure() {
                       LOG( "Oryza camera configured.");
                        /*auto myid = std::this_thread::get_id();

                        std::stringstream ss;
                        ss << this;
                        std::string Address = ss.str();
                        LOG("ID thread Oryza cam: -- " + Address);*/
                    }

                    bool OryzaCamera::login() {
                        LOG("Oryza camera log in.");
                        p_connection->login((LDWORD)this);
                        return p_connection->loginSuccess((LDWORD)this);
                    }

                    bool OryzaCamera::logout() {
                        LOG("Oryza camera logged out.");
                        p_connection->logout((LDWORD)this);
                        return true;
                    }

                    bool OryzaCamera::subscribe() {
                        LOG("Oryza camera subscribed.");
                        p_connection->subscribe((LDWORD)this);

                        std::stringstream ss;
                        ss << this;
                        std::string Address = ss.str();

                        return p_connection->subscribeSuccess();
                    }

                    bool OryzaCamera::unsubscribe() {
                        LOG("Oryza camera unsubscribed.");;
                        p_connection->unsubscribe((LDWORD)this);
                        return true;
                    }

                    void OryzaCamera::doAction() {
                        std::cout << "Oryza camera performing an action." << std::endl;
                    }

                    void OryzaCamera::capture(BYTE* pBuffer, DWORD dwBufSize, LDWORD User) {
                        p_ev_cntrl->capture(pBuffer, dwBufSize, User);
                    }

                    void OryzaCamera::setCallBack(const bool& callback)
                    {
                        p_ev_cntrl->setCallback(callback);
                    }

                    bool OryzaCamera::callback() const
                    {
                        return p_ev_cntrl->callback();
                    }

                    void OryzaCamera::setCallBackEvent(const bool& callbackEvent)
                    {
                        p_ev_cntrl->setCallBackEvent(callbackEvent);
                    }

                    bool OryzaCamera::callbackEvent() const
                    {
                        return p_ev_cntrl->callbackEvent();
                    }

                    std::string OryzaCamera::StatusCode() const
                    {
                        return p_ev_cntrl->StatusCode();
                    }

                    void OryzaCamera::setStatusCode(const std::string& StatusCode)
                    {
                        p_ev_cntrl->setStatusCode(StatusCode);
                    }

                    void OryzaCamera::setIsRecognized(const bool& isRecognized)
                    {
                        p_ev_cntrl->setIsRecognized(isRecognized);
                    }

                    bool OryzaCamera::isRecognized() const
                    {
                        return p_ev_cntrl->isRecognized();
                    }

                    void OryzaCamera::setImgpath(const std::string& imgpath)
                    {
                        p_ev_cntrl->setImgpath(imgpath);
                    }

                    std::string OryzaCamera::imgpath() const
                    {
                        return p_ev_cntrl->imgpath();
                    }

                    std::string OryzaCamera::ipServer() const
                    {
                        return p_ev_cntrl->ipServer();
                    }

                    void OryzaCamera::setIpServer(const std::string& ipServer)
                    {
                        p_ev_cntrl->setIpServer(ipServer);
                    }

                    std::string OryzaCamera::ipClient() const
                    {
                        return p_ev_cntrl->ipClient();
                    }

                    void OryzaCamera::setIpClient(const std::string& ipClient)
                    {
                        p_ev_cntrl->setIpClient(ipClient);
                    }

                     std::string OryzaCamera::timeAlarm() const
                    {
                        return p_ev_cntrl->timeAlarm();
                    }

                    void OryzaCamera::setTimeAlarm(const std::string& timeAlarm)
                    {
                        p_ev_cntrl->setTimeAlarm(timeAlarm);
                    }

                    int OryzaCamera::Mincnt() const
                    {
                        return p_ev_cntrl->Mincnt();
                    }

                    void OryzaCamera::setMincnt(const int min)
                    {
                        p_ev_cntrl->setMincnt(min);
                    }

                    std::string OryzaCamera::Cam() const
                    {
                        return p_ev_cntrl->Cam();
                    }

                    void OryzaCamera::setCam(const std::string& Cam)
                    {
                        p_ev_cntrl->setCam(Cam);
                    }

                    std::string OryzaCamera::host() const
                    {
                        return p_ev_cntrl->host();
                    }

                    void OryzaCamera::setHost(const std::string& host)
                    {
                        p_ev_cntrl->setHost(host);
                    }

                    std::string OryzaCamera::port() const
                    {
                        return p_ev_cntrl->port();;
                    }

                    void OryzaCamera::setPort(const std::string& post)
                    {
                        p_ev_cntrl->setPort(post);
                    }

                    std::string OryzaCamera::username() const
                    {
                        return p_ev_cntrl->username();
                    }

                    void OryzaCamera::setUsername(const std::string& username)
                    {
                        p_ev_cntrl->setUsername(username);
                    }

                    std::string OryzaCamera::password() const
                    {
                        return p_ev_cntrl->password();
                    }

                    void OryzaCamera::setPassword(const std::string& password)
                    {
                        p_ev_cntrl->setPassword(password);
                    }

                    std::string OryzaCamera::url() const
                    {
                        return p_ev_cntrl->url();
                    }

                    void OryzaCamera::setUrl(const std::string& url)
                    {
                        p_ev_cntrl->setUrl(url);
                    }

                    std::string OryzaCamera::Points() const
                    {
                        return p_ev_cntrl->Points();
                    }

                    void OryzaCamera::setPoints(const std::string& Points)
                    {
                        p_ev_cntrl->setPoints(Points);
                    }

                    std::string OryzaCamera::deviceId() const
                    {
                        return p_ev_cntrl->deviceId();
                    }

                    void OryzaCamera::setDeviceId(const std::string deviceId)
                    {
                        p_ev_cntrl->setdeviceId(deviceId);
                    }
                
                    std::vector<std::string> OryzaCamera::faceDataInfo() const
                    {
                        return p_ev_cntrl->faceDataInfo();
                    }

                    void OryzaCamera::setFaceDataInfo(const std::vector<std::string> faceDataInfo)
                    {
                        p_ev_cntrl->setFaceDataInfo(faceDataInfo);
                    }

                    std::vector<std::string> OryzaCamera::recognitionDataInfo() const
                    {
                        return p_ev_cntrl->recognitionDataInfo();
                    }

                    void OryzaCamera::setRecognitionDataInfo(const std::vector<std::string> faceRecognitionInfo)
                    {
                        p_ev_cntrl->setRecognitionDataInfo(faceRecognitionInfo);
                    }
                    std::vector<std::string> OryzaCamera::trafficInfo() const
                    {
                        return std::vector<std::string>();
                    }

                    void OryzaCamera::setTrafficInfo(const std::vector<std::string> trafficInfo)
                    {

                    }

                    void OryzaCamera::saveFaceRecognition(std::vector<std::string> pstInfo)
                    {
                        p_ev_cntrl->saveFaceRecognition(pstInfo);
                    }

                }
            }
        }
    }
}