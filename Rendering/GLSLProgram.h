/* Purpose: Links and manages a vertex and fragment shader which is integral to rendering anything. 
 * also tracks the currently active GLSLProgram and makes sure only one is active at once.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once

#include <GL/glew.h>
#include <string>

namespace Jauntlet {
// GLSLProgram is the main shader used by the program, using a vertex and fragment shader
class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();
	// creates both a vertex and fragment shader.
	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	// links the two shaders to the program
	void linkShaders();
	
	// Checks if the Shaders are already linked
	bool isLinked = false;

	// allows you to add properties to the shaders that you can use in GLSL
	void addAttribute(const std::string& attributeName);

	// returns the GLint of the location of a uniform
	GLint getUniformLocation(const std::string& uniformName) const;

	// returns the id of the program
	GLuint getId() const { return _programID; };

	// enable the use of the program
	void use();
	// disable the use of the program
	void unuse();
	
	static GLSLProgram* currentProgram;
private:

	// function to compile an individual shader
	void compileShader(const std::string& filePath, GLuint id);

	int _numAttributes = 0;

	GLuint _programID = 0;
	GLuint _vertexShaderID = 0;
	GLuint _fragmentShaderID = 0;
};
}
