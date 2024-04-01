#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>

#include "Memory.hpp"
#include "Error.hpp"

namespace memory {
	using namespace std;
	using error::get_error;
	using std::map;

	Memory::Memory(const wstring& process_name, const DWORD& process_id, const HANDLE& process_handle, const map<wstring, ADDRPOINT>& module_list)
		: process_name(process_name), process_id(process_id), process_handle(process_handle), module_list(module_list) {}


	Memory::Memory(const wstring& process_name) : process_name(process_name) {
		process_handle = get_process_handle(process_name);
		if (process_handle == nullptr) {
			cerr << "CAN NOT FOUND GAME PROCESS, ERROR: " << get_error(GetLastError()) << endl;
			exit(0);
		}

		if (!update_module_list()) {
			cerr << "CAN NOT UPDATE MODULE LIST, ERROR: " << get_error(GetLastError()) << endl;
			exit(0);
		}
	}

	Memory::~Memory()
	{
		if (this->process_handle)
			CloseHandle(this->process_handle);
	}

	const map<wstring, ADDRPOINT>& Memory::module_map() const
	{
		return module_list;
	}

	const ADDRPOINT Memory::addr(const wstring& module_name) const
	{
		return module_list.find(module_name)->second;
	}

	const ADDRPOINT Memory::find_value_addr(const ValueOffset& value_offsets) const
	{
#ifdef DEBUG  // DEBUG阶段行为，发布时跳过以提高运行效率
		if (value_offsets.module() == L"") {
			return nullptr;
		}

		if (value_offsets.offset().empty()) {
			return nullptr;
		}

		if (module_list.find(value_offsets.module()) == module_list.end()) {
			return nullptr;
		}
#endif // DEBUG

		char* module_addr = static_cast<char*>(module_list.find(value_offsets.module())->second);
		const vector<ADDRPOINT>& offsets = value_offsets.offset();


		for (auto iter = offsets.begin(); iter != offsets.end(); ++iter) {
			ULL offset = reinterpret_cast<ULL>(*iter);
			if (iter == offsets.end() - 1) {
				return	reinterpret_cast<ADDRPOINT>(module_addr + offset);
			}

			ReadProcessMemory(process_handle, reinterpret_cast<ADDRPOINT>(module_addr + offset), &module_addr, sizeof(module_addr), NULL);
		}

		return nullptr;
	}

	const ADDRPOINT Memory::find_object_addr(const ObjectOffset& object_offsets, const wstring& value_name) const
	{
#ifdef DEBUG  // DEBUG阶段行为，发布时跳过以提高运行效率
		if (object_offsets.module() == L"") {
			return nullptr;
		}

		if (object_offsets.offset().empty()) {
			return nullptr;
		}

		if (module_list.find(object_offsets.module()) == module_list.end()) {
			return nullptr;
		}

		if (object_offsets.values().empty()) {
			return nullptr;
		}

		if (object_offsets.values().find(value_name) == object_offsets.values().end()) {
			return nullptr;
		}
#endif // DEBUG

		ULL module_addr = reinterpret_cast<ULL>(module_list.find(object_offsets.module())->second);
		ULL value_offset = reinterpret_cast<ULL>(object_offsets.values().find(value_name)->second);
		const vector<ADDRPOINT>& offsets = object_offsets.offset();

		for (auto iter = offsets.begin(); iter != offsets.end(); ++iter) {
			if (iter == offsets.end() - 1) {
				return	reinterpret_cast<ADDRPOINT>(module_addr + reinterpret_cast<ULL>(*iter) + value_offset);
			}

			ReadProcessMemory(process_handle, reinterpret_cast<ADDRPOINT>(module_addr + reinterpret_cast<ULL>(*iter)), &module_addr, sizeof(module_addr), NULL);
		}

		return nullptr;
	}

	HANDLE Memory::get_process_handle(const std::wstring& processName) {
		DWORD processId = 0;
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (snapshot != INVALID_HANDLE_VALUE) {
			PROCESSENTRY32 entry{};
			entry.dwSize = sizeof(PROCESSENTRY32);

			if (Process32First(snapshot, &entry)) {
				do {
					if (processName == entry.szExeFile) {
						processId = entry.th32ProcessID;
						break;
					}
				} while (Process32Next(snapshot, &entry));
			}

			CloseHandle(snapshot);
		}

		process_id = processId;
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	}

	bool Memory::update_module_list() {
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);

		if (snapshot != INVALID_HANDLE_VALUE) {
			MODULEENTRY32 module_entry{};
			module_entry.dwSize = sizeof(MODULEENTRY32);

			module_list.emplace(std::make_pair(wstring(L"BASE"), reinterpret_cast<ADDRPOINT>(0x00)));

			if (Module32First(snapshot, &module_entry)) {
				do {
					//std::wcout << wstring(module_entry.szModule) << ":  " << std::hex << reinterpret_cast<ADDRPOINT>(module_entry.modBaseAddr) << std::endl;
					module_list.emplace(std::make_pair(wstring(module_entry.szModule), reinterpret_cast<ADDRPOINT>(module_entry.modBaseAddr)));
				} while (Module32Next(snapshot, &module_entry));
			}

			CloseHandle(snapshot);
		}
		else {
			return false;
		}
		return true;
	}

}
