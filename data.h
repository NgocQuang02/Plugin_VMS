#pragma once
#include <string>
#include <vector>

namespace nx {
	namespace vms_server_plugins {
		namespace analytics {
			namespace stub {
				/* Traffic Plate SDK */
				struct PlateInfo {
					std::string lane;
					std::string speed;
					std::string licensePlate;
					std::string licensePlateColor;
					std::string vehicleType;
				};

				class Plate_Detection_SDK {
				private:
					PlateInfo plateData;

				public:
					// Constructor
					Plate_Detection_SDK();

					// Getter
					PlateInfo getPlateInfo();

					// Setter
					void setPlateInfo(const int& newLane, const int& newSpeed, const std::string& newLicensePlate,
						const std::string& newLicensePlateColor, const std::string& newVehicleType);
				};


				/* Face Detection SDK */
				struct DetectedInfo {
					std::string sex;
					std::string age;
					std::string eye;
					std::string mouth;
					std::string beard;
					std::string glass;
					std::string mask;
				};

				struct RecognizedInfo {
					std::string name;
					std::string birthday;
					std::string facepic;
					std::string sex;
					std::string age;
					std::string eye;
					std::string mouth;
					std::string beard;
					std::string glass;
					std::string mask;
				};

				class Face_Detection_SDK {
				private:
					DetectedInfo detectedInfo;
					RecognizedInfo recognizedInfo;

				public:
					// Constructor
					Face_Detection_SDK();

					// Getter
					DetectedInfo getDetectedInfo();

					RecognizedInfo getRecognizedInfo();

					// Setter
					void setDetectedInfo(const int& newSex, const int& newAge, const int& newEye, const int& newMouth, const int& newBeard, const int& newGlass, const int& newMask);

					void setRecognizedInfo(const std::string& newName, const std::string& newBirthday, const std::string& newFacepic, const int& newSex,
						const int& newAge, const int& newEye, const int& newMouth, const int& newBeard, const int& newGlass, const int& newMask);

				};
			} // namespace sample
		} // namespace analytics
	} // namespace vms_server_plugins
} // namespace nx