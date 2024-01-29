#pragma once
#include "../Externals/yyjson.h"
#include <string>

// Wrapper around yyjson to create a more readable format for use in code.
// Meant to preserve the speed of yyjson, however may cause slight performance decreases.
// to use yyjson without the wrapper, use `#include<Jauntlet/Externals/yyjson.h>, and follow its documentation here: https://ibireme.github.io/yyjson/doc/doxygen/html/md_doc__a_p_i.html
namespace JSON {
	// Run-time options for reading JSON
	typedef yyjson_read_flag ReadFlag;
	// A JSON value that can be changed
	typedef yyjson_mut_val MutuableValue;
	
	// The main document that holds all of the JSON values and strings.
	class Document {
	public:
		yyjson_doc* rawValue;

		Document(yyjson_doc* rValue) : rawValue(rValue) {};
		Document(const char* path, ReadFlag flag = 0);
		Document(std::string path, ReadFlag flag = 0);
		~Document() { yyjson_doc_free(rawValue); };

		// prevents accidental copying of documents
		Document(const Document& other) = delete;
		Document& operator=(const Document& other) = delete;
	};
	
	// An immutable JSON value
	class Value {
	public:
		yyjson_val* rawValue;

		Value(yyjson_val* rValue) : rawValue(rValue) {};

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
		bool operator!=(std::nullptr_t ptr) {
			return ptr != rawValue;
		}
		bool operator==(std::nullptr_t ptr) {
			return ptr == rawValue;
		}
	};

	// Made for faster iteration through an object. Best to use when iterating through a single object many times over.
	class OBJIterator : public yyjson_obj_iter {
	public:
		OBJIterator(Value object) : yyjson_obj_iter(yyjson_obj_iter_with(object.rawValue)) {};
		
		Value next();

		Value operator[](const char* value);
	};

	Value getRoot(Document& document);
}