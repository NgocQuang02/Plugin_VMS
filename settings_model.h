#pragma once
#include <string>

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
                namespace CrowdDetection{
				const std::string kUsernameSetting = "username1";

				const std::string kPasswordSetting = "password1";

                const std::string kIpSetting = "ip1";

                const std::string kTime = "time1";

                const std::string kIpRecv = "IPRECV1";

				const std::string kLoginSettingsModel =
					/*suppress newline*/ 1 + (const char*)R"json("
{
    "type": "Settings",
    "items":
    [
        {
            "type": "GroupBox",
            "caption": "Login",
            "items":
            [
                {
                    "type": "TextField",
                    "name": ")json" + kUsernameSetting + R"json(",
                    "caption": "Username",
                    "defaultValue": "admin"
                
                },
                {
                    "type": "PasswordField",
                    "name": ")json" + kPasswordSetting + R"json(",
                    "caption": "Password",
                    "defaultValue": "Oryza@123"
                },
                {
                    "type": "SpinBox",
                    "name": "MinCount",
                    "caption": "Minimum human count",
                    "description": "Max = 50",
                    "defaultValue": 3,
                    "minValue": 1,
                    "maxValue": 50,
                    "isActive": false
                },
                {
                    "type": "TextField",
                    "name": ")json" + kTime + R"json(",
                    "caption": "Time to Start Alarm",
                    "defaultValue": "2"
                },
                {
                    "type": "TextField",
                    "name": ")json" + kIpRecv + R"json(",
                    "caption": "Ip Client",
                    "defaultValue": "192.168.105.111"
                },
                {
                    "type": "TextField",
                    "name": ")json" + kIpSetting + R"json(",
                    "caption": "Ip Server BE",
                    "defaultValue": "192.168.105.111"
                }
            ]
        },
        {
            "type": "GroupBox",
            "caption": "Polygons",
            "items":
            [
                {
                    "type": "PolygonFigure",
                    "name": "excludedArea.figure",
                    "caption": "Excluded area",
                    "useLabelField": false,
                    "maxPoints": 8
                }
            ]
        }
    ]
}
)json";

				const std::string kLoginSuccessSettingsModel =
					/*suppress newline*/ 1 + (const char*)R"json("
{
    
})json";
                }
			} // namespace stub
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx