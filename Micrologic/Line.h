#pragma once

#include <cstdio>
#include <string>
#include <array>

namespace labbish::Micrologic {
	class Line {
	public:
		enum TYPE {
			LINE = 0,
			WIDELINE = 1
		};
		TYPE mode;
		bool nextValue;
		bool value;
		std::array<bool, 4> nextWideValue;
		std::array<bool, 4> wideValue;
		Line(TYPE mode = LINE);
		Line(const Line&);
		Line& operator=(const Line&);
		Line(Line&&) noexcept;
		Line& operator=(Line&&) noexcept;
		void set(bool value);
		void set(std::array<bool, 4> wideValue);
		void flush();
		std::string checkValue();
	};

	inline std::string to_string(Line::TYPE type) {
		if (type == Line::LINE) return "line";
		else return "wide line";
	}
}
