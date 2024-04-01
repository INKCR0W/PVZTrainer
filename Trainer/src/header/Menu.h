#pragma once

#include <string>
#include <vector>



namespace menu {
	using std::wstring;
	using std::string;
	using std::vector;

	//class Key {
	//public:
	//	int key_code;
	//	bool pushed;

	//	Key();	// 不提供定义（不允许使用此构造）
	//	Key(int key_code);
	//};

	class Feature {
	public:
		Feature(const string name);

		const string name;
		bool is_on;

		void toggle();
	};

	class Menu {
	private:
		long long menu_point;
		vector<Feature> menu_list;

		bool enter, up, down;
		
		bool should_refresh;
		void refresh();

	public:
		Menu();
		Menu(vector<Feature> menu_list);
		Menu(vector<string> menu_list);
		~Menu() {}

		bool init();
		//bool init(vector<Feature> list);

		void listen();

		const bool feature(int code) const;
		void set_feature(int code, bool state);
	};
};