#include <vector>

#include "face_detection_obj.h"

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				using namespace nx::sdk;
				namespace {

					static Attributes makeAttributes(std::vector<std::string> faceDataInfo)
					{
						return {
							nx::sdk::makePtr<Attribute>(IAttribute::Type::string, "Age", faceDataInfo[e_Age]),
							nx::sdk::makePtr<Attribute>(IAttribute::Type::string, "Sex", faceDataInfo[e_Sex]),
							nx::sdk::makePtr<Attribute>(IAttribute::Type::string, "Beard", faceDataInfo[e_Beard]),
							nx::sdk::makePtr<Attribute>(IAttribute::Type::string, "Eye", faceDataInfo[e_Eye]),
							nx::sdk::makePtr<Attribute>(IAttribute::Type::string, "Mouth", faceDataInfo[e_Mouth]),
							nx::sdk::makePtr<Attribute>(IAttribute::Type::string, "Mask", faceDataInfo[e_Mask]),
							nx::sdk::makePtr<Attribute>(IAttribute::Type::string, "Glass", faceDataInfo[e_Glass])
						};
					}

				} // namespace

				FaceDetectionObj::FaceDetectionObj(std::vector<std::string> faceDataInfo) :
					base_type(kNewFaceDetectionObjectType, makeAttributes(faceDataInfo))
				{
				}

			} // namespace stub
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx