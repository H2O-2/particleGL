#include "shaderParser.hpp"

#include "consoleMsg/consoleMsg.hpp"

ShaderParser::ShaderParser() : vertexShaderPath("") {}

ShaderParser::ShaderParser(const string &vertexShaderPath, const string &fragShaderPath, const string &geometryShaderPath)
    : vertexShaderPath(vertexShaderPath), fragShaderPath(fragShaderPath), geometryShaderPath(geometryShaderPath) {
}

ShaderParser::~ShaderParser() {
    if (!vertexShaderPath.empty()) {
        glDeleteProgram(id);
    }
}

void ShaderParser::init() {
    string vertexShaderCode = FileReader::read(vertexShaderPath);
    string fragShaderCode = FileReader::read(fragShaderPath);
    const char* vertexShaderSrc = vertexShaderCode.c_str();
    const char* fragShaderSrc = fragShaderCode.c_str();

    uint32_t vertexShader, fragShader, geometryShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);
    checkCompileError(vertexShader, GL_VERTEX_SHADER);

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);
    checkCompileError(fragShader, GL_FRAGMENT_SHADER);

    if (!geometryShaderPath.empty()) {
        const char* geometryShaderSrc = (FileReader::read(geometryShaderPath)).c_str();
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderSrc, NULL);
        glCompileShader(geometryShader);
        checkCompileError(geometryShader, GL_GEOMETRY_SHADER);
    }

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragShader);
    if (!geometryShaderPath.empty()) {
        glAttachShader(id, geometryShader);
    }
    glLinkProgram(id);
    checkLinkError(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    if (!geometryShaderPath.empty()) {
        glDeleteShader(geometryShader);
    }

    // Buffer all active uniform variables
    // First get the number of active uniform variables
    int uniformNum;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniformNum);

    // Then store them in a hash map
    char nameBuffer[128];
    for (int i = 0; i < uniformNum; ++i) {
        int size;
        GLenum type;
        glGetActiveUniform(id, i, sizeof(nameBuffer), NULL, &size, &type, nameBuffer);
        string uniformName = string(nameBuffer);
        uniformBuffer[uniformName].location = glGetUniformLocation(id, nameBuffer);
        uniformBuffer[uniformName].size = size;
        uniformBuffer[uniformName].type = type;
    }
}

uint32_t ShaderParser::getID() const {
    return id;
}

void ShaderParser::use() const {
    glUseProgram(id);
}

void ShaderParser::setInt(const string &name, const int v) {
    return glUniform1i(getUniformLocation(name), v);
}

void ShaderParser::setFloat(const string &name, const float v) {
    return glUniform1f(getUniformLocation(name), v);
}

void ShaderParser::setBool(const string &name, const bool v) {
    return glUniform1i(getUniformLocation(name), (int)v);
}

void ShaderParser::setVec2(const string &name, const glm::vec2 &v) {
    return glUniform2fv(getUniformLocation(name), 1, &v[0]);
}

void ShaderParser::setVec3(const string &name, const glm::vec3 &v) {
    return glUniform3fv(getUniformLocation(name), 1, &v[0]);
}

void ShaderParser::setVec4(const string &name, const glm::vec4 &v) {
    return glUniform4fv(getUniformLocation(name), 1, &v[0]);
}

void ShaderParser::setMat4(const string &name, const glm::mat4 &v) {
    return glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &v[0][0]);
}

int ShaderParser::getUniformLocation(string name) {
    int location = uniformBuffer[name].location;
    if(location < 0) {
        ConsoleMsg::errorMsg("ACCESSING INVALID UNIFORM VARIABLE");
        exit(-1);
    }

    return location;
}

void ShaderParser::checkCompileError(uint32_t shader, GLenum type) const {
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        ConsoleMsg::errorMsg(string("SHADER COMPILATION ERROR OF TYPE ") +
                (type == GL_VERTEX_SHADER ? "VERTEX, " : (type == GL_FRAGMENT_SHADER ? "FRAGMENT, " : "GEOMETRY, ")) + infoLog);
        exit(-1);
    }
}

void ShaderParser::checkLinkError(uint32_t id) const {
    int success;
    char infoLog[1024];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
        ConsoleMsg::errorMsg(string("SHADER LINKING ERROR, ") + infoLog);
        exit(-1);
    }
}
