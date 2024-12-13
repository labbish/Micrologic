#pragma once

#include <unordered_set>
#include <string>
#include "VersionInfo.h"

namespace labbish::Micrologic {
	struct RepoInfo {
		static const std::string Name;
		static const std::string Owner;
		static const std::vector<std::wstring> Contributors;
		static const VersionInfo Version;
	};
}