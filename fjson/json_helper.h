// Copyright (c) 2016, NetEase Inc. All rights reserved.
//
// Author: šýżÍĚěŃÄdudu <dingyongfeng@yixin.im>
// Date: 2016/5/23
//
#ifndef _JSON_HELPER_
#define _JSON_HELPER_
#include "fjson.h"
#ifdef __cplusplus
extern "C"
{
#endif
	json_value * json_value_find(const json_value * parent, const char * name);
	int json_value_find_as_bool(const json_value * parent, const char * name);
	json_int_t json_value_find_as_int(const json_value * parent, const char * name);
	double json_value_find_as_double(const json_value * parent, const char * name);
	const json_char *json_value_find_as_string(const json_value * parent, const char * name);
	json_value* json_value_array_copy(const json_value * val);
	json_value* json_value_object_copy(const json_value * v);

	void json_value_object_replace(const json_value * parent, const char * name, json_value* new_val);

	//replace or push;
	void json_value_object_replace2(const json_value * parent, const char * name, json_value* new_val);
#ifdef __cplusplus
}
#endif
#endif //_JSON_HELPER_
