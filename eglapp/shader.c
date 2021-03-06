/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

/*
 * shader.c
 * Functions for loading and process shaders.
 */

#include "shader.h"
#include "cube.h"

#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include "fakedriverInterface.h"

/* 
 * Loads the shader source into memory.
 *
 * sFilename: String holding filename to load 
 */
char* load_shader(char *sFilename) {
    char *pResult = NULL;
    FILE *pFile = NULL;
    long iLen = 0;

    pFile = fopen(sFilename, "r");

    if(pFile == NULL) {
        fprintf(stderr, "Error: Cannot read file '%s'\n", sFilename);
  		exit(-1);
    }

    fseek(pFile, 0, SEEK_END); /* Seek end of file */
    iLen = ftell(pFile);
    fseek(pFile, 0, SEEK_SET); /* Seek start of file again */
    pResult = calloc(iLen+1, sizeof(char));
    fread(pResult, sizeof(char), iLen, pFile);
    pResult[iLen] = '\0';
    fclose(pFile);

    return pResult;
}

void checkCompileErrors(GLuint shader, int type)
{
    GLint success = 0;
    GLchar infoLog[1024] = { 0 };
    if (type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        }
    }
    success = 0;
}

/* 
 * Create shader, load in source, compile, dump debug as necessary.
 *
 * pShader: Pointer to return created shader ID.
 * sFilename: Passed-in filename from which to load shader source.
 * iShaderType: Passed to GL, e.g. GL_VERTEX_SHADER.
 */
void process_shader(GLuint *pShader, char *sFilename, GLint iShaderType) {
	GLint iStatus;
	const char *aStrings[1] = { NULL };

	/* Create shader and load into GL. */
	*pShader = GL_CHECK(glCreateShader(iShaderType));
	
#define max_path 4096
    char resolved_path[max_path] = { 0 };
    _fullpath(resolved_path, sFilename, max_path);

	aStrings[0] = load_shader(resolved_path);
	
	GL_CHECK(glShaderSource(*pShader, 1, aStrings, NULL));

	/* Clean up shader source. */
	free((void *)aStrings[0]);
	aStrings[0] = NULL;

	/* Try compiling the shader. */
	GL_CHECK(glCompileShader(*pShader));
	GL_CHECK(glGetShaderiv(*pShader, GL_COMPILE_STATUS, &iStatus));

    checkCompileErrors(*pShader, iShaderType);

	// Dump debug info (source and log) if compilation failed.
	if(iStatus != GL_TRUE) {
		exit(-1);
	}
}

