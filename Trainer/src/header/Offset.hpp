#pragma once

#include <vector>
#include <map>
#include <string>
#include <Windows.h>


namespace offset {
	using std::vector;
	using std::wstring;
	using std::map;

	typedef void* ADDRPOINT;
	typedef unsigned long long ULL;

	class ValueOffset {
	protected:
		wstring p_module;
		vector<ADDRPOINT> p_offsets;

	public:
		ValueOffset(const ValueOffset&) = delete;
		ValueOffset();
		ValueOffset(const wstring& p_module, const vector<ADDRPOINT>& p_offsets);
		ValueOffset(const wstring& p_module, const vector<ULL>& offsets);
		~ValueOffset() {}

		bool set_module(const wstring& module_name);
		bool set_offset(const vector<ADDRPOINT>& offset_array);

		const wstring& module() const;
		const vector<ADDRPOINT>& offset() const;
	};

	class ObjectOffset : public ValueOffset {
	private:
		map<wstring, ADDRPOINT> p_values;

	public:
		ObjectOffset(const ObjectOffset&) = delete;
		ObjectOffset();
		ObjectOffset(const wstring& p_module, const vector<ADDRPOINT>& p_offsets, const map<wstring, ADDRPOINT>& p_values);
		ObjectOffset(const wstring& p_module, const vector<ULL>& p_offsets, const map<wstring, ULL>& p_values);
		~ObjectOffset() {}

		bool set_values(const map<wstring, ADDRPOINT>& values_offsets);
		//bool add_value(const wstring& module_name, const vector<ADDRPOINT>& offsets);

		const map<wstring, ADDRPOINT>& values() const;
	};
}