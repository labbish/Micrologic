#pragma once

#include <map>
#include <vector>
#include <string>
#include <random>
#include <variant>
#include <nlohmann/json.hpp>
#include "ErrorMsg.h"

namespace labbish::Micrologic {
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

	namespace Neko {
		using NekoRepeatedMsg = std::vector<std::string>;
		struct NekoOnceMsg :public std::string {};
		struct NekoForeverMsg :public std::string {};
		using NekoMsg = std::variant<NekoRepeatedMsg, NekoOnceMsg, NekoForeverMsg>;
		const std::vector<NekoMsg> NekoMsgs = {
			NekoRepeatedMsg{
				"Meow~",
				"喵呜~",
				"にゃあ～",
			},
			NekoRepeatedMsg{
				"Zagozago~ There's no way you can use me nyaa~",
				"There is no neko!",
				"Go find a galgame or anime if you like nekos so much...",
			},
			NekoRepeatedMsg{
				"Added in v1.2!",
				"With \033[31mA\033[32mN\033[33mS\033[34mI",
				"Have you ever wondered who I am?",
				"Try input command \"credits\"!",
			},
			NekoRepeatedMsg{
				"Stop trying... Meow?",
				"Haven't you feel tired trying a same command for so many times...?",
			},
			NekoForeverMsg("It is evil of you to tease me repeatedly..."),
			NekoOnceMsg("To tell you a secret, there's a command which clears all memory..."),
			NekoForeverMsg("I'm sleeping...Meow..."), //Literally forever
		};
		inline int nekoStage = 0;
		inline void nekoError() {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::string msg;
			int count;
			if (std::holds_alternative<NekoRepeatedMsg>(NekoMsgs[nekoStage])) {
				int size = int(std::get<NekoRepeatedMsg>(NekoMsgs[nekoStage]).size());
				std::uniform_int_distribution<> dis(0, int(size) - 1);
				msg = std::get<NekoRepeatedMsg>(NekoMsgs[nekoStage])[dis(gen)];
				count = int(1.5 * size);
			}
			else if (std::holds_alternative<NekoOnceMsg>(NekoMsgs[nekoStage])) {
				msg = std::get<NekoOnceMsg>(NekoMsgs[nekoStage]);
				count = 0;
			}
			else /*if (std::holds_alternative<NekoForeverMsg>(NekoMsgs[nekoStage]))*/ {
				msg = std::get<NekoForeverMsg>(NekoMsgs[nekoStage]);
				count = 10;
			}
			message::ErrorMsg() << msg;
			std::uniform_int_distribution<> bool_dis(0, count);
			nekoStage = std::min(nekoStage + (bool_dis(gen) == 0), int(NekoMsgs.size() - 1));
		}
	}

	const std::map<std::string, std::string> ErrorMsgs = {
		{ "NO_LANG", "Language not found: \"{}\"" },
		{ "NO_MSG", "Message not found: \"{}\".\"{}\"" },
		{ "NO_KEY", "Key not found: \"{}\"" },
		{ "NO_FILE", "File not found: \"{}\"" },
		{ "NO_HELP", "Help message not found: \"{}\"" },
		{ "NO_CMD", "No such command or incorrect argument count: \"{}\"" },
		{ "NOT_NUM", "Not a num: \"{}\"" },
		{ "NOT_POSITIVE", "Should be positive: {}" },
		{ "NOT_BIT", "Should be bit: \"{}\"" },
		{ "OUT_OF_RANGE", "Number out of range: {}" },
		{ "LINE_COUNT", "Incorrect line count" },
		{ "LINE_TYPE", "Type of line No.{} should be {}" },
		{ "SAME_TYPE", "Type of line No.{} and line No.{} should be the same" },
		{ "CANNOT_OPEN", "Error opening \"{}\"" },
		{ "CANNOT_CREATE", "Error creating \"{}\"" },
		{ "CANNOT_WRITE", "Error writing \"{}\"" },
		{ "SUB_MODE", "Command \"{}\" is unavailable when opening file" },
		{ "SAFE_MODE", "Command \"{}\" is unavailable when opening file in safe mode" },
		{ "SAFE_REDIRECT", "Output redirection to files is unavailable when opening file in safe mode" },
		{ "UPDATE_GET", "Error opening github repo: \"{}/{}\"" },
		{ "UPDATE_PARSE", "Error parsing release info" },
		{ "TIMEOUT", "Update check timeout" },
	};

	template<typename... Args>
	inline const void writeError(std::string error, Args... args) {
		if (ErrorMsgs.find(error) == ErrorMsgs.end()) return;
		std::string format = ErrorMsgs.at(error);
		std::string formatted_msg = std::vformat(format, std::make_format_args(args...));
		message::ErrorMsg() << formatted_msg;
	}
}