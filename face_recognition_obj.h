#pragma once
#include "abstract_obj.h"


using namespace nx::vms_server_plugins::analytics::stub::CrowdDetection;
namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {

				const std::string kFaceRecognitionObjectType = "nx.stub.crwod_plugin_test.CrowdObjectType";

				class FaceRecognitionObj : public AbstractObj
				{
					using base_type = AbstractObj;
				public:
					FaceRecognitionObj(std::vector<std::string> recognitionDataInfo);
				};

			} // namespace stub
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx