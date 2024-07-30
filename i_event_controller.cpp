#include "i_event_controller.h"

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {

					IEventController::IEventController() {

					}

					IEventController::~IEventController() {

					}

					bool IEventController::callback()
					{
						return m_callback;
					}

					void IEventController::setCallback(bool callback)
					{
						m_callback = callback;
					}

					bool IEventController::callbackEvent()
					{
						return m_callbackevent;
					}

					void IEventController::setCallBackEvent(bool callbackEvent)
					{
						m_callbackevent = callbackEvent;
					}

					std::string IEventController::StatusCode() {
						return m_statuscode;
					}

					void IEventController::setStatusCode(std::string StatusCode) {
						m_statuscode = StatusCode;
					}

					bool IEventController::isRecognized()
					{
						return m_isRecognized;
					}

					void IEventController::setIsRecognized(bool isRecognized)
					{
						m_isRecognized = isRecognized;
					}

					void IEventController::setFaceDataInfo(std::vector<std::string> faceDataInfo)
					{
						m_facedataInfo = faceDataInfo;
					}

					std::vector<std::string> IEventController::faceDataInfo()
					{
						return m_facedataInfo;
					}

					void IEventController::setRecognitionDataInfo(std::vector<std::string> recognitionDataInfo)
					{
						m_recognitionDataInfo = recognitionDataInfo;
					}

					std::vector<std::string> IEventController::recognitionDataInfo()
					{
						return m_recognitionDataInfo;
					}

					std::string IEventController::username() {
						return m_username;
					}

					void IEventController::setUsername(std::string username) {
						m_username = username;
					}

					std::string IEventController::password() {
						return m_password;
					}

					void IEventController::setPassword(std::string password) {
						m_password = password;
					}

					std::string IEventController::host() {
						return m_host;
					}

					void IEventController::setHost(std::string host) {
						m_host = host;
					}

					std::string IEventController::imgpath() {
						return m_imgpath;
					}

					void IEventController::setImgpath(std::string imgpath) {
						m_imgpath = imgpath;
					}

					std::string IEventController::port() {
						return m_port;
					}

					void IEventController::setPort(std::string port) {
						m_port = port;
					}

					std::string IEventController::deviceId() {
						return m_deviceId;
					}

					void IEventController::setdeviceId(const std::string deviceId) {
						m_deviceId = deviceId;
					}

					std::string IEventController::ipServer() {
						return m_ipServer;
					}

					void IEventController::setIpServer(std::string ipServer) {
						m_ipServer = ipServer;
					}

					std::string IEventController::ipClient() {
						return m_ipClient;
					}

					void IEventController::setIpClient(std::string ipClient) {
						m_ipClient = ipClient;
					}

					std::string IEventController::timeAlarm() {
						return m_timeAlarm;
					}

					void IEventController::setTimeAlarm(std::string timeAlarm) {
						m_timeAlarm = timeAlarm;
					}

					std::string IEventController::Cam() {
						return m_cam;
					}

					void IEventController::setCam(const std::string Cam) {
						m_cam = Cam;
					}

					std::string IEventController::url() {
						return m_url;
					}

					void IEventController::setUrl(const std::string url) {
						m_url = url;
					}

					std::string IEventController::Points() {
						return m_points;
					}

					void IEventController::setPoints(const std::string Points) {
						m_points = Points;
					}

					int IEventController::Mincnt()	 {
						return m_min;
					}
					void IEventController::setMincnt(const int Mincnt) {
						m_min = Mincnt;
					}

				} // namespace sample
			}
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx