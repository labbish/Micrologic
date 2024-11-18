#pragma once

#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include "ErrorMsg.h"

namespace labbish {
	namespace Micrologic {
		using json = nlohmann::json;

		template<typename... Args>
		inline const void writeError(std::string error, Args... args);

		inline const json getLanguages() {
			json langs;
			std::ifstream f("lang.json");
			if (!f.good()) {
				writeError("CANNOT_OPEN", "lang.json");
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
				writeError("NO_LANG", lang);
				return "";
			}
			if (!hasKey(lang, key)) {
				writeError("NO_MSG", lang, key);
				return "";
			}
			return Languages[lang][key].get<std::string>();
		}

		const std::map<std::string, std::string> ErrorMsgs = {
			{ "NO_LANG", "Language not found: \"{}\"" },
			{ "NO_MSG", "Message not found: \"{}\".\"{}\"" },
			{ "NO_KEY", "Key not found: \"{}\"" },
			{ "NO_FILE", "File not found: \"{}\"" },
			{ "NO_CMD", "No such command or incorrect argument count: \"{}\"" },
			{ "NOT_NUM", "Not a num: \"{}\"" },
			{ "NOT_POSITIVE", "Should be positive: {}" },
			{ "NOT_BIT", "Should be bit: \"{}\"" },
			{ "OUT_OF_RANGE", "Number out of range: {}" },
			{ "LINE_COUNT", "Incorrect line count" },
			{ "CANNOT_OPEN", "Error opening {}" },
			{ "CANNOT_WRITE", "Error writing {}" },
			{ "SAFE_MODE", "Command \"{}\" is unavailable when opening file in safe mode" },
			{ "SAFE_REDIRECT", "Output redirection to files is unavailable when opening file in safe mode" },
		};

		template<typename... Args>
		inline const void writeError(std::string error, Args... args) {
			if (ErrorMsgs.find(error) == ErrorMsgs.end()) return;
			std::string format = ErrorMsgs.at(error);
			std::string formatted_msg = std::vformat(format, std::make_format_args(args...));
			message::ErrorMsg() << formatted_msg;
		}
	}
}