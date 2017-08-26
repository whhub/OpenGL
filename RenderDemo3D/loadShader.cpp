#include "loadShader.h"
#include <fstream>
#include <vector>

GLuint LoadShader( const char *vertice_path, const char *framgment_path )
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    bool bRet = compileShader(vertexShaderID, vertice_path);
    if(!bRet)
        return 0;
    bRet = compileShader(fragmentShaderID, framgment_path);
    if(!bRet)
        return 0;

    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vertexShaderID);
    glAttachShader(ProgramID, fragmentShaderID);
    glLinkProgram(ProgramID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return ProgramID;
}

static const GLchar* getFileData( const char * path )
{
    FILE* infile;
    fopen_s(&infile, path, "rb");
    if(!infile)
    {
        return NULL;
    }
    fseek(infile, 0, SEEK_END);
    int len = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    GLchar *source = new GLchar[len + 1];
    fread(source, 1, len, infile);
    fclose(infile);
    source[len] = 0;
    return const_cast<const GLchar *>(source);
}

bool compileShader( GLuint &shader_id, const char *path )
{
    const GLchar *shader_code = getFileData(path);
    if(strlen(shader_code) <= 0 )
        return 0;

    GLint Result = GL_FALSE;
    GLint InfoLogLength;

    glShaderSource(shader_id, 1, &shader_code, NULL);
    glCompileShader(shader_id);

        delete []shader_code;

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &Result);
    if ( !Result ){
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::vector<char> ShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(shader_id, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        printf("%s\n", &ShaderErrorMessage[0]);
        return false;
    }

    return true;
}