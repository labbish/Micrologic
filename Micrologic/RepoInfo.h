#pragma once

#include <set>
#include <string>
#include "VersionInfo.h"

namespace labbish::Micrologic {
	struct RepoInfo {
		static const std::string Author;
		static const std::set<std::string> Contributors;
		static const std::string RepoName;
		static const VersionInfo Version;
	};
}