#include <iostream>
#include <Windows.h>

#include "Menu.h"

namespace menu {
	using std::cout;
	using std::endl;
	using std::string;

	//Key::Key(int key_code) : key_code(key_code) {}

	Feature::Feature(const string name) : name(name), is_on(false) {}

	void Feature::toggle()
	{
		is_on = !is_on;
	}

	Menu::Menu() : menu_point(0), menu_list(), enter(false), up(false), down(false), should_refresh(false) {}
	Menu::Menu(vector<Feature> menu_list) : menu_point(0), menu_list(menu_list), enter(false), up(false), down(false), should_refresh(false) {}
	Menu::Menu(vector<string> menu) : menu_point(0), enter(false), up(false), down(false), should_refresh(false) {
		for (auto line : menu) {
			menu_list.push_back(Feature(line));
		}
	}

	bool Menu::init()
	{
		if (menu_list.empty()) {
			return false;
		}

		refresh();
		return true;
	}

	//bool Menu::init(vector<Feature> list)
	//{
	//	menu_list = list;

	//	if (menu_list.empty()) {
	//		return false;
	//	}

	//	refresh();
	//	return true;
	//}

	void Menu::refresh()
	{
		system("cls");
		long long menu_size = menu_list.size();
		int line_count = -1;

		if (menu_point < 0) {
			menu_point = menu_size - 1;
		}
		else if (menu_point > menu_size - 1) {
			menu_point = 0;
		}


		for (Feature& line : menu_list) {
			++line_count;

			if (line_count == menu_point) {
				cout << (line.is_on ? "○" : "×") << line.name << (line.is_on ? ": 开" : ": 关") << endl;
			}
			else {
				cout << line.name << (line.is_on ? ": 开" : ": 关") << endl;
			}
		}
	}

	// 这部分代码我有空会优化，现在实在是懒得再写一个类了
	void Menu::listen()
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {	// 按下回车
			if (!enter) {
				enter = true;
				should_refresh = true;

				menu_list[menu_point].toggle();
			}
		}
		else {
			enter = false;
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000) {		// 按下向上键
			if (!up) {
				up = true;
				should_refresh = true;

				--menu_point;
			}
		}
		else {
			up = false;
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {	// 按下向下键
			if (!down) {
				down = true;
				should_refresh = true;

				++menu_point;
			}
		}
		else {
			down = false;
		}

		if (should_refresh) {
			refresh();
			should_refresh = false;
		}
	}

	const bool Menu::feature(int code) const
	{
		return menu_list[code].is_on;
	}

	void Menu::set_feature(int code, bool state)
	{
		menu_list[code].is_on = state;
		refresh();
	}
}
