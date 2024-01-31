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

		Value(yyjson_val* rValue);

		// returns the contents if the value is a Bool
		// returns NULL if value is not a bool, or is NULL
		inline bool toBool() {
			return yyjson_get_bool(rawValue);
		}
		// returns the contents if the value is an Int
		// returns 0 if value is NULL or not an int
		inline int toInt() {
			return yyjson_get_int(rawValue);
		}
		// returns the contents if the value is an uint64_t
		// returns 0 if value is NULL or not an uint64_t
		inline uint64_t toUint() {
			return yyjson_get_uint(rawValue);
		}
		// returns the contents length (string, array, object)
		// returns 0 if value is NULL or not a valid option
		inline size_t getLength() {
			return yyjson_get_len(rawValue);
		}
		// returns the contents if the value is a double
		// returns 0.0 if value is NULL or not a double
		inline double toDouble() {
			return yyjson_get_num(rawValue);
		}
		// returns the contents if the value is raw
		// returns NULL if value is NULL or not raw
		inline const char* toRaw() {
			return yyjson_get_raw(rawValue);
		}
		// returns the contents if the value is an int64_t
		// returns 0 if value is NULL or not an int64_t
		inline int64_t toSint() {
			return yyjson_get_sint(rawValue);
		}
		// returns the contents if the value is a string
		// returns NULL if value is NULL or not a string
		inline const char* toString() {
			return yyjson_get_str(rawValue);
		}

		// Returns the values JSON type as a readable string.
		// meant for debugging
		inline const char* getTypeDesc() {
			return yyjson_get_type_desc(rawValue);
		}

		// returns the first element in an array.
		// returns NULL if element is not an array.
		inline Value first() {
			return yyjson_arr_get_first(rawValue);
		}
		// returns the last element in an array.
		// returns NULL if element is not an array.
		inline Value last() {
			return yyjson_arr_get_last(rawValue);
		}

		Value operator[](int index);
		Value operator[](const char* value);
		bool operator!=(std::nullptr_t ptr);
		bool operator==(std::nullptr_t ptr);
	};

	class MutableDocument;

	// The main document that holds all of the JSON values and strings.
	class Document {
	public:
		// The raw value of the Document; used for calls directly to yyjson
		yyjson_doc* rawValue;

		Document(yyjson_doc* rValue);
		Document(const char* path, ReadFlag flag = 0);
		Document(std::string path, ReadFlag flag = 0);
		~Document();

		// Returns the root of the Document
		inline Value getRoot() {
			return yyjson_doc_get_root(rawValue);
		}

		// Saves the document to the specified path.
		// Use MutableDocument for an editable JSON document.
		inline void writeTo(const char* path, WriteFlag flag = 0) {
			yyjson_write_file(path, rawValue, flag, NULL, NULL);
		}
		// Saves the document to a string
		// Use MutableDocument for an editable JSON document.
		inline const char* write(WriteFlag flag = 0) {
			return yyjson_write(rawValue, flag, NULL);
		}

		// copies the document as a new mutableDocument.
		inline MutableDocument copy();

		// prevents accidental copying of documents
		Document(const Document& other) = delete;
		Document& operator=(const Document& other) = delete;
		bool operator!=(std::nullptr_t ptr);
		bool operator==(std::nullptr_t ptr);
	};
	
	class MutableValue;

	class MutableDocument {
	public:
		// The raw value of the Document; used for calls directly to yyjson
		yyjson_mut_doc* rawValue;

		MutableDocument();
		MutableDocument(yyjson_mut_doc* rValue);
		~MutableDocument();

		// sets the root of the document to the specified value
		inline void setRoot(MutableValue& val);
		// Returns the root of the Document
		inline MutableValue getRoot();
		// Clears the root of the Document
		inline void clearRoot() {
			yyjson_mut_doc_set_root(rawValue, NULL);
		}

		// Saves the document to the specified path.
		inline void writeTo(const char* path, WriteFlag flag = 0) {
			yyjson_mut_write_file(path, rawValue, flag, NULL, NULL);
		}
		// Saves the document to a string
		inline const char* write(WriteFlag flag = 0) {
			return yyjson_mut_write(rawValue, flag, NULL);
		}

		// copies the document as a new document
		inline MutableDocument copy() {
			return MutableDocument(yyjson_mut_doc_mut_copy(rawValue, NULL));
		}

		// prevents accidental copying of MutableDocuments
		MutableDocument(const MutableDocument& other) = delete;
		MutableDocument& operator=(const MutableDocument& other) = delete;
		bool operator!=(std::nullptr_t ptr);
		bool operator==(std::nullptr_t ptr);
	};

	MutableDocument Document::copy() {
		return yyjson_doc_mut_copy(rawValue, NULL);
	}

	class MutableValue {
	public:
		yyjson_mut_val* rawValue;

		MutableValue(MutableDocument& doc, const char* string);
		MutableValue(MutableDocument& doc, bool value);
		MutableValue(MutableDocument& doc, uint64_t uint);
		MutableValue(MutableDocument& doc, int64_t sint);
		MutableValue(MutableDocument& doc, int integer);
		MutableValue(MutableDocument& doc, double duble);
		MutableValue(yyjson_mut_val* rValue);
	};

	inline MutableValue MutableDocument::getRoot() {
		return yyjson_mut_doc_get_root(rawValue);
	}
	inline void MutableDocument::setRoot(MutableValue& val) {
		yyjson_mut_doc_set_root(rawValue, val.rawValue);
	}

	class MutableObject;

	class MutableArray : public MutableValue {
	public:
		MutableArray(MutableDocument& doc);
		MutableArray(MutableDocument& doc, bool* vals, size_t count);
		MutableArray(MutableDocument& doc, double* vals, size_t count);
		MutableArray(MutableDocument& doc, int8_t* vals, size_t count);
		MutableArray(MutableDocument& doc, int16_t* vals, size_t count);
		MutableArray(MutableDocument& doc, int32_t* vals, size_t count);
		MutableArray(MutableDocument& doc, int64_t* vals, size_t count);
		MutableArray(MutableDocument& doc, uint8_t* vals, size_t count);
		MutableArray(MutableDocument& doc, uint16_t* vals, size_t count);
		MutableArray(MutableDocument& doc, uint32_t* vals, size_t count);
		MutableArray(MutableDocument& doc, uint64_t* vals, size_t count);
		MutableArray(MutableDocument& doc, float* vals, size_t count);
		MutableArray(MutableDocument& doc, const char** vals, size_t count);
		MutableArray(yyjson_mut_val* array);

		// appends another value to the end of the array
		// returns false on error
		inline bool append(JSON::MutableValue& val) {
			return yyjson_mut_arr_append(rawValue, val.rawValue);
		}
		// appends a bool to the end of the array
		// returns false on error
		inline bool append(MutableDocument& document, bool val) {
			return yyjson_mut_arr_add_bool(document.rawValue, rawValue, val);
		}
		// appends an unsigned int to the end of the array
		// returns false on error
		inline bool append(MutableDocument& document, uint64_t val) {
			return yyjson_mut_arr_add_uint(document.rawValue, rawValue, val);
		}
		// appends an int to the end of the array
		// returns false on error
		inline bool append(MutableDocument& document, int64_t val) {
			return yyjson_mut_arr_add_sint(document.rawValue, rawValue, val);
		}
		// appends a double to the end of the array
		// returns false on error
		inline bool append(MutableDocument& document, double val) {
			return yyjson_mut_arr_add_real(document.rawValue, rawValue, val);
		}
		// appends a string to the end of the array
		// returns false on error
		inline bool append(MutableDocument& document, const char* val) {
			return yyjson_mut_arr_add_str(document.rawValue, rawValue, val);
		}

		// appends a new array to the end of the array
		inline MutableArray addArray(MutableDocument& document) {
			return yyjson_mut_arr_add_arr(document.rawValue, rawValue);
		}
		// appends a new object to the end of the array
		inline MutableObject addObject(MutableDocument& document);

		// appends another value to the beginning of the array
		// returns false on error
		inline bool prepend(JSON::MutableValue& val) {
			return yyjson_mut_arr_prepend(rawValue, val.rawValue);
		}
		// appends another value to the specifed index of the array
		// returns false on error or out of bounds
		inline bool insert(JSON::MutableValue& val, size_t index) {
			return yyjson_mut_arr_insert(rawValue, val.rawValue, index);
		}
		
		// replaces a value at the specified index, returns old value.
		inline MutableValue replace(JSON::MutableValue& val, size_t index) {
			return yyjson_mut_arr_replace(rawValue, index, val.rawValue);
		}
		// Removes and returns the value at the specified index.
		inline MutableValue remove(size_t index) {
			return yyjson_mut_arr_remove(rawValue, index);
		}
		// Removes and returns the value at the first index
		inline MutableValue removeFirst() {
			return yyjson_mut_arr_remove_first(rawValue);
		}
		// Removes and returns the value at the end of the array
		inline MutableValue removeLast() {
			return yyjson_mut_arr_remove_last(rawValue);
		}
		// Clears the entire array
		inline void clear() {
			yyjson_mut_arr_clear(rawValue);
		}
	};

	class MutableObject : public MutableValue {
	public:
		MutableObject(MutableDocument& doc);
		MutableObject(MutableDocument& doc, const char** keys, const char** vals, size_t count);
		MutableObject(yyjson_mut_val* object);
		
		// appends a key and a value within the object. Expects a string as the key and any value as the value
		// returns false on error
		inline bool append(JSON::MutableValue& key, JSON::MutableValue& value) {
			return yyjson_mut_obj_add(rawValue, key.rawValue, value.rawValue);
		}
		// appends a key and a value within the object. Expects a string as the key and a bool as the value
		// returns false on error
		inline bool append(MutableDocument& doc, const char* key, bool value) {
			return yyjson_mut_obj_add_bool(doc.rawValue, rawValue, key, value);
		}
		// appends a key and a value within the object. Expects a string as the key and an unsigned int as the value
		// returns false on error
		inline bool append(MutableDocument& doc, const char* key, uint64_t value) {
			return yyjson_mut_obj_add_uint(doc.rawValue, rawValue, key, value);
		}
		// appends a key and a value within the object. Expects a string as the key and an int as the value
		// returns false on error
		inline bool append(MutableDocument& doc, const char* key, int64_t value) {
			return yyjson_mut_obj_add_int(doc.rawValue, rawValue, key, value);
		}
		// appends a key and a value within the object. Expects a string as the key and a double as the value
		// returns false on error
		inline bool append(MutableDocument& doc, const char* key, double value) {
			return yyjson_mut_obj_add_real(doc.rawValue, rawValue, key, value);
		}
		// appends a key and a value within the object. Expects a string as the key and a string as the value
		// returns false on error
		inline bool append(MutableDocument& doc, const char* key, const char* value) {
			return yyjson_mut_obj_add_str(doc.rawValue, rawValue, key, value);
		}

		// appends a new array to the end of the object
		inline MutableArray addArray(MutableDocument& doc, const char* key) {
			return yyjson_mut_obj_add_arr(doc.rawValue, rawValue, key);
		}
		// appends a new object to the end of the object
		inline MutableObject addObject(MutableDocument& doc, const char* key) {
			return yyjson_mut_obj_add_obj(doc.rawValue, rawValue, key);
		}
		
		// removes the key and its value from the object
		// returns false on error
		inline bool remove(JSON::MutableValue& key) {
			return yyjson_mut_obj_remove(rawValue, key.rawValue);
		}
		// Clears the entire object
		inline void clear() {
			yyjson_mut_obj_clear(rawValue);
		}

		// renames an existing key to another key name.
		inline bool renameKey(MutableDocument& doc, const char* key, const char* newKey) {
			return yyjson_mut_obj_rename_key(doc.rawValue, rawValue, key, newKey);
		}
	};

	inline MutableObject MutableArray::addObject(MutableDocument& document) {
		return yyjson_mut_arr_add_obj(document.rawValue, rawValue);
	}

	// Made for faster iteration through an object. Best to use when iterating through a single object many times over.
	class OBJIterator : public yyjson_obj_iter {
	public:
		OBJIterator(Value object);
		
		// get the next value within an object
		inline Value next() {
			return yyjson_obj_iter_next(this);
		}

		Value operator[](const char* value);
	};
}