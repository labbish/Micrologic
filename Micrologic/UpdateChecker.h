#pragma once

#include <curl/curl.h>
#include <string>

namespace labbish::Micrologic::UpdateChecker {
	inline size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}
	inline std::string getLatestRelease(const std::string& owner, const std::string& repo) {
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
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
		}
		curl_global_cleanup();
		return readBuffer;
	}
}