#pragma once
#include<string>
#include<GL/glew.h>

// GLSLProgram is the main shader used by the program, using a vertex and fragment shader


namespace Jauntlet {
	class GLSLProgram
	{
	public:
		// constructor
		GLSLProgram();

		// creates both a vertex and fragment shader, as you need both.
		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		// links the two shaders to the program
		void linkShaders();

		// allows you to add properties to the shaders that you can use in GLSL
		void addAttribute(const std::string& attributeName);

		// returns the GLint of the location of a uniform
		GLint getUniformLocation(const std::string& uniformName);

		// functions to enable / disable the use of the program
		void use();
		void unuse();
	private:

		// function to compile an individual shader
		void compileShader(const std::string& filePath, GLuint id);

		int _numAttributes;

		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};
}
