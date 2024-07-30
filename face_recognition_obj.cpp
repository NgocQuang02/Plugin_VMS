#include <vector>

#include "face_recognition_obj.h"


namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				using namespace nx::sdk;
					static Attributes makeAttributes(std::vector<std::string> recognitionDataInfo)
					{
						return {
							nx::sdk::makePtr<Attribute>(IAttribute::Type::string, "Cảnh báo tụ tập đám đông!", ""),
						};
					}

				FaceRecognitionObj::FaceRecognitionObj(std::vector<std::string> recognitionDataInfo):
					base_type(kFaceRecognitionObjectType, makeAttributes(recognitionDataInfo))
				{
				}
				
			} // namespace stub
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx