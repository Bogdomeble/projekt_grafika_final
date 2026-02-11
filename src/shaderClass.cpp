#include "shaderClass.h"
#include <iostream>
#include <cstdlib>

// Reads a text file and returns its contents as a string
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (!in.is_open())
	{
		std::cerr << "ERROR: Failed to open file: " << filename << std::endl;
		std::cerr << "Make sure you're running the program from the build/bin/ directory" << std::endl;
		std::cerr << "Expected path: " << filename << std::endl;
		exit(EXIT_FAILURE);
	}
	
	std::string contents;
	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();
	return contents;
}

// Shader constructor
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::cout << "Loading vertex shader: " << vertexFile << std::endl;
	std::cout << "Loading fragment shader: " << fragmentFile << std::endl;
	
	// Read vertex and fragment shader files
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);
	
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create vertex shader object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach vertex shader source code
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the vertex shader
	glCompileShader(vertexShader);

	// Check for vertex shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR: Vertex shader compilation failed: " << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}

	// Create fragment shader object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach fragment shader source code
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the fragment shader
	glCompileShader(fragmentShader);

	// Check for fragment shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR: Fragment shader compilation failed: " << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}

	// Create shader program
	ID = glCreateProgram();
	// Attach vertex shader
	glAttachShader(ID, vertexShader);
	// Attach fragment shader
	glAttachShader(ID, fragmentShader);
	// Link the shader program
	glLinkProgram(ID);

	// Check for linking errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cerr << "ERROR: Shader program linking failed: " << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Shaders loaded and compiled successfully" << std::endl;

	// Delete the vertex and fragment shaders as they're linked into the program now and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}