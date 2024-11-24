#pragma once

#include <vector>
#include <string>
#include <format>
#include <iostream>
#include <exception>

namespace labbish {
	class version_exception :public std::runtime_error {
	public:
		explicit version_exception(const std::string& message) :std::runtime_error(message) {}
	};

	struct VersionInfo {
		std::vector<int> version; //e.g. "v5.1.4"->{5,1,4}
		inline VersionInfo() noexcept {}
		inline VersionInfo(std::vector<int> version) noexcept :version(version) {}
		inline VersionInfo(std::string version) {
			if (version == "" || version == "v") throw version_exception("Empty version string");
			if (version[0] != 'v') throw version_exception(std::format("Version string not starting with 'v': \"{}\"", version));
			version = version.substr(1);
			std::vector<size_t> dots{};
			size_t pos = -1;
			do {
				dots.push_back(pos);
				pos = version.find('.', pos + 1);
			} while (pos != std::string::npos && pos != version.length() - 1);
			dots.push_back(version.length());
			for (int i = 0; i < dots.size() - 1; i++) {
				std::string num = version.substr(dots[i] + 1, dots[i + 1] - dots[i] - 1);
				try {
					this->version.push_back(std::stoi(num));
				}
				catch (const std::invalid_argument&) {
					throw version_exception(std::format("Version string contain non-number: \"{}\"", num));
				}
				catch (const std::out_of_range&) {
					throw version_exception(std::format("Version string contain number out of range: \"{}\"", num));
				}
			}
		}

		friend inline bool operator==(VersionInfo v1, VersionInfo v2) {
			for (int i = 0; i < v1.version.size() && i < v2.version.size(); i++)
				if (v1.version[i] != v2.version[i]) return false;
			return true;
		}

		friend std::ostream& operator<<(std::ostream& os, const VersionInfo& version) {
			os << "v";
			for (int i = 0; i < version.version.size(); i++) {
				os << version.version[i];
				if (i != version.version.size() - 1) os << ".";
			}
			return os;
		}
	};
}