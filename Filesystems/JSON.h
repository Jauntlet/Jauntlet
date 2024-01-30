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
		bool toBool();
		// returns the contents if the value is an Int
		// returns 0 if value is NULL or not an int
		int toInt();
		// returns the contents if the value is an uint64_t
		// returns 0 if value is NULL or not an uint64_t
		uint64_t toUint();
		// returns the contents length (string, array, object)
		// returns 0 if value is NULL or not a valid option
		size_t getLength();
		// returns the contents if the value is a double
		// returns 0.0 if value is NULL or not a double
		double toDouble();
		// returns the contents if the value is raw
		// returns NULL if value is NULL or not raw
		const char* toRaw();
		// returns the contents if the value is an int64_t
		// returns 0 if value is NULL or not an int64_t
		int64_t toSint();
		// returns the contents if the value is a string
		// returns NULL if value is NULL or not a string
		const char* toString();

		// Returns the values JSON type as a readable string.
		// meant for debugging
		const char* getTypeDesc();

		// returns the first element in an array.
		// returns NULL if element is not an array.
		Value first();
		// returns the last element in an array.
		// returns NULL if element is not an array.
		Value last();

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

		Document(yyjson_doc* rValue) : rawValue(rValue) {};
		Document(const char* path, ReadFlag flag = 0);
		Document(std::string path, ReadFlag flag = 0);
		~Document() { yyjson_doc_free(rawValue); };

		// Returns the root of the Document
		Value getRoot();

		// Saves the document to the specified path.
		// Use MutableDocument for an editable JSON document.
		void writeTo(const char* path, WriteFlag flag = 0);
		// Saves the document to a string
		// Use MutableDocument for an editable JSON document.
		const char* write(WriteFlag flag = 0);

		// copies the document as a new mutableDocument.
		MutableDocument copy();

		// prevents accidental copying of documents
		Document(const Document& other) = delete;
		Document& operator=(const Document& other) = delete;
	};
	
	class MutableValue;

	class MutableDocument {
	public:
		// The raw value of the Document; used for calls directly to yyjson
		yyjson_mut_doc* rawValue;

		MutableDocument() { rawValue = yyjson_mut_doc_new(NULL); }
		~MutableDocument() { yyjson_mut_doc_free(rawValue); }
		MutableDocument(yyjson_mut_doc* rValue) : rawValue(rValue) {}

		void setRoot(MutableValue& val);
		// Returns the root of the Document
		MutableValue getRoot();
		// Clears the root of the Document
		void clearRoot();

		// Saves the document to the specified path.
		void writeTo(const char* path, WriteFlag flag = 0);
		// Saves the document to a string
		const char* write(WriteFlag flag = 0);

		// copies the document as a new document
		MutableDocument copy();

		// prevents accidental copying of MutableDocuments
		MutableDocument(const MutableDocument& other) = delete;
		MutableDocument& operator=(const MutableDocument& other) = delete;
	};

	class MutableValue {
	public:
		yyjson_mut_val* rawValue;

		MutableValue(MutableDocument& doc, const char* string) { rawValue = yyjson_mut_str(doc.rawValue, string); }
		MutableValue(MutableDocument& doc, bool value) { rawValue = yyjson_mut_bool(doc.rawValue, value);}
		MutableValue(MutableDocument& doc, uint64_t uint) { rawValue = yyjson_mut_uint(doc.rawValue, uint); }
		MutableValue(MutableDocument& doc, int64_t sint) { rawValue = yyjson_mut_sint(doc.rawValue, sint); }
		MutableValue(MutableDocument& doc, int integer) { rawValue = yyjson_mut_int(doc.rawValue, integer); }
		MutableValue(MutableDocument& doc, double duble) { rawValue = yyjson_mut_real(doc.rawValue, duble); }

		MutableValue(yyjson_mut_val* rValue) : rawValue(rValue) {};
	};

	class MutableObject;

	class MutableArray : public MutableValue {
	public:
		MutableArray(MutableDocument& doc) : MutableValue(yyjson_mut_arr(doc.rawValue)) {}
		MutableArray(MutableDocument& doc, bool* vals, size_t count) : MutableValue(yyjson_mut_arr_with_bool(doc.rawValue, vals, count)) {}
		MutableArray(MutableDocument& doc, int64_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_sint(doc.rawValue, vals, count)) {}
		MutableArray(MutableDocument& doc, uint64_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_uint(doc.rawValue, vals, count)) {}
		MutableArray(MutableDocument& doc, double* vals, size_t count) : MutableValue(yyjson_mut_arr_with_real(doc.rawValue, vals, count)) {}
		MutableArray(MutableDocument& doc, int8_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_sint8(doc.rawValue, vals, count)) {}
		MutableArray(MutableDocument& doc, int16_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_sint16(doc.rawValue, vals, count)) {}
		MutableArray(MutableDocument& doc, int32_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_sint32(doc.rawValue, vals, count)) {}
		MutableArray(MutableDocument& doc, uint8_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_uint8(doc.rawValue, vals, count)) {}
		MutableArray(MutableDocument& doc, uint16_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_uint16(doc.rawValue, vals, count)) {}
		MutableArray(MutableDocument& doc, uint32_t* vals, size_t count) : MutableValue(yyjson_mut_arr_with_uint32(doc.rawValue, vals, count)) {}
		MutableArray(MutableDocument& doc, float* vals, size_t count) : MutableValue(yyjson_mut_arr_with_float(doc.rawValue, vals, count)) {}
		MutableArray(MutableDocument& doc, const char** vals, size_t count) : MutableValue(yyjson_mut_arr_with_str(doc.rawValue, vals, count)) {}
		MutableArray(yyjson_mut_val* array) : MutableValue(array) {}

		// appends another value to the end of the array
		// returns false on error
		bool append(JSON::MutableValue& val) { return yyjson_mut_arr_append(rawValue, val.rawValue); }
		// appends a new array to the end of the array
		MutableArray appendArray(MutableDocument& document) { return yyjson_mut_arr_add_arr(document.rawValue, rawValue); }
		MutableObject appendObject(MutableDocument& document);

		// appends another value to the beginning of the array
		// returns false on error
		bool prepend(JSON::MutableValue& val) { return yyjson_mut_arr_prepend(rawValue, val.rawValue); }
		// appends another value to the specifed index of the array
		// returns false on error or out of bounds
		bool insert(JSON::MutableValue& val, size_t index) { return yyjson_mut_arr_insert(rawValue, val.rawValue, index); }
		
		// replaces a value at the specified index, returns old value.
		MutableValue replace(JSON::MutableValue& val, size_t index) { return yyjson_mut_arr_replace(rawValue, index, val.rawValue); }
		// Removes and returns the value at the specified index.
		MutableValue remove(size_t index) { return yyjson_mut_arr_remove(rawValue, index); }
		// Removes and returns the value at the first index
		MutableValue removeFirst() { return yyjson_mut_arr_remove_first(rawValue); }
		// Removes and returns the value at the end of the array
		MutableValue removeLast() { return yyjson_mut_arr_remove_last(rawValue); }
		// Clears the entire array
		void clear() { yyjson_mut_arr_clear(rawValue); }
	};

	class MutableObject : public MutableValue {
	public:
		MutableObject(MutableDocument& doc) : MutableValue(yyjson_mut_obj(doc.rawValue)) {}
		MutableObject(yyjson_mut_val* object) : MutableValue(object) {}
		// appends a key and a value within the object. Expects a string as the key and any value as the value
		void append(JSON::MutableValue& key, JSON::MutableValue& value) { yyjson_mut_obj_add(rawValue, key.rawValue, value.rawValue); }
	};

	// Made for faster iteration through an object. Best to use when iterating through a single object many times over.
	class OBJIterator : public yyjson_obj_iter {
	public:
		OBJIterator(Value object) : yyjson_obj_iter(yyjson_obj_iter_with(object.rawValue)) {};
		// get the next value within an object
		Value next();

		Value operator[](const char* value);
	};
}