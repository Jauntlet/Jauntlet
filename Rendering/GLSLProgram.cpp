#include <fstream>
#include <vector>
#include "../Errors.h"
#include "GLSLProgram.h"

using namespace Jauntlet;

// initialize static member
GLSLProgram* GLSLProgram::currentProgram = nullptr;

GLSLProgram::GLSLProgram() {
	// Empty
}
GLSLProgram::~GLSLProgram() {
	if (GLSLProgram::currentProgram == this) {
		unuse();
	}
}

void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
	_programID = glCreateProgram();

	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0) {
		fatalError("New vertex shader failed to be created!");
	}

	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0) {
		fatalError("New fragment shader failed to be created!");
	}

	compileShader(vertexShaderFilePath, _vertexShaderID);
	compileShader(fragmentShaderFilePath, _fragmentShaderID);
}

void GLSLProgram::linkShaders() {

	// Attach and link shaders to program
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);
	glLinkProgram(_programID);

	// Error checking 
	GLint testLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&testLinked);
	if (testLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		glDeleteProgram(_programID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shaders failed to link to program!");
	}

	// Always detach shaders after linking
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);

	isLinked = true;
}

void GLSLProgram::addAttribute(const std::string& attributeName) {
	glBindAttribLocation(_programID, _numAttributes, attributeName.c_str());
	_numAttributes++;
}


	GLint location = glGetUniformLocation(_programID, uniformName.c_str());
int GLSLProgram::getUniformLocation(const std::string& uniformName) const {
	if (location == GL_INVALID_INDEX) {
		fatalError("Uniform " + uniformName + " not found in shader!");
	}
	return location;
}

void GLSLProgram::use() {
	if (currentProgram != nullptr) { // makes sure only one program is in use
		if (currentProgram->_programID == _programID) {
			return;
		}
		currentProgram->unuse();
	}

	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; ++i) {
		glEnableVertexAttribArray(i);
	}

	currentProgram = this;
}
void GLSLProgram::unuse() {
	glUseProgram(0);
	
	for (int i = 0; i < _numAttributes; ++i) {
		glDisableVertexAttribArray(i);
	}

	currentProgram = nullptr; // no program is in use
}

void GLSLProgram::compileShader(const std::string& filePath, unsigned int id) {
	std::ifstream vertexFile(filePath);
	if (vertexFile.fail()) {
		perror(filePath.c_str());
		fatalError("Failed to open " + filePath);
	}

	std::string fileContents = "";
	std::string line;

	while (std::getline(vertexFile, line)) {
		fileContents += line + "\n";
	}

	vertexFile.close();

	const char* contentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);

	glCompileShader(id);

	// Error checking
	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		// Gather error
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		// process error and delete shader
		glDeleteShader(id);
		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader '" + filePath + "' failed to compile!");
	}
}
