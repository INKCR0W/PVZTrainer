#pragma once

#include <map>
#include <string>

namespace error {
	using namespace std;

	static const map<unsigned long, string> error_list = {
		{5, "拒绝访问。"},
		{6, "句柄无效。"},
		{87, "参数错误（大概率是游戏没有启动）。"},
	};

	const string get_error(unsigned long error_id);
}