#pragma once
#include <dhnetsdk.h>
#ifdef WIN64
#pragma comment(lib, "dhnetsdk.lib")
#endif

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				class ISDKConnection
				{
				public:
					virtual void login(LDWORD dwUser) = 0;

					virtual void logout(LDWORD dwUser) = 0;

					virtual void subscribe(LDWORD dwUser) = 0;

					virtual void unsubscribe(LDWORD dwUser) = 0;
				};
			}
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx

