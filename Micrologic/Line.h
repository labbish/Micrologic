#pragma once

#include <cstdio>
#include <string>
#include <array>

namespace labbish {
	namespace Micrologic {
		class Line {
		public:
			static constexpr int WlineSize = 4;
			enum TYPE {
				LINE = 0,
				WIDELINE = 1
			};
			TYPE mode;
			bool nextValue;
			bool value;
			std::array<bool, WlineSize> nextWideValue;
			std::array<bool, WlineSize> wideValue;
			Line(TYPE mode = LINE);
			Line(const Line&);
			Line& operator=(const Line&);
			Line(Line&&) noexcept;
			Line& operator=(Line&&) noexcept;
			void set(bool value);
			void set(std::array<bool, WlineSize> wideValue);
			void flush();
			std::string checkValue();
		};
	}
}
