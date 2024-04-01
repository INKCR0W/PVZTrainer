#include "Error.hpp"
namespace error {
	const string get_error(unsigned long error_id)
	{
		string error_info;
		auto iter = error_list.find(error_id);
		
		if (iter != error_list.end()) {
			return iter->second;
		}
		else {
			return to_string(error_id);
		}
	}
}