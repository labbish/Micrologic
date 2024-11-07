#pragma once

#include <map>
#include <string>
#include "nlohmann/json.hpp"
#include "ErrorMsg.h"

namespace labbish {
	namespace Micrologic {
		using json = nlohmann::json;

		inline const json getLanguages() {
			json langs;
			std::ifstream f("lang.json");
			if (!f.good()) {
				message::ErrorMsg() << "Error opening lang.json";
				return json();
			}
			f >> langs;
			f.close();
			return langs;
		}

		const json Languages = getLanguages();

		inline const bool hasLanguage(std::string lang) {
			return Languages.contains(lang);
		}

		inline std::vector<std::string> getKeys() {
			std::vector<std::string> keys;
			for (const auto& item : Languages.items()) {
				keys.push_back(item.key());
			}
			return keys;
		}

		inline const bool hasKey(std::string lang, std::string key) {
			return Languages[lang].contains(key);
		}

		inline const std::string getMessage(std::string lang, std::string key) {
			if (!hasLanguage(lang)) {
				message::ErrorMsg() << "Language not found: " << lang;
				return "";
			}
			if (!hasKey(lang, key)) {
				message::ErrorMsg() << "Message not found: " << lang << "." << key;
				return "";
			}
			return Languages[lang][key].get<std::string>();
		}
	}
}