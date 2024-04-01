#include "Trainer.hpp"
#include "Memory.hpp"
#include "Menu.h"

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

		menu::Menu menu({
		"无限阳光",
		"无CD",
		"自动拾取",
		"作者: INKCR0W",
		});

		std::cout << "初始化成功\n" << "进程PID：" << game.pid() << "\n进程基址：" << std::hex << std::uppercase << game.addr(L"PlantsVsZombies.exe") << "\n等待进入游戏\n\n\n\n" << std::endl;

		while (!is_gaming()) {
			Sleep(50);
		}


		if (!menu.init()) {
			std::cerr << "初始化失败" << std::endl;
			system("pause");
			exit(0);
		}

		while (true) {
			menu.listen();

			if (!is_gaming())
				continue;

			if (menu.feature(0)) {
				infinite_sun();
			}

			if (menu.feature(1)) {
				none_cd();
			}

			if (menu.feature(2)) {
				auto_collection();
			}

			if (menu.feature(3)) {
				system("start https://github.com/INKCR0W/PVZTrainer");
				menu.set_feature(3, false);
			}

			Sleep(10);
		}
		return true;
	}
}