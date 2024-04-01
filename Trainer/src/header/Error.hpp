#pragma once

#include <map>
#include <string>

namespace error {
	using namespace std;

	static const map<unsigned long, string> error_list = {
		{5, "�ܾ����ʡ�"},
		{6, "�����Ч��"},
		{87, "�������󣨴��������Ϸû����������"},
	};

	const string get_error(unsigned long error_id);
}