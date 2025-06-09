#include "shaderClass.h"

// Reads a text file and returns its contents as a string
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Shader constructor
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// Read vertex and fragment shader files
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);
	//std::cout << "\n" << R"(Vertex code loaded:)" << vertexCode << "\n" << R"(fragment code loaded:)" << "\n" << fragmentCode << "\n";
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create vertex shader object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach vertex shader source code
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the vertex shader
	glCompileShader(vertexShader);

	// Create fragment shader object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach fragment shader source code
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the fragment shader
	glCompileShader(fragmentShader);

	// Create shader program
	ID = glCreateProgram();
	// Attach vertex shader
	glAttachShader(ID, vertexShader);
	// Attach fragment shader
	glAttachShader(ID, fragmentShader);
	// Link the shader program
	glLinkProgram(ID);

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