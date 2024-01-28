#include "JSON.h"

using namespace JSON;

Document* JSON::readFile(const std::string& path, ReadFlag flag) {
	return yyjson_read_file(path.data(), flag, NULL, NULL);
}
Document* JSON::readFile(const std::string& path) {
	return yyjson_read_file(path.data(), 0, NULL, NULL);
}
Document* JSON::readFile(const char* path, ReadFlag flag) {
	return yyjson_read_file(path, flag, NULL, NULL);
}
Document* JSON::readFile(const char* path) {
	return yyjson_read_file(path, 0, NULL, NULL);
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

Value JSON::getRoot(Document* document) {
	return Value(yyjson_doc_get_root(document));
}

Value JSON::OBJIterator::operator[](const char* value) {
	return Value(yyjson_obj_iter_get(this, value));
}

Value OBJIterator::next() {
	return yyjson_obj_iter_next(this);
}