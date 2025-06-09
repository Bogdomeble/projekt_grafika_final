    #ifndef SHADER_CLASS_H
    #define SHADER_CLASS_H

    #include <glad/glad.h>
    #include <string>
    #include <fstream>
    #include <sstream>
    #include <iostream>
    #include <cerrno>

    // Function to read the contents of a file into a string
    std::string get_file_contents(const char* filename);

    class Shader
    {
    public:
        GLuint ID; // Shader program ID
        // Constructor that takes vertex and fragment shader file paths
        Shader(const char* vertexFile, const char* fragmentFile);

        // Activates the shader program
        void Activate();
        // Deletes the shader program
        void Delete();
    };
    #endif