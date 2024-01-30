#pragma once
#include "../Externals/yyjson.h"
#include <string>

// Wrapper around yyjson to create a more readable format for use in code.
// Meant to preserve the speed of yyjson, however may cause slight performance decreases.
// to use yyjson without the wrapper, use `#include<Jauntlet/Externals/yyjson.h>, and follow its documentation here: https://ibireme.github.io/yyjson/doc/doxygen/html/md_doc__a_p_i.html
namespace JSON {
	// Run-time options for reading JSON
	typedef yyjson_read_flag ReadFlag;
	typedef yyjson_write_flag WriteFlag;
	
	// An immutable JSON value
	class Value {
	public:
		yyjson_val* rawValue;

		Value(yyjson_val* rValue) : rawValue(rValue) {}

		// returns the contents if the value is a Bool
		// returns NULL if value is not a bool, or is NULL
		bool toBool() {
			return yyjson_get_bool(rawValue);
		}
		// returns the contents if the value is an Int
		// returns 0 if value is NULL or not an int
		int toInt() {
			return yyjson_get_int(rawValue);
		}
		// returns the contents if the value is an uint64_t
		// returns 0 if value is NULL or not an uint64_t
		uint64_t toUint() {
			return yyjson_get_uint(rawValue);
		}
		// returns the contents length (string, array, object)
		// returns 0 if value is NULL or not a valid option
		size_t getLength() {
			return yyjson_get_len(rawValue);
		}
		// returns the contents if the value is a double
		// returns 0.0 if value is NULL or not a double
		double toDouble() {
			return yyjson_get_num(rawValue);
		}
		// returns the contents if the value is raw
		// returns NULL if value is NULL or not raw
		const char* toRaw() {
			return yyjson_get_raw(rawValue);
		}
		// returns the contents if the value is an int64_t
		// returns 0 if value is NULL or not an int64_t
		int64_t toSint() {
			return yyjson_get_sint(rawValue);
		}
		// returns the contents if the value is a string
		// returns NULL if value is NULL or not a string
		const char* toString() {
			return yyjson_get_str(rawValue);
		}

		// Returns the values JSON type as a readable string.
		// meant for debugging
		const char* getTypeDesc() {
			return yyjson_get_type_desc(rawValue);
		}

		// returns the first element in an array.
		// returns NULL if element is not an array.
		Value first() {
			return yyjson_arr_get_first(rawValue);
		}
		// returns the last element in an array.
		// returns NULL if element is not an array.
		Value last() {
			return yyjson_arr_get_last(rawValue);
		}

		Value operator[](int index) {
			return yyjson_arr_get(rawValue, index);
		}
		Value operator[](const char* value) {
			return yyjson_obj_get(rawValue, value);
		}
		bool operator!=(std::nullptr_t ptr) {
			return ptr != rawValue;
		}
		bool operator==(std::nullptr_t ptr) {
			return ptr == rawValue;
		}
	};

	// The main document that holds all of the JSON values and strings.
	class Document {
	public:
		yyjson_doc* rawValue;

		Document(yyjson_doc* rValue) : rawValue(rValue) {}
		Document(const char* path, ReadFlag flag = 0) : rawValue(yyjson_read_file(path, flag, NULL, NULL)) {};
		Document(std::string path, ReadFlag flag = 0) : rawValue(yyjson_read_file(path.data(), flag, NULL, NULL)) {};
		~Document() { yyjson_doc_free(rawValue); }

		void writeTo(const char* path, WriteFlag flag = 0) { 
			yyjson_write_file(path, rawValue, flag, NULL, NULL);
		}
		Value getRoot() {
			return Value(yyjson_doc_get_root(rawValue));
		}

		// prevents accidental copying of documents
		Document(const Document& other) = delete;
		Document& operator=(const Document& other) = delete;
	};
	
	class MutableDocument {
	public:
		yyjson_mut_doc* rawValue;

		MutableDocument() {
			rawValue = yyjson_mut_doc_new(NULL);
		}
		~MutableDocument() { 
			yyjson_mut_doc_free(rawValue);
		}

		void writeTo(const char* path, WriteFlag flag = 0) { 
			yyjson_mut_write_file(path, rawValue, flag, NULL, NULL);
		}

		// prevents accidental copying of MutableDocuments
		MutableDocument(const MutableDocument& other) = delete;
		MutableDocument& operator=(const MutableDocument& other) = delete;
	};

	class MutableValue {
	public:
		yyjson_mut_val* rawValue;

		MutableValue(MutableDocument& doc, const char* string) { 
			rawValue = yyjson_mut_str(doc.rawValue, string);
		}
		MutableValue(MutableDocument& doc, int integer) { 
			rawValue = yyjson_mut_int(doc.rawValue, integer);
		}
		void setDocRoot(MutableDocument& doc) { 
			yyjson_mut_doc_set_root(doc.rawValue, rawValue);
		}
	};

	class MutableArray {
	public:
		yyjson_mut_val* rawValue;

		MutableArray(MutableDocument& doc) { 
			rawValue = yyjson_mut_arr(doc.rawValue);
		}
		void append(JSON::MutableValue& val) { 
			yyjson_mut_arr_append(rawValue, val.rawValue);
		}
		void setDocRoot(MutableDocument& doc) { 
			yyjson_mut_doc_set_root(doc.rawValue, rawValue);
		}
	};

	class MutableObject {
	public:
		yyjson_mut_val* rawValue;
		
		MutableObject(MutableDocument& doc) { 
			rawValue = yyjson_mut_obj(doc.rawValue); 
		}
		void setDocRoot(MutableDocument& doc) {
			yyjson_mut_doc_set_root(doc.rawValue, rawValue); 
		}
	};

	// Made for faster iteration through an object. Best to use when iterating through a single object many times over.
	class OBJIterator : public yyjson_obj_iter {
	public:
		OBJIterator(Value object) : yyjson_obj_iter(yyjson_obj_iter_with(object.rawValue)) {}
		
		Value next() {
			return yyjson_obj_iter_next(this);
		}

		Value operator[](const char* value) {
			return Value(yyjson_obj_iter_get(this, value));
		}
	};
}