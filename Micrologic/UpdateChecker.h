#pragma once

#include <string>
#include <optional>
#include <functional>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace labbish::Micrologic::UpdateChecker {
	inline size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}
	inline std::optional<std::string> getLatestRelease(const std::string& owner, const std::string& repo,
		const std::function<void(const std::string&, const std::string&)>& errorHandler
		= [](const std::string&, const std::string&) {},
		const std::function<void(const std::string&, const std::string&)>& timeoutHandler
		= [](const std::string&, const std::string&) {}) {
		try {
			CURL* curl;
			CURLcode res;
			std::string readBuffer;
			curl_global_init(CURL_GLOBAL_DEFAULT);
			curl = curl_easy_init();
			if (curl) {
				std::string url = "https://api.github.com/repos/" + owner + "/" + repo + "/releases/latest";
				curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
				curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
				curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
				res = curl_easy_perform(curl);
				curl_easy_cleanup(curl);
				if (res == CURLE_OPERATION_TIMEDOUT) {
					timeoutHandler(owner, repo);
					return std::nullopt;
				}
			}
			curl_global_cleanup();
			return readBuffer;
		}
		catch (...) {
			errorHandler(owner, repo);
			return std::nullopt;
		}
	}
	inline std::optional<std::string> getLatestReleaseName(const std::string& owner, const std::string& repo,
		const std::function<void(const std::string&, const std::string&)>& webErrorHandler
		= [](const std::string&, const std::string&) {},
		const std::function<void(const std::string&, const std::string&)>& jsonErrorHandler
		= [](const std::string&, const std::string&) {},
		const std::function<void(const std::string&, const std::string&)>& timeoutHandler
		= [](const std::string&, const std::string&) {}) {
		try {
			std::optional<std::string> releaseInfo = getLatestRelease(owner, repo, webErrorHandler, timeoutHandler);
			if (releaseInfo == std::nullopt) return std::nullopt;
			nlohmann::json releaseJson = nlohmann::json::parse(*releaseInfo);
			return releaseJson["name"].get<std::string>();
		}
		catch (...) {
			jsonErrorHandler(owner, repo);
			return std::nullopt;
		}
	}
}