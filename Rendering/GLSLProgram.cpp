#include <fstream>
#include <vector>
#include "../Errors.h"
#include "GLSLProgram.h"

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
#ifdef OPENGL
	_programID = glCreateProgram();

	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0) {
		Jauntlet::fatalError("New vertex shader failed to be created!");
	}

	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0) {
		Jauntlet::fatalError("New fragment shader failed to be created!");
	}
#endif

	compileShader(vertexShaderFilePath, _vertexShaderID);
	compileShader(fragmentShaderFilePath, _fragmentShaderID);
}

void GLSLProgram::linkShaders() {
#ifdef OPENGL
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
		Jauntlet::fatalError("Shaders failed to link to program!");
	}

	// Always detach shaders after linking
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
#endif

	isLinked = true;
}

void GLSLProgram::addAttribute(const std::string& attributeName) {
#ifdef OPENGL
	glBindAttribLocation(_programID, _numAttributes, attributeName.c_str());
#endif
	_numAttributes++;
}


int GLSLProgram::getUniformLocation(const std::string& uniformName) const {
#ifdef OPENGL
	GLint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		Jauntlet::fatalError("Uniform " + uniformName + " not found in shader!");
	}
	return location;
#endif
	return 0;
}

void GLSLProgram::use() {
	if (currentProgram != nullptr) { // makes sure only one program is in use
		if (currentProgram->_programID == _programID) {
			return;
		}
		currentProgram->unuse();
	}

#ifdef OPENGL
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; ++i) {
		glEnableVertexAttribArray(i);
	}
#endif

	currentProgram = this;
}
void GLSLProgram::unuse() {
#ifdef OPENGL
	glUseProgram(0);
	
	for (int i = 0; i < _numAttributes; ++i) {
		glDisableVertexAttribArray(i);
	}
#endif

	currentProgram = nullptr; // no program is in use
}

void GLSLProgram::compileShader(const std::string& filePath, unsigned int id) {
	std::ifstream vertexFile(filePath);
	if (vertexFile.fail()) {
		perror(filePath.c_str());
		Jauntlet::fatalError("Failed to open " + filePath);
	}

	std::string fileContents = "";
	std::string line;

	while (std::getline(vertexFile, line)) {
		fileContents += line + "\n";
	}

	vertexFile.close();

#ifdef OPENGL
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
		Jauntlet::fatalError("Shader '" + filePath + "' failed to compile!");
	}
#endif
}
