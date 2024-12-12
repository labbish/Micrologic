#include "RepoInfo.h"

namespace labbish::Micrologic {
	const std::string RepoInfo::Name = "Micrologic";
	const std::string RepoInfo::Owner = "labbish";
	const std::vector<std::wstring> RepoInfo::Contributors = { L"ZZ_404", L"Ichiku", L"石头里的苹果" };
	const VersionInfo RepoInfo::Version = VersionInfo("v2.6");
}