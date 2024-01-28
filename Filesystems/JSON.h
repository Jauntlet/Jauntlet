#pragma once
#include "../Externals/yyjson.h"
#include <string>

// Wrapper around yyjson to create a more readable format for use in code.
// Meant to preserve the speed of yyjson, however may cause slight performance decreases.
// to use yyjson without the wrapper, use `#include<Jauntlet/Externals/yyjson.h>, and follow its documentation here: https://ibireme.github.io/yyjson/doc/doxygen/html/md_doc__a_p_i.html
namespace JSON {
	typedef yyjson_doc Document;
	typedef yyjson_read_flag ReadFlag;
	//typedef yyjson_val Value;
	
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

	Document* readFile(const std::string& path, ReadFlag flag);
	Document* readFile(const std::string& path);
	Document* readFile(const char* path, ReadFlag flag);
	Document* readFile(const char* path);

	Value getRoot(Document* document);
}