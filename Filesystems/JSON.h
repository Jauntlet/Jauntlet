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

		bool toBool();
		int toInt();
		uint64_t toUint();
		size_t getLength();
		double toDouble();
		const char* toRaw();
		int64_t toSint();
		const char* toString();
		
		const char* getTypeDesc();

		Value operator[](int index);
		Value operator[](const char* value);
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