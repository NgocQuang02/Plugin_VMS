#include "Websocket.h"
namespace nx {
    namespace vms_server_plugins {
        namespace analytics {
            namespace stub {
                namespace CrowdDetection {
                    CWebSocket_Sync::CWebSocket_Sync(boost::asio::io_context& ioc)
                        : resolver_(ioc), ws_(ioc) { }

                    void CWebSocket_Sync::connectWeb(char const* host, char const* port) {
                        // Save these for later
                        LOG("CONNECT WEBSOCKET");
                        host_ = host;

                        // Look up the domain name
                        auto const results = resolver_.resolve(host, port);

                        // Make the connection on the IP address we get from a lookup
                        auto ep = net::connect(ws_.next_layer(), results);
                        Logger::instance()->writeLogServer(__func__, "connect websocket success");
                        // Update the host_ string. This will provide the value of the
                        // Host HTTP header during the WebSocket handshake.
                        // See https://tools.ietf.org/html/rfc7230#section-5.4
                        host_ += ':' + std::to_string(ep.port());

                        // Set a decorator to change the User-Agent of the handshake
                        ws_.set_option(websocket::stream_base::decorator([](auto& m) {
                            m.set("Authorization", "W+wBZ1TcC-RMcQM4ywXg");
                            }));

                        // Perform the websocket handshake
                        ws_.handshake(host_, "/ws");
                        LOG("CONNECT WEBSOCKET SUCCESS");
                    }

                    void CWebSocket_Sync::send_data(std::string text) {

                        ws_.write(net::buffer(text));
                        beast::flat_buffer buffer;
                        //read server return data
                        ws_.read(buffer);

                        //convert buffer to string
                        auto bufs = buffer.data();
                        std::string retVal(buffers_begin(bufs), buffers_begin(bufs) + buffer.size());
                        std::string err;
                        //convert from string to json
                        const auto jsonRet = json11::Json::parse(retVal, err);
                        std::string data = jsonRet.dump();

                        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
                        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
                        Logger::instance()->writeLogServer(__func__, std::ctime(&currentTime));
                        Logger::instance()->writeLogServer(__func__, data);
                        buffer.clear();
                    }

                    std::string CWebSocket_Sync::read_data()
                    {
                        beast::flat_buffer buffer;
                        ws_.write(net::buffer(""));
                        //read server return data
                        ws_.read(buffer);

                        //convert buffer to string
                        auto bufs = buffer.data();
                        std::string retVal(buffers_begin(bufs), buffers_begin(bufs) + buffer.size());
                        std::string err;
                        //convert from string to json
                        const auto jsonRet = json11::Json::parse(retVal, err);

                        // std::cout << ": " <<  << std::endl;
                        // std::cout << text << std::endl;
                        std::string data = jsonRet["Hello"].string_value();
                        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
                        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
                        Logger::instance()->writeLogServer(__func__, std::ctime(&currentTime));
                        Logger::instance()->writeLogServer(__func__, data);
                        buffer.clear();
                        return data;
                    }

                    void CWebSocket_Sync::CloseConnection() {
                        Logger::instance()->writeLogServer(__func__, "close socket");
                        ws_.close(websocket::close_code::normal);
                        Logger::instance()->writeLogServer(__func__, "close socket2");
                    }
                }
            }
        }
    }
}