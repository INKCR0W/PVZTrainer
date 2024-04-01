#include "Memory.hpp"
#include "Offset.hpp"

namespace trainer {
	namespace offsets {
		const static offset::ValueOffset sun_offset(L"BASE", { 0x755E0C, 0x868, 0x5578 });
		const static offset::ValueOffset cd_offset(L"icudt38.dll", { 0x699A4, 0x14, 0x6C, 0x0, 0xF0, 0x70 });
		const static offset::ValueOffset collection_offset(L"icudt38.dll", { 0x69830, 0x10, 0xFC, 0x50 });
	}

	class Trainer {
	public:
		Trainer();
		
		const bool run();

	private:
		memory::Memory game;
		const bool is_gaming() const;
		void infinite_sun() const;
		void none_cd() const;
		void auto_collection() const;
	};
}