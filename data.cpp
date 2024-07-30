#include "data.h"
#include <vector>


namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {

				/* Traffic Plate SDK */
				Plate_Detection_SDK::Plate_Detection_SDK() {}

				PlateInfo Plate_Detection_SDK::getPlateInfo() {
					return plateData;
				}

				void Plate_Detection_SDK::setPlateInfo(const int& newLane, const int& newSpeed, const std::string& newLicensePlate,
					const std::string& newLicensePlateColor, const std::string& newVehicleType) {
					plateData.lane = std::to_string(newLane);
					plateData.speed = std::to_string(newSpeed);
					plateData.licensePlate = newLicensePlate;
					plateData.licensePlateColor = newLicensePlateColor;
					plateData.vehicleType = newVehicleType;
				}


				/* Face Detection SDK */
				Face_Detection_SDK::Face_Detection_SDK() {}

				DetectedInfo Face_Detection_SDK::getDetectedInfo() {
					return detectedInfo;
				}

				RecognizedInfo Face_Detection_SDK::getRecognizedInfo() {
					return recognizedInfo;
				}

				void Face_Detection_SDK::setDetectedInfo(const int& newSex, const int& newAge, const int& newEye, const int& newMouth,
					const int& newBeard, const int& newGlass, const int& newMask) {
					detectedInfo.sex = std::to_string(newSex);
					detectedInfo.age = std::to_string(newAge);
					detectedInfo.eye = std::to_string(newEye);
					detectedInfo.mouth = std::to_string(newMouth);
					detectedInfo.beard = std::to_string(newBeard);
					detectedInfo.glass = std::to_string(newGlass);
					detectedInfo.mask = std::to_string(newMask);
				}

				void Face_Detection_SDK::setRecognizedInfo(const std::string& newName, const std::string& newBirthday, const std::string& newFacepic,
					const int& newSex, const int& newAge, const int& newEye, const int& newMouth, const int& newBeard, const int& newGlass, const int& newMask) {
					recognizedInfo.name = newName;
					recognizedInfo.birthday = newBirthday;
					recognizedInfo.facepic = newFacepic;
					recognizedInfo.sex = std::to_string(newSex);
					recognizedInfo.age = std::to_string(newAge);
					recognizedInfo.eye = std::to_string(newEye);
					recognizedInfo.mouth = std::to_string(newMouth);
					recognizedInfo.beard = std::to_string(newBeard);
					recognizedInfo.glass = std::to_string(newGlass);
					recognizedInfo.mask = std::to_string(newMask);
				}
			} // namespace sample
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx