#pragma once

#include <iostream>
#include <ostream>
#include <chrono>

namespace labbish {
	namespace message {
		class TimeMsg :public std::ostream {
		public:
			inline TimeMsg() :std::ostream(&buf) {
				buf.setColor();
			}
			inline ~TimeMsg() {
				buf.resetColor();
			}
		private:
			class TimeBuf : public std::streambuf {
			public:
				inline void setColor() {
					std::cerr << "\033[32m[TIME] Command Time Cost: ";
				}

				inline void resetColor() {
					std::cerr << "ms. \033[0m\n";
				}
				inline int overflow(int c) override {
					if (c != EOF) {
						std::cerr.put(c);
					}
					return c;
				}
			};
			TimeBuf buf;
		};

		class TimeDebugger {
		public:
			std::chrono::steady_clock::time_point start, end;
			inline TimeDebugger() {
				start = end = std::chrono::high_resolution_clock::now();
			}
			inline void flush() {
				start = end = std::chrono::high_resolution_clock::now();
			}
			inline void debug() {
				end = std::chrono::high_resolution_clock::now();
				TimeMsg() << (long double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000;
				start = end;
			}
		};
	}
}