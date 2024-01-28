#include "JSON.h"

using namespace JSON;

Document::Document(const char* path, ReadFlag flag) 
	:
	rawValue(yyjson_read_file(path, flag, NULL, NULL))
{
	// Empty
}
Document::Document(std::string path, ReadFlag flag)
	:
	rawValue(yyjson_read_file(path.data(), flag, NULL, NULL)) 
{
	// Empty
}

bool Value::toBool() {
	return yyjson_get_bool(rawValue);
}
int Value::toInt() {
	return yyjson_get_int(rawValue);
}
uint64_t Value::toUint() {
	return yyjson_get_uint(rawValue);
}
size_t Value::getLength() {
	return yyjson_get_len(rawValue);
}
double Value::toDouble() {
	return yyjson_get_num(rawValue);
}
const char* Value::toRaw() {
	return yyjson_get_raw(rawValue);
}
int64_t Value::toSint() {
	return yyjson_get_sint(rawValue);
}
const char* Value::toString() {
	return yyjson_get_str(rawValue);
}

const char* Value::getTypeDesc() {
	return yyjson_get_type_desc(rawValue);
}

Value Value::operator[](int index) {
	return yyjson_arr_get(rawValue, index);
}
Value Value::operator[](const char* value) {
	return yyjson_obj_get(rawValue, value);
}

Value Value::first() {
	return yyjson_arr_get_first(rawValue);
}
Value Value::last() {
	return yyjson_arr_get_last(rawValue);
}

Value JSON::getRoot(Document& document) {
	return Value(yyjson_doc_get_root(document.rawValue));
}

Value OBJIterator::operator[](const char* value) {
	return Value(yyjson_obj_iter_get(this, value));
}

Value OBJIterator::next() {
	return yyjson_obj_iter_next(this);
}