#pragma once

#include <iostream>
#include <ostream>

namespace labbish {
	namespace message {
		class ErrorMsg :public std::ostream {
		public:
			inline ErrorMsg() :std::ostream(&buf) {
				buf.setColor();
			}
			inline ~ErrorMsg() {
				buf.resetColor();
			}
		private:
			class ErrorBuf : public std::streambuf {
			public:
				inline void setColor() {
					std::cerr << "\033[31m[ERROR] ";
				}

				inline void resetColor() {
					std::cerr << "\033[0m\n";
				}
				inline int overflow(int c) override {
					if (c != EOF) {
						std::cerr.put(c);
					}
					return c;
				}
			};
			ErrorBuf buf;
		};
	}
}