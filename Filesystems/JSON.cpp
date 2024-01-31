#include "JSON.h"

using namespace JSON;

Value::Value(yyjson_val* rValue) : rawValue(rValue) {}

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
bool Value::operator!=(std::nullptr_t ptr) {
	return ptr != rawValue;
}
bool Value::operator==(std::nullptr_t ptr) {
	return ptr == rawValue;
}

Value Value::first() {
	return yyjson_arr_get_first(rawValue);
}
Value Value::last() {
	return yyjson_arr_get_last(rawValue);
}

Document::Document(const char* path, ReadFlag flag) : rawValue(yyjson_read_file(path, flag, NULL, NULL)) {}
Document::Document(std::string path, ReadFlag flag) : rawValue(yyjson_read_file(path.data(), flag, NULL, NULL)) {}
Document::Document(yyjson_doc* rValue) : rawValue(rValue) {};
Document::~Document() { 
	yyjson_doc_free(rawValue); 
};

Value Document::getRoot() { 
	return yyjson_doc_get_root(rawValue); 
}
void Document::writeTo(const char* path, WriteFlag flag) { 
	yyjson_write_file(path, rawValue, flag, NULL, NULL); 
}
const char* Document::write(WriteFlag flag ) {
	return yyjson_write(rawValue, flag, NULL);
}

MutableDocument::MutableDocument() : rawValue(yyjson_mut_doc_new(NULL)) {}
MutableDocument::MutableDocument(yyjson_mut_doc* rValue) : rawValue(rValue) {}
MutableDocument::~MutableDocument() { 
	yyjson_mut_doc_free(rawValue); 
}

MutableDocument Document::copy() {
	return yyjson_doc_mut_copy(rawValue, NULL);
}

void MutableDocument::setRoot(MutableValue& val) { 
	yyjson_mut_doc_set_root(rawValue, val.rawValue); 
}
MutableValue MutableDocument::getRoot() { 
	return yyjson_mut_doc_get_root(rawValue); 
}
void MutableDocument::clearRoot() { 
	yyjson_mut_doc_set_root(rawValue, NULL); 
}

void MutableDocument::writeTo(const char* path, WriteFlag flag) { 
	yyjson_mut_write_file(path, rawValue, flag, NULL, NULL); 
}
const char* MutableDocument::write(WriteFlag flag) {
	return yyjson_mut_write(rawValue, flag, NULL);
}
MutableDocument MutableDocument::copy() {
	return MutableDocument(yyjson_mut_doc_mut_copy(rawValue, NULL));
}

MutableValue::MutableValue(MutableDocument& doc, const char* string) : rawValue(yyjson_mut_str(doc.rawValue, string)) {}
MutableValue::MutableValue(MutableDocument& doc, bool value) : rawValue(yyjson_mut_bool(doc.rawValue, value)) {}
MutableValue::MutableValue(MutableDocument& doc, uint64_t uint) : rawValue(yyjson_mut_uint(doc.rawValue, uint)) {}
MutableValue::MutableValue(MutableDocument& doc, int64_t sint) : rawValue(yyjson_mut_sint(doc.rawValue, sint)) {}
MutableValue::MutableValue(MutableDocument& doc, int integer) : rawValue(yyjson_mut_int(doc.rawValue, integer)) {}
MutableValue::MutableValue(MutableDocument& doc, double duble) : rawValue(yyjson_mut_real(doc.rawValue, duble)) {}
MutableValue::MutableValue(yyjson_mut_val* rValue) : rawValue(rValue) {};

MutableArray::MutableArray(MutableDocument& doc) : MutableValue(yyjson_mut_arr(doc.rawValue)) {}
MutableArray::MutableArray(MutableDocument& doc, bool* vals, size_t count) : MutableValue(yyjson_mut_arr_with_bool(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(MutableDocument& doc, double* vals, size_t count) : MutableValue(yyjson_mut_arr_with_real(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(MutableDocument& doc, int8_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_sint8(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(MutableDocument& doc, int16_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_sint16(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(MutableDocument& doc, int32_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_sint32(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(MutableDocument& doc, int64_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_sint(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(MutableDocument& doc, uint8_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_uint8(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(MutableDocument& doc, uint16_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_uint16(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(MutableDocument& doc, uint32_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_uint32(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(MutableDocument& doc, uint64_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_uint(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(MutableDocument& doc, float* vals, size_t count) : MutableValue(yyjson_mut_arr_with_float(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(MutableDocument& doc, const char** vals, size_t count) : MutableValue(yyjson_mut_arr_with_str(doc.rawValue, vals, count)) {}
MutableArray::MutableArray(yyjson_mut_val* array) : MutableValue(array) {}

bool MutableArray::append(JSON::MutableValue& val) {
	return yyjson_mut_arr_append(rawValue, val.rawValue); 
}
bool MutableArray::append(MutableDocument& document, bool val) {
	return yyjson_mut_arr_add_bool(document.rawValue, rawValue, val); 
}
bool MutableArray::append(MutableDocument& document, uint64_t val) {
	return yyjson_mut_arr_add_uint(document.rawValue, rawValue, val); 
}
bool MutableArray::append(MutableDocument& document, int64_t val) { 
	return yyjson_mut_arr_add_sint(document.rawValue, rawValue, val); 
}
bool MutableArray::append(MutableDocument& document, double val) { 
	return yyjson_mut_arr_add_real(document.rawValue, rawValue, val); 
}
bool MutableArray::append(MutableDocument& document, const char* val) { 
	return yyjson_mut_arr_add_str(document.rawValue, rawValue, val); 
}

MutableArray MutableArray::addArray(MutableDocument& document) { 
	return yyjson_mut_arr_add_arr(document.rawValue, rawValue); 
}
MutableObject MutableArray::addObject(MutableDocument& document) { 
	return yyjson_mut_arr_add_obj(document.rawValue, rawValue); 
}
bool MutableArray::prepend(JSON::MutableValue& val) { 
	return yyjson_mut_arr_prepend(rawValue, val.rawValue); 
}
bool MutableArray::insert(JSON::MutableValue& val, size_t index) { 
	return yyjson_mut_arr_insert(rawValue, val.rawValue, index); 
}
MutableValue MutableArray::replace(JSON::MutableValue& val, size_t index) { 
	return yyjson_mut_arr_replace(rawValue, index, val.rawValue); 
}
MutableValue MutableArray::remove(size_t index) { 
	return yyjson_mut_arr_remove(rawValue, index); 
}
MutableValue MutableArray::removeFirst() { 
	return yyjson_mut_arr_remove_first(rawValue); 
}
MutableValue MutableArray::removeLast() { 
	return yyjson_mut_arr_remove_last(rawValue); 
}
void MutableArray::clear() { 
	yyjson_mut_arr_clear(rawValue); 
}

MutableObject::MutableObject(MutableDocument& doc) : MutableValue(yyjson_mut_obj(doc.rawValue)) {}
MutableObject::MutableObject(yyjson_mut_val* object) : MutableValue(object) {}

void MutableObject::append(JSON::MutableValue& key, JSON::MutableValue& value) { 
	yyjson_mut_obj_add(rawValue, key.rawValue, value.rawValue); 
}

OBJIterator::OBJIterator(Value object) : yyjson_obj_iter(yyjson_obj_iter_with(object.rawValue)) {};

Value OBJIterator::operator[](const char* value) {
	return Value(yyjson_obj_iter_get(this, value));
}

Value OBJIterator::next() {
	return yyjson_obj_iter_next(this);
}