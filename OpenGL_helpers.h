/*------------------------------------------------------------------------------
OpenGL Helper functions -- the un-Vulkan

OpenGL is thirty years old. It's great. However, despite it's massive API, there 
are 'missing' helper functions that can make writing applications a good deal 
cleaner and easier.

Here is a small collection of 'helper functions' that are part of my code and 
may help you with yours. They are written in C so they could be plugged into or 
attached to most code. 

My purpose is to 'hide' some of the grungy detail that OpenGL needs. As an 
example I want to set a GLSL uniform variable but rather commonly it is in the 
'current' program and I don't need or want to know the 'location' of the 
variable in GLSL storage. Just do it: set the variable as I specify !

None of these helper functions are particularly complicated. I just wish they 
were part of the regular API.





------------------------------------------------------------------------------*/
#pragma once
#define FALSE 0
#define TRUE  (!FALSE)
#ifndef __cplusplus
#define _GNU_SOURCE
#endif
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
#include <math.h>		//	Common mathematics functions
#include <stdarg.h>		//	Variable arguments
#include <stdbool.h>	//	(since C99)	Boolean type
#include <stdio.h>		//	Input/output
#include <stdlib.h>		//	General utilities
#include <string.h>		//	String handling
#include <GL/glx.h>		//	OpenGL X-Windows
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
#define __FUNC__ __func__
#define SOURCE_CODE_BUFFER_SIZE (128 * 1024) // 128k bytes

#define COLOR_ORANGE				"\x1b[38;2;255;127;0m"
#define ANSI_COLOR_RED				"\x1b[31m"
#define ANSI_COLOR_GREEN			"\x1b[32m"
#define ANSI_COLOR_YELLOW			"\x1b[33m"
#define ANSI_COLOR_BLUE				"\x1b[34m"
#define ANSI_COLOR_MAGENTA			"\x1b[35m"
#define ANSI_COLOR_CYAN				"\x1b[36m"
#define ANSI_COLOR_LIGHT_RED		"\x1b[91m"
#define ANSI_COLOR_LIGHT_GREEN		"\x1b[92m"
#define ANSI_COLOR_LIGHT_YELLOW		"\x1b[93m"
#define ANSI_COLOR_LIGHT_BLUE		"\x1b[94m"
#define ANSI_COLOR_LIGHT_MAGENTA	"\x1b[95m"
#define ANSI_COLOR_LIGHT_CYAN		"\x1b[96m"
#define ANSI_COLOR_RESET			"\x1b[0m"
#define ANSI_CLEAR_LINE				"\x1b[2K"

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
typedef struct glsl_uniform_type
{
	int count;
	const char *type;
	GLint gl_type;
	const char *gl_type_name;
	const char *glsl_type_name;
	const char *gl_uniform_call;
	const char *gl_getuniform_call;
}
GLSL_UNIFORM_TYPE;
#define ALPHA_MASK_SAMPLER2D		10
#define IMAGE_SAMPLER2D				30
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
	These C type definitions mimic the types found in GLSL
------------------------------------------------------------------------------*/
typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];
typedef int sampler2D;
typedef float *mat4;
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void oglh_generate_and_bind_opengl_object(GLint type, GLuint *object_id);


/*------------------------------------------------------------------------------
	In the folowing functions ensure that *data points to one of these C types:
	
	int, bool, float or
	vec2, vec3, vec4, 
	sampler2D
	*mat4
	
	which are defined above and that the corresponding type is one of:
	
	GL_INT, GL_BOOL, GL_FLOAT, 
	GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4
	GL_SAMPLER_2D, 
	GL_FLOAT_MAT4
------------------------------------------------------------------------------*/
void oglh_get_uniform_variable(const char *variable_name, int type, void *data);
void oglh_set_uniform_variable(const char *variable_name, int type, void *data);


/*------------------------------------------------------------------------------
	Set a uniform to an immediate constant value
	
	An example of setting values for ads lighting (Phong lighting:
	
	oglh_set_uniform_value("ads_lighting", GL_FLOAT_VEC3, 0.5, 0.7, 0.3);
	
	The uniform variable named "ads_lighting" is a vec3 so the function
	passes three doubles which are formed into a vec3 in the GLSL uniform 
	variable. This is done inside the oglh_set_uniform_value function.
------------------------------------------------------------------------------*/
void oglh_set_uniform_value(const char *variable_name, int type, ...);


/*------------------------------------------------------------------------------
	This function compiles both frag and vert shaders then links the shader and 
	activates it.
	
	The file extensions for shaders are: 
	shader_name.frag, shader_name.vert and shader_name.h 
	
	An optional common header file shader_name.h is "included" in both shader 
	files and can be used elsewhere. If the optional header is used then the 
	GLSL version needs to be on the first line.
------------------------------------------------------------------------------*/
GLuint oglh_install_shader(const char *shader_name);


/*------------------------------------------------------------------------------
	Now for simple FBO use. Use the normal glDraw routines and periodically
	blit the FBO to the front buffer
------------------------------------------------------------------------------*/
void oglh_set_rendering_to_fbo(int width, int height);
void oglh_blit_fbo_to_front_buffer(void);


/*------------------------------------------------------------------------------
	This function shows all active uniform variables (and their values) that are 
	in the shader program. If a variable is present but isn't used it gets 
	optimized out and won't appear -- which can be a surprise.
	
------------------------------------------------------------------------------*/
void oglh_display_active_uniform_variables(void);


/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
