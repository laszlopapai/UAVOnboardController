#pragma once


#include <Thread/IFilter.h>
#include <pwd.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>

namespace IoT {
	namespace Device {

		template<typename T>
		class CalibratedFilter
			: public Core::IFilter<T>
		{
		public:
			virtual bool isCalibrating() = 0;
			virtual bool isCalibrated() = 0;
			virtual bool calibrate(T data, uint32_t deltaTime) = 0;

		protected:
			bool loadCalibration(std::string filename, void *data, size_t size) {
				auto pw = getpwuid(getuid());
				std::string appPath = std::string(pw->pw_dir) + "/.drone/";
				std::ifstream ifs(appPath + filename, std::ios::binary);

				if (ifs.good()) {
					ifs.read((char *) data, size);
					ifs.close();
					return true;
				}
				return false;
			}

			bool saveCalibration(std::string filename, void *data, size_t size) {
				auto pw = getpwuid(getuid());
				std::string appPath = std::string(pw->pw_dir) + "/.drone/";
				std::system((std::string("mkdir ") + appPath).c_str());
				std::ofstream ofs(appPath + filename, std::ios::binary);
				auto thisPtr = reinterpret_cast<char *>(this);

				if (ofs.good()) {
					ofs.write((char *) data, size);
					ofs.close();
					return true;
				}

				return false;
			}
		};
	}
}
