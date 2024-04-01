#include "Offset.hpp"


namespace offset {
	using namespace std;

	ValueOffset::ValueOffset() : p_module(), p_offsets() {}
	ValueOffset::ValueOffset(const wstring& p_module, const vector<ADDRPOINT>& p_offsets) : p_module(p_module), p_offsets(p_offsets) {}
	ValueOffset::ValueOffset(const wstring& p_module, const vector<ULL>& offsets) : p_module(p_module), p_offsets() {
		for (const auto& iter : offsets) {
			p_offsets.push_back(reinterpret_cast<ADDRPOINT>(iter));
		}
	}

	bool ValueOffset::set_module(const wstring& module_name) {
		p_module = module_name;
		return p_module == L"" ? false : true;
	}

	bool ValueOffset::set_offset(const vector<ADDRPOINT>& offset_array) {
		p_offsets = offset_array;
		return p_offsets.size() == 0 ? false : true;
	}

	const wstring& ValueOffset::module() const {
		return p_module;
	}

	const vector<ADDRPOINT>& ValueOffset::offset() const {
		return p_offsets;
	}



	ObjectOffset::ObjectOffset() : ValueOffset(), p_values() {}
	ObjectOffset::ObjectOffset(const wstring& p_module, const vector<ADDRPOINT>& p_offsets, const map<wstring, ADDRPOINT>& p_values) : ValueOffset(p_module, p_offsets), p_values(p_values) {}
	ObjectOffset::ObjectOffset(const wstring& p_module, const vector<ULL>& p_offsets, const map<wstring, ULL>& values) : ValueOffset(p_module, p_offsets), p_values() {
		for (const auto& iter : values) {
			p_values.emplace(std::make_pair(iter.first, reinterpret_cast<ADDRPOINT>(iter.second)));
		}
	}

	bool ObjectOffset::set_values(const map<wstring, ADDRPOINT>& values_offsets) {
		// clear��������map�ﱣ������ж��󣬵����ͷ�map����ռ�õ��ڴ棬������ٴ��������ʱ��Ч��
		// ���Ҫ��ȫ�ͷſ���ʹ��erase����
		p_values.clear();
		p_values = values_offsets;
		return p_values.size() == 0 ? false : true;
	}

//	bool ObjectOffset::add_value(const wstring& module_name, const vector<ADDRPOINT>& offsets)
//	{
//#ifdef DEBUG  // DEBUG�׶���Ϊ������ʱ�������������Ч��
//		// ���Ѵ�����ֱ�ӷ���false
//		if (p_values.find(module_name) != p_values.end())
//			return false;
//#endif // DEBUG
//
//		p_values.emplace(module_name, offsets);
//		return true;
//	}

	const map<wstring, ADDRPOINT>& ObjectOffset::values() const {
		return p_values;
	}
}