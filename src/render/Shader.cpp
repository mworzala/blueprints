#include "Shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath) {

    // Reading vertex and fragment shaders.
    std::string vertexSource, fragmentSource, geometrySource;
    std::ifstream vertexFile, fragmentFile, geometryFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    geometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        std::stringstream vertexFileStream, fragmentFileStream, geometryFileStream;

        // Vertex
        vertexFile.open(vertexPath);
        vertexFileStream << vertexFile.rdbuf();
        vertexFile.close();
        vertexSource = vertexFileStream.str();

        // Fragment
        fragmentFile.open(fragmentPath);
        fragmentFileStream << fragmentFile.rdbuf();
        fragmentFile.close();
        fragmentSource = fragmentFileStream.str();

        if (geometryPath != nullptr) {
            // Geometry
            geometryFile.open(geometryPath);
            geometryFileStream << geometryFile.rdbuf();
            geometryFile.close();
            geometrySource = geometryFileStream.str();
        }
    } catch (std::ifstream::failure e) {
        std::cout << "Error reading shader file!" << std::endl;
    }

    // Compiling shader program
    const char* vertexSourceC = vertexSource.c_str();
    const char* fragmentSourceC = fragmentSource.c_str();
    const char* geometrySourceC = geometrySource.c_str();

    unsigned int vertex, fragment, geometry;
    int success;
    char infoLog[512];

    // Vertex
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSourceC, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "Error compiling vertex shader:\n" << infoLog << std::endl;
    }

    // Fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSourceC, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "Error compiling fragment shader:\n" << infoLog << std::endl;
    }

    if (geometryPath != nullptr) {
        // Geometry
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometrySourceC, nullptr);
        glCompileShader(geometry);
        if (!success) {
            glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
            std::cout << "Error compiling geometry shader:\n" << infoLog << std::endl;
        }
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    if (geometryPath != nullptr)
        glAttachShader(m_id, geometry);
    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        std::cout << "Error linking shader program:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr)
        glDeleteShader(geometry);
}

void Shader::use() const {
    glUseProgram(m_id);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int) value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec4(const std::string &name, glm::vec4 value) const {
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::del() const {
    glDeleteShader(m_id);
}
