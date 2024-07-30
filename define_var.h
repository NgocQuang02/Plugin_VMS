#pragma once
#include <iostream>
#include <unordered_map>
#include <dhnetsdk.h>
#ifdef _WIN64
#pragma comment(lib, "dhnetsdk.lib")
#endif

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				namespace CrowdDetection {
					const std::string kSex[] = {
						"Unknown", // EM_DEV_EVENT_FACEDETECT_SEX_TYPE_UNKNOWN
						"Man", // EM_DEV_EVENT_FACEDETECT_SEX_TYPE_MAN
						"Woman" // EM_DEV_EVENT_FACEDETECT_SEX_TYPE_WOMAN
					};

					const std::string kEye[] = {
						"Unknown", // EM_EYE_STATE_UNKNOWN
						"Not distinguished", // EM_EYE_STATE_NODISTI
						"Close", // EM_EYE_STATE_CLOSE
						"Open" // EM_EYE_STATE_OPEN
					};

					const std::string kBeard[] = {
						"Unknown", // EM_BEARD_STATE_UNKNOWN
						"Not distinguished", // EM_BEARD_STATE_NODISTI
						"No beard", // EM_BEARD_STATE_NOBEARD
						"Have beard" // EM_BEARD_STATE_HAVEBEARD
					};

					const std::string kGlass[] = {
						"Unknown", // EM_FACEDETECT_GLASSES_UNKNOWN
						"Wear glasses", // EM_FACEDETECT_WITH_GLASSES
						"Without glasses", // EM_FACEDETECT_WITHOUT_GLASSES
						"Wear sun glasses", // EM_FACEDETECT_SUN_GLASSES
						"Wear black frame glasses" // EM_FACEDETECT_BLACKFRAME_GLASSES
					};

					const std::string kMouth[] = {
						"Unknown", // EM_MOUTH_STATE_UNKNOWN
						"Not distinguished", // EM_MOUTH_STATE_NODISTI
						"Close", // EM_MOUTH_STATE_CLOSE
						"Open" // EM_MOUTH_STATE_OPEN
					};

					const std::string kMask[] = {
						"Unknown", // EM_MASK_STATE_UNKNOWN
						"Not distinguished", // EM_MASK_STATE_NODISTI
						"No mask", // EM_MASK_STATE_NOMASK
						"Wearing mask" // EM_MASK_STATE_WEAR
					};

					enum faceDataEnum {
						e_Age,
						e_Sex,
						e_Beard,
						e_Eye,
						e_Mouth,
						e_Mask,
						e_Glass
					};

					enum recognitionDataEnum {
						e_Name,
						e_id,
						e_trafficplate,
						e_verifyout						
					};

				}
			}
		}
	}
}