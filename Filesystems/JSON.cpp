#include "JSON.h"

using namespace JSON;

Value::Value(yyjson_val* rValue) : rawValue(rValue) {}

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

Document::Document(const char* path, ReadFlag flag) : rawValue(yyjson_read_file(path, flag, NULL, NULL)) {}
Document::Document(std::string path, ReadFlag flag) : rawValue(yyjson_read_file(path.data(), flag, NULL, NULL)) {}
Document::Document(yyjson_doc* rValue) : rawValue(rValue) {};
Document::~Document() { 
	yyjson_doc_free(rawValue); 
};

bool Document::operator!=(std::nullptr_t ptr) {
	return ptr != rawValue;
}
bool Document::operator==(std::nullptr_t ptr) {
	return ptr == rawValue;
}

MutableDocument::MutableDocument() : rawValue(yyjson_mut_doc_new(NULL)) {}
MutableDocument::MutableDocument(yyjson_mut_doc* rValue) : rawValue(rValue) {}
MutableDocument::~MutableDocument() { 
	yyjson_mut_doc_free(rawValue); 
}

bool MutableDocument::operator!=(std::nullptr_t ptr) {
	return ptr != rawValue;
}
bool MutableDocument::operator==(std::nullptr_t ptr) {
	return ptr == rawValue;
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

MutableObject::MutableObject(MutableDocument& doc) : MutableValue(yyjson_mut_obj(doc.rawValue)) {}
MutableObject::MutableObject(MutableDocument& doc, const char** keys, const char** vals, size_t count) : MutableValue(yyjson_mut_obj_with_str(doc.rawValue, keys, vals, count)) {}
MutableObject::MutableObject(yyjson_mut_val* object) : MutableValue(object) {}

OBJIterator::OBJIterator(Value object) : yyjson_obj_iter(yyjson_obj_iter_with(object.rawValue)) {};

Value OBJIterator::operator[](const char* value) {
	return Value(yyjson_obj_iter_get(this, value));
}