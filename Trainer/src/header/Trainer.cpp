#include "Trainer.hpp"
#include "Memory.hpp"

#include <iostream>

namespace trainer {
	Trainer::Trainer() : game(L"PlantsVsZombies.exe") {}

	const bool Trainer::is_gaming() const {
		if (game.find_value_addr(offsets::sun_offset) == reinterpret_cast<memory::ADDRPOINT>(0x5578)) {
			return false;
		}

		return true;
	}

	void Trainer::infinite_sun() const {
		game.write_value(offsets::sun_offset, 114514);
	}

	void Trainer::none_cd() const {
		const memory::ADDRPOINT first_cd = game.find_value_addr(offsets::cd_offset);
		game.write_addr(first_cd, static_cast<char>(1));
		for (int i = 0; i != 10; ++i) {
			game.write_addr(reinterpret_cast<memory::ADDRPOINT>((char*)first_cd + (static_cast<unsigned long long>(i) * 0x50)), static_cast<char>(1));
		}
	}

	void Trainer::auto_collection() const {
		const DWORD first_col = reinterpret_cast<DWORD>(game.find_value_addr(offsets::collection_offset));
		int _picked = 0;
		int _type = 0;

		for (int i = 0; i != 50; ++i) {
			memory::ADDRPOINT picked = reinterpret_cast<memory::ADDRPOINT>(first_col + i * 0xD8);
			memory::ADDRPOINT type = reinterpret_cast<memory::ADDRPOINT>(first_col + 0x8 + i * 0xD8);
			
			game.read_addr<int>(picked, &_picked);
			game.read_addr<int>(type, &_type);

			if (_picked == 0 && _type < 7) {
				game.write_addr(picked, 1);
			}
		}
	}

	const bool Trainer::run() {
		if (!is_gaming())
			return false;

		infinite_sun();
		none_cd();
		auto_collection();
		return true;
	}
}