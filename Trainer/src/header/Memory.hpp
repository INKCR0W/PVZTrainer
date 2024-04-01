#pragma once

#include <vector>
#include <string>
#include <map>

#include "Offset.hpp"


namespace memory {
	using offset::ValueOffset;
	using offset::ObjectOffset;
	using std::wstring;
	using std::map;
	using std::vector;

	typedef void* ADDRPOINT;
	typedef unsigned long long ULL;

	class Memory {
	private:
		wstring process_name;
		DWORD process_id;
		HANDLE process_handle;

		map<wstring, ADDRPOINT> module_list;

		HANDLE get_process_handle(const std::wstring& processName);
		bool update_module_list();

	public:
		Memory() = delete;
		Memory(const Memory&) = delete;
		Memory(const wstring& process_name);
		Memory(const wstring& process_name, const DWORD& process_id, const HANDLE& process_handle, const map<wstring, ADDRPOINT>& module_list);
		~Memory();

		const map<wstring, ADDRPOINT>& module_map() const;
		const ADDRPOINT addr(const wstring& module_name) const;

		const ADDRPOINT find_value_addr(const ValueOffset& value_offsets) const;
		const ADDRPOINT find_object_addr(const ObjectOffset& object_offsets, const wstring& value_name) const;

		template<typename T>
		bool read_addr(const ADDRPOINT& addr, T* value) const;

		template<typename T>
		bool write_addr(const ADDRPOINT& addr, const T value) const;

		template<typename T>
		bool read_value(const ValueOffset& value_offsets, T* value) const;

		template<typename T>
		bool write_value(const ValueOffset& value_offsets, const T value) const;

		template<typename T>
		bool read_object(const ObjectOffset& object_offsets, const wstring& value_name, T* value) const;

		template<typename T>
		bool write_object(const ObjectOffset& object_offsets, const wstring& value_name, const T value) const;
	};




	template<typename T>
	bool Memory::read_addr(const ADDRPOINT& addr, T* value) const
	{
		return ReadProcessMemory(process_handle, addr, value, sizeof(T), NULL);
	}

	template<typename T>
	bool Memory::write_addr(const ADDRPOINT& addr, const T value) const
	{
		return WriteProcessMemory(process_handle, addr, &value, sizeof(T), NULL);
	}

	template<typename T>
	bool Memory::read_value(const ValueOffset& value_offsets, T* value) const
	{
		const ADDRPOINT addr = find_value_addr(value_offsets);
		if (addr == nullptr) {
			return false;
		}
		return	ReadProcessMemory(process_handle, addr, value, sizeof(T), NULL);
	}

	template<typename T>
	bool Memory::write_value(const ValueOffset& value_offsets, const T value) const
	{
		const ADDRPOINT addr = find_value_addr(value_offsets);
		if (addr == nullptr) {
			return false;
		}
		return	WriteProcessMemory(process_handle, addr, &value, sizeof(T), NULL);
	}

	template<typename T>
	bool Memory::read_object(const ObjectOffset& object_offsets, const wstring& value_name, T* value) const
	{
		const ADDRPOINT addr = find_object_addr(object_offsets, value_name);
		if (addr == nullptr) {
			return false;
		}
		return	ReadProcessMemory(process_handle, addr, value, sizeof(T), NULL);
	}

	template<typename T>
	bool Memory::write_object(const ObjectOffset& object_offsets, const wstring& value_name, const T value) const
	{
		const ADDRPOINT addr = find_object_addr(object_offsets, value_name);
		if (addr == nullptr) {
			return false;
		}
		return	WriteProcessMemory(process_handle, addr, &value, sizeof(T), NULL);
	}
}