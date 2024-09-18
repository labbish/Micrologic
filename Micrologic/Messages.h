﻿#pragma once
#include <string>
#include <map>

const std::vector<std::string> languages = { "en_us", "zh_cn" };

const std::map<std::string, std::string> en_us = {
	{"LINE","No.%d line added.\n"},
	{"LINES","No.%d~%d line added.\n"},
	{"WLINE","No.%d wide line added.\n"},
	{"WLINES","No.%d~%d wide line added.\n"},
	{"BLOCKN","No.%d blockN added. Input: No.%d line. Output: No.%d line.\n"},
	{"BLOCKA","No.%d blockA added. Input: No.%d&%d line. Output: No.%d line.\n"},
	{"BLOCKR","No.%d blockR added. Input: No.%d&%d line. Output: No.%d line.\n"},
	{"BLOCKT","No.%d blockT added. Input: No.%d line. Output: No.%d line.\n"},
	{"BLOCKC","No.%d blockC added. Input: No.%d %d %d %d line. Output: No.%d line.\n"},
	{"BLOCKP","No.%d blockP added. Input: No.%d line. Output: No.%d %d %d %d line.\n"},
	{"CHECK","Value of No.%d line is %s\n"},
	{"CHECKS","Value of lines are: "},
	{"SET","Set value of No.%d line to %d.\n"},
	{"SETW","Set wide value of No.%d line to %d%d%d%d.\n"},
	{"INPUT:","Added line No.%d as input line No.%d.\n"},
	{"INPUT","Inputted value %d to input line No.%d.\n"},
	{"INPUTW","Inputted value %d%d%d%d to input line No.%d.\n"},
	{"OUTPUT:","Added line No.%d as output line No.%d.\n"},
	{"OUTPUT","Value of No.%d output line is %s\n"},
	{"OUTPUTS","Value of output lines are: "},
	{"TICK","Ticked.\n"},
	{"TICKS","Ticked for %d times.\n"},
	{"TICK!","Value of lines after ticking are: "},
	{"TICKS!","Value of lines after ticking are: "},
	{"SPEED","Set speed to %d.\n"},
	{"OPEN","Opened: %s\n\n"},
	{"MOD","Loaded Mod: %s (%s)\n"},
	{"BLOCK1","No.%d block (%s) added. Input: No."},{"BLOCK2","line. Output: No."},{"BLOCK3","line.\n"},
	{"INSPECT","Inspection result of %s No.%d is %d.\n"},
	{"PATH","Current Path: %s\n"},
	{"PATH_SET","Set path to: %s\n"},
	{"CLEAR","Cleared.\n"},
	{"LANG","Set language to %s.\n"},
	{"LANG_LIST","All languages: "},
	{"NEKO","Meow~Have a nice day~\n" },
};

const std::map<std::string, std::string> zh_cn = {
	{"LINE","添加了第%d条线。\n"},
	{"LINES","添加了第%d~%d条线。\n"},
	{"WLINE","添加了第%d条宽线。\n"},
	{"WLINES","添加了第%d~%d条宽线。\n"},
	{"BLOCKN","添加了第%d个N块，输入线号为%d，输出线号为 %d。\n"},
	{"BLOCKA","添加了第%d个A块，输入线号为%d和%d，输出线号为 %d。\n"},
	{"BLOCKR","添加了第%d个R块，输入线号为%d和%d，输出线号为 %d。\n"},
	{"BLOCKT","添加了第%d个T块，输入线号为%d，输出线号为 %d。\n"},
	{"BLOCKC","添加了第%d个C块，输入线号为%d %d %d %d，输出线号为 %d。\n"},
	{"BLOCKP","添加了第%d个C块，输入线号为%d，输出线号为%d %d %d %d。\n"},
	{"CHECK","第%d条线的值是%s\n"},
	{"CHECKS","所有线的值是："},
	{"SET","将第%d条线的值设置为%d。\n"},
	{"SETW","将第%d条线的值设置为%d%d%d%d。\n"},
	{"INPUT:","将第%d条线设置为第%d条输入线。\n"},
	{"INPUT","输入%d到第%d条输入线。\n"},
	{"INPUTW","输入%d%d%d%d到第%d条输入线。\n"},
	{"OUTPUT:","将第%d条线设置为第%d条输出线。\n"},
	{"OUTPUT","第%d条输出线的值是%s\n"},
	{"OUTPUTS","所有输出线的值是："},
	{"TICK","更新1刻。\n"},
	{"TICKS","更新%d刻。\n"},
	{"TICK!","更新后的值是："},
	{"TICKS!","更新后的值是："},
	{"SPEED","将速度设为%d。\n"},
	{"OPEN","打开文件：%s\n\n"},
	{"MOD","加载模块：%s（%s）\n"},
	{"BLOCK1","添加了第%d个块（类型为%s），输入线号为"},{"BLOCK2","，输出线号为"},{"BLOCK3","。\n"},
	{"INSPECT","所有块%s中的第%d个的校验结果是%d。\n"},
	{"PATH","当前目录：%s\n"},
	{"PATH_SET","将目录设为：%s\n"},
	{"CLEAR","已清空。\n"},
	{"LANG","将语言设为%s。\n"},
	{"LANG_LIST","所有语言："},
	{"NEKO","喵呜~今天也是美好的一天喵~\n"},
};

inline const char* getMessage(std::string lang, std::string key) {
	if (lang == "en_us") return en_us.at(key).c_str();
	else if (lang == "zh_cn") return zh_cn.at(key).c_str();
	else return "";
}