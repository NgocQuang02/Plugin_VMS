// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#define NX_DEBUG_ENABLE_OUTPUT true
#include "engine.h"
#include <nx/kit/debug.h>

#include "device_agent.h"
#include "stub_analytics_plugin_FaceDetection_ini.h"


namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {

					using namespace nx::sdk;
					using namespace nx::sdk::analytics;

					Engine::Engine() :
						// Call the DeviceAgent helper class constructor telling it to verbosely report to stderr.
						nx::sdk::analytics::Engine(ini().enableOutput)
					{
					}

					Engine::~Engine()
					{
					}

					/**
					 * Called when the Server opens a video-connection to the camera if the plugin is enabled for this
					 * camera.
					 *
					 * @param outResult The pointer to the structure which needs to be filled with the resulting value
					 *     or the error information.
					 * @param deviceInfo Contains various information about the related device such as its id, vendor,
					 *     model, etc.
					 */
					void Engine::doObtainDeviceAgent(Result<IDeviceAgent*>* outResult, const IDeviceInfo* deviceInfo)
					{
						*outResult = new DeviceAgent(deviceInfo);
					}

					/**
					 * @return JSON with the particular structure. Note that it is possible to fill in the values
					 *     that are not known at compile time, but should not depend on the Engine settings.
					 */
					std::string Engine::manifestString() const
					{
						// Ask the Server to supply uncompressed video frames in YUV420 format (see
						// https://en.wikipedia.org/wiki/YUV).
						// Note that this format is used internally by the Server, therefore requires minimum
						// resources for decoding, thus it is the recommended format.
						return /*suppress newline*/ 1 + (const char*)R"json(
{
	"typeLibrary":
    {
        "eventTypes":
        [
            {
                "id": "nx.stub.crwod_plugin_test.CrowdEventType",
                "name": "Login Event"
            }
        ]
	},
    "capabilities": "needUncompressedVideoFrames_yuv420",
    "deviceAgentSettingsModel":
)json"
							+ kLoginSettingsModel
							+ R"json(,
	"objectActions": {
		"requirements": {
			"capabilities": "needBestShotVideoFrame",
			"bestShotVideoFramePixelFormat": "yuv420"
		}
	}
}
)json";
					}

				} // namespace sample
			}
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx