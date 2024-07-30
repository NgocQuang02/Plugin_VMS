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

#include "../../Logger.h"

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {

					class WorkerThread;

					class OryzaSDKConnection : public ISDKConnection
					{
					public:
						OryzaSDKConnection();
						~OryzaSDKConnection();

					public:
						void login(LDWORD dwUser) override;

						void logout(LDWORD dwUser) override;

						void subscribe(LDWORD dwUser) override;

						void unsubscribe(LDWORD dwUser) override;

						bool loginSuccess(LDWORD dwUser);

						bool subscribeSuccess();

					private:
						LLONG m_lLoginID = 1;
						LLONG m_lRealPicHandle = 1;
						BOOL m_bNetSDKInitFlag = 0;

						WorkerThread* workerthread;
						WorkerThread* socketThread;
						//std::shared_ptr<WorkerThread> workerthread (new WorkerThread());
					};

				} // namespace sample
			}
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx


