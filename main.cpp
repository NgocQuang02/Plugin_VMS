#include <nx/kit/debug.h>

#include "crowd_detection/plugin.h"

extern "C" NX_PLUGIN_API nx::sdk::IPlugin * createNxPluginByIndex(int instanceIndex)
{
	using namespace nx::vms_server_plugins::analytics::stub;

	switch (instanceIndex)
	{
		case 0: return new CrowdDetection::Plugin();

		default: return nullptr;
	}
}
