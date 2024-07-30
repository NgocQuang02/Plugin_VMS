#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <string>
//#include <nx/kit/json.h>
#include "../../json11.hpp"
#include "../../Logger.h"
#include <cstdio>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace http = boost::beast::http;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

//using nx::kit::Json;
using namespace nx::vms_server_plugins::analytics::stub;

namespace nx {
    namespace vms_server_plugins {
        namespace analytics {
            namespace stub {
                namespace CrowdDetection {
                    // Sends a WebSocket message and prints the response
                    class CWebSocket_Sync {
                        tcp::resolver resolver_;
                        websocket::stream<tcp::socket> ws_;
                        boost::beast::multi_buffer buffer_;
                        std::string host_;
                    public:
                        // Resolver and socket require an io_context
                        explicit CWebSocket_Sync(boost::asio::io_context& ioc);
                        void connectWeb(char const* host, char const* port);
                        void send_data(std::string text);
                        std::string read_data();
                        void CloseConnection();
                    };
                }
            }
        }
    }
}