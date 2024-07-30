#pragma once
#include <dhnetsdk.h>
#ifdef WIN64
#pragma comment(lib, "dhnetsdk.lib")
#endif
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "../../i_SDK_connection.h"


namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {

					class DahuaSDKConnection : public ISDKConnection
					{
					public:
						DahuaSDKConnection();
						 ~DahuaSDKConnection();

					public:
						void login(LDWORD dwUser) override;

						void logout(LDWORD dwUser) override;

						void subscribe(LDWORD dwUser) override;

						void unsubscribe(LDWORD dwUser) override;

						bool loginSuccess();
						
						bool subscribeSuccess();

					private:
						LLONG m_lLoginID = 0;
						LLONG m_lRealPicHandle = 0;
						BOOL m_bNetSDKInitFlag = 0;
					};

				} // namespace sample
			}
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx


