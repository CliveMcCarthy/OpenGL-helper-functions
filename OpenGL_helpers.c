/*------------------------------------------------------------------------------
 oglh_ OpenGL Helper functions -- the un-Vulkan

OpenGL is thirty years old. It's great. However, despite it's massive API, there 
are 'missing' helper functions that can make writing applications a good deal 
cleaner and easier.

Here is a small collection of 'helper functions' that came from my code and 
may help you with yours. They are written in C so they could be plugged into 
most code. 

My purpose was to 'hide' some of the grungy detail that OpenGL needlessly 
demands. 

As an example I want to set a GLSL uniform variable but rather commonly it is in 
the 'current' program and I REALLY don't need to know the 'location' of the 
variable in GLSL storage. Just do it: set the variable as I specify
 -- spare me the details.

None of these helper functions are particularly complicated. I just wish they, 
or something like them, were part of the standard API.


------------------------------------------------------------------------------*/
#include "OpenGL_helpers.h"
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
static void oglh_program_error
(
	const char *path_file, int line, const char *func,
	const char *control, ...
)
{
	va_list parms;
	char text[256]; // text expansion buffer

	va_start(parms, control);
	vsprintf(text, control, parms);
	va_end(parms);

	printf
	(
		ANSI_COLOR_LIGHT_RED
		"\n*** error ***\n"
		ANSI_COLOR_RESET
	);

	printf
	(
		COLOR_ORANGE
		"\t%s\n"
		ANSI_COLOR_RESET,
		text
	);

	printf
	(
		ANSI_COLOR_LIGHT_RED
		"(in file: %s at line: %d in function: %s)\n"
		ANSI_COLOR_RESET,
		path_file, line, func
	);

	getchar();
	exit(EXIT_FAILURE);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
static void oglh_program_warning
(
	const char *path_file, int line, const char *func, const char *control, ...
)
{
	va_list parms;
	char text[256]; // text expansion buffer

	va_start(parms, control);
	vsprintf(text, control, parms);
	va_end(parms);
	
	printf(ANSI_COLOR_YELLOW);
	printf("\n** warning ** %s\n", text);
	printf("(in file: %s at line: %d in function: %s)\n",
		path_file, line, func);
	printf(ANSI_COLOR_RESET);
}
/*------------------------------------------------------------------------------
	OpenGL error detailed reporting
------------------------------------------------------------------------------*/
static void oglh_error
(
	const char *path_file, int line, const char *func, GLenum error_code
)
{
	printf(ANSI_CLEAR_LINE ANSI_COLOR_RED);
	printf("** OpenGL error ** ");

	switch(error_code)
	{
	case GL_NO_ERROR:
		printf("GL_NO_ERROR\n");
		break;

	case GL_INVALID_OPERATION:
		printf("GL_INVALID_OPERATION\n");
		break;

	case GL_INVALID_ENUM:
		printf("GL_INVALID_ENUM\n");
		break;

	case GL_INVALID_VALUE:
		printf("GL_INVALID_VALUE\n");
		break;

	case GL_STACK_OVERFLOW:
	case GL_STACK_UNDERFLOW:
		printf("GL_STACK_OVERFLOW/GL_STACK_UNDERFLOW\n");
		break;

	case GL_OUT_OF_MEMORY:
		printf("GL_OUT_OF_MEMORY\n");
		break;

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		printf("GL_INVALID_FRAMEBUFFER_OPERATION\n");
		break;

	default:
		printf("undefined! = %d\n", error_code);
		break;
	}

	printf
	(
		"(in file: %s at line: %d in function: %s)\n", 
		path_file, line, func
	);
	printf(ANSI_CLEAR_LINE ANSI_COLOR_RESET);
	getchar();
	exit(error_code);
}
/*------------------------------------------------------------------------------
	OpenGL error checking -- can be sprinkled in code liberally
------------------------------------------------------------------------------*/
static inline void oglh_error_check
(
	const char *path_file, int line, const char *func
)
{
	GLenum error_code;

	if((error_code = glGetError()) == GL_NO_ERROR)
	{
		return;	// quickly -- no need to report anything
	}
	else
	{
		oglh_error(path_file, line, func, error_code);
	}
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void oglh_activate_and_bind_opengl_object(GLint type, GLuint *object_id)
{
	*object_id = 0;

	switch(type)
	{
		case GL_TEXTURE_2D:
			glActiveTexture(IMAGE_SAMPLER2D + GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *object_id);
		break;

		case GL_VERTEX_ARRAY:
			glGenVertexArrays(1, object_id);
			glBindVertexArray(*object_id);	// note the different parameters
		break;
		
		default:
			oglh_program_error(__FILE__, __LINE__, __FUNC__,
				"unrecognized object type: %d", type);
		break;
	}

	if(*object_id == 0)
	{
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"generating and binding opengl object failed type: %d", type);
	}

	oglh_error_check(__FILE__, __LINE__, __FUNC__);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void oglh_generate_and_bind_opengl_object(GLint type, GLuint *object_id)
{
	*object_id = 0;

	switch(type)
	{
		case GL_ARRAY_BUFFER:
		case GL_ELEMENT_ARRAY_BUFFER:
		case GL_PIXEL_PACK_BUFFER:
		case GL_PIXEL_UNPACK_BUFFER:
		case GL_TRANSFORM_FEEDBACK_BUFFER:
			glGenBuffers(1, object_id);
			glBindBuffer(type, *object_id);
		break;

		case GL_FRAMEBUFFER:
			glGenFramebuffers(1, object_id);
			glBindFramebuffer(type, *object_id);
		break;

		case GL_RENDERBUFFER:
			glGenRenderbuffers(1, object_id);
			glBindRenderbuffer(type, *object_id);
		break;

		case GL_TEXTURE_1D:
		case GL_TEXTURE_2D:
		case GL_TEXTURE_3D:
		case GL_TEXTURE_1D_ARRAY:
		case GL_TEXTURE_2D_ARRAY:
		case GL_TEXTURE_CUBE_MAP:
			glGenTextures(1, object_id);
			glBindTexture(type, *object_id);
			//glActiveTexture(object_id);
			oglh_error_check(__FILE__, __LINE__, __FUNC__);
		break;

		case GL_VERTEX_ARRAY:
			glGenVertexArrays(1, object_id);
			glBindVertexArray(*object_id);	// note the different parameters
		break;

		default:
			oglh_program_error(__FILE__, __LINE__, __FUNC__,
				"unrecognized object type: %d", type);
		break;
	}

	if(*object_id == 0)
	{
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"generating and binding opengl object failed type: %d", type);
	}

	oglh_error_check(__FILE__, __LINE__, __FUNC__);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void oglh_activate_and_bind_opengl_texture(int texture_map_unit, int texture_id)
{
	if(texture_map_unit == GL_TEXTURE0)
	{
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"can't use TEXTURE_MAP_UNIT_0 "
			"which is reserved for the fixed pipeline");
	}
	glActiveTexture(texture_map_unit);
	glBindTexture(GL_TEXTURE_2D, texture_id);
 	oglh_error_check(__FILE__, __LINE__, __FUNC__);
}
/*------------------------------------------------------------------------------

	get the GLSL program location for a variable within the current program
------------------------------------------------------------------------------*/
static GLint get_uniform_location(const char *variable_name)
{
	GLint location, program_id;
	// whine: why does a user care about the uniform's location
	// an interior detail -- that is the compiler's job
	// this is much to close to assembly language

	glGetIntegerv(GL_CURRENT_PROGRAM, &program_id);
	location = glGetUniformLocation(program_id, variable_name);

	if(location == -1)
	{
		// a built-in variable
	}
	else
	if(location < 0)
	{
		oglh_program_warning(__FILE__, __LINE__, __FUNC__,
			"failed to locate uniform variable: '%s'\n\tin program %d",
			variable_name, program_id);
	}
	
	oglh_error_check(__FILE__, __LINE__, __FUNC__);
	return location;
}
/*------------------------------------------------------------------------------
	 The standard definitions for GLSL UNIFORM types are used
	 The value is passed via void pointer to the variable to be set
	 
	Ensure that *data points appropriately to one of these C types
	int, bool, float, vec2, vec3, vec4, *mat4, ivec2, ivec3, ivec4, sampler2D
	It's a void pointer so all hell can break loose if you get it wrong.
------------------------------------------------------------------------------*/
void oglh_set_uniform_variable(const char *variable_name, int type, void *data)
{
	GLint location;
	// all the possible variable types
	int *value;
	float *float_value;
	vec2 *vec2_value;
	vec3 *vec3_value;
	vec4 *vec4_value;
	mat4 mat4_value;

	location = get_uniform_location(variable_name);

	switch(type) // switch on GLSL data type
	{
		case GL_INT:
		case GL_BOOL:
		case GL_SAMPLER_2D:
			value = data;
			glUniform1i(location, *value);
		break;

		case GL_FLOAT:
			float_value = data;
			glUniform1f(location, *float_value);
		break;

		case GL_FLOAT_VEC2:
			vec2_value = data;
			glUniform2f(location, (*vec2_value)[0], (*vec2_value)[1]);
		break;

		case GL_FLOAT_VEC3:
			vec3_value = data;
			glUniform3f
			(
				location, 
				(*vec3_value)[0], (*vec3_value)[1], (*vec3_value)[2]
			);
		break;

		case GL_FLOAT_VEC4:
			vec4_value = data;
			glUniform4f
			(
				location, 
				(*vec4_value)[0], (*vec4_value)[1], 
				(*vec4_value)[2], (*vec4_value)[3]
			);
		break;

		case GL_FLOAT_MAT4:
			mat4_value = data; // sixteen floats in the matrix
			glUniformMatrix4fv(location, 1, TRUE, mat4_value);
		break;

		case GL_DOUBLE: // there are no doubles in GLSL
		default:
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"GLSL bad uniform data type %d\n", type);
	}	
	oglh_error_check(__FILE__, __LINE__, __FUNC__);
}
/*------------------------------------------------------------------------------
	 The standard definitions for GLSL UNIFORM types are used
	 The parameters are passed as void pointers to the data

	Ensure that *data points to one of these C types
	int, bool, float, vec2, vec3, vec4, *mat4, ivec2, ivec3, ivec4, sampler2D
------------------------------------------------------------------------------*/
void oglh_get_uniform_variable(const char *variable_name, int type, void *data)
{
	GLint location, program_id;

	glGetIntegerv(GL_CURRENT_PROGRAM, &program_id);
	location = get_uniform_location(variable_name);

	switch(type) // switch on GLSL data type
	{
		case GL_INT:
		case GL_BOOL:
		case GL_SAMPLER_2D:
			glGetUniformiv(program_id, location, (int *)data);
		break;

		case GL_FLOAT:
			glGetUniformfv(program_id, location, (float *)data);
		break;

		case GL_FLOAT_VEC2:
		case GL_FLOAT_VEC3:
		case GL_FLOAT_VEC4:
			glGetUniformfv(program_id, location, (float *)data);
		break;

		case GL_DOUBLE: // there are no doubles in GLSL
		default:
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"GLSL bad uniform data type %d\n", type);
	}
	oglh_error_check(__FILE__, __LINE__, __FUNC__);
}
/*------------------------------------------------------------------------------
	This sets the uniform variable with immediate constant data
------------------------------------------------------------------------------*/
void oglh_set_uniform_value(const char *variable_name, int type, ...)
{
	va_list arg_list;
	GLint location;
	int value;
	float float_value;
	float vec_value[4];

	va_start(arg_list, type); // start at the last fixed parameter
	location = get_uniform_location(variable_name);

	switch(type) // switch on GLSL data type
	{
		case GL_INT:
		case GL_BOOL:
		case GL_SAMPLER_2D:
			// the same code for bool, int, & sampler2D
			value = va_arg(arg_list, int);
			glUniform1i(location, value);
		break;

		case GL_FLOAT:
		//case GL_DOUBLE:
			// arglist always uses doubles -- floats are not allowed
			float_value = va_arg(arg_list, double);
			glUniform1f(location, (float)float_value);
		break;

		case GL_FLOAT_VEC2:
			vec_value[0] = va_arg(arg_list, double);
			vec_value[1] = va_arg(arg_list, double);
			glUniform2f(location, (vec_value)[0], (vec_value)[1]);
		break;

		case GL_FLOAT_VEC3:
			vec_value[0] = va_arg(arg_list, double);
			vec_value[1] = va_arg(arg_list, double);
			vec_value[2] = va_arg(arg_list, double);
			glUniform3f(location, (vec_value)[0], (vec_value)[1], (vec_value)[2]);
		break;

		case GL_FLOAT_VEC4:
			vec_value[0] = va_arg(arg_list, double);
			vec_value[1] = va_arg(arg_list, double);
			vec_value[2] = va_arg(arg_list, double);
			vec_value[3] = va_arg(arg_list, double);
			glUniform4f(location, (vec_value)[0], (vec_value)[1], (vec_value)[2], (vec_value)[3]);
		break;

		case GL_FLOAT_MAT4:
		oglh_program_warning(__FILE__, __LINE__, __FUNC__,
			"GLSL uniform data type GL_FLOAT_MAT4 is not supported");
		break;

		default:
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"GLSL bad uniform data type %d", type);
	}
	va_end(arg_list);
	oglh_error_check(__FILE__, __LINE__, __FUNC__);
}
/*------------------------------------------------------------------------------
	Note: if a common header is used then the GLSL version
	needs to be specified on the _first_ line of the header and not in the GLSL
	vertex or fragment code.
	
	To separate aspects of a header for C and GLSL compilation this can be done 
	with a simple conditional compilation directive such as:
	
	#if defined(__GNUC__) || defined(_MSC_VER)
		// Stuff for C code alone
	#else
		// Stuff for GLSL code alone
		#define M_PI 3.14159265358979323846264338327950288
	#endif

------------------------------------------------------------------------------*/
static char *load_glsl_file(const char *shader_name, GLenum shader_type)
{
	char *shader_code_buffer = NULL;
	char shader_file[FILENAME_MAX];
	FILE *shader_code_fptr;
	char *code_buffer = NULL;
	char header_file[FILENAME_MAX];
	FILE *header_fptr;

	switch(shader_type)
	{
		case GL_FRAGMENT_SHADER:
			sprintf(shader_file, "%s.frag", shader_name);
		break;

		case GL_VERTEX_SHADER:
			sprintf(shader_file, "%s.vert", shader_name);
		break;

		default:
			oglh_program_error(__FILE__, __LINE__, __FUNC__,
				"GLSL shader is neither vertex nor fragment %d", shader_type);
	}

	printf("Compiling shader file\t: %s\n", shader_file);
	
	if((shader_code_buffer = (char *)calloc(1, SOURCE_CODE_BUFFER_SIZE)) == NULL)
	{
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"GLSL out of memory opening buffer");
	}

	if((shader_code_fptr = fopen(shader_file, "rt")) == NULL)
	{
		oglh_program_error
		(
			__FILE__, __LINE__, __FUNC__,
			"GLSL opening shader file: %12s",
			shader_file
		);
	}
	
	sprintf(header_file, "%s.h", shader_name);

	if((header_fptr = fopen(header_file, "rt")) == NULL)
	{
		// no common header file -- so don't use one
		fread(shader_code_buffer, SOURCE_CODE_BUFFER_SIZE, 1, shader_code_fptr);
	}
	else
	{
		if((code_buffer = (char *)calloc(1, SOURCE_CODE_BUFFER_SIZE)) == NULL)
		{
			oglh_program_error(__FILE__, __LINE__, __FUNC__,
				"GLSL out of memory opening buffer");
		}
		
		printf("Using header file\t: %s\n", header_file);
		// read the optional header into the buffer
		fread(shader_code_buffer, SOURCE_CODE_BUFFER_SIZE, 1, header_fptr);
		// reset the line numbering
		strcat(shader_code_buffer, "#line 0\n");
		fread(code_buffer, SOURCE_CODE_BUFFER_SIZE, 1, shader_code_fptr);
		strcat(shader_code_buffer, code_buffer);
		
		printf("Shader\t\t\t: %s\n", shader_file);
		//puts(shader_code_buffer);
		free(code_buffer);
	}

	oglh_error_check(__FILE__, __LINE__, __FUNC__);
	return shader_code_buffer;
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
static GLuint compile_shader(const char *shader_name, GLenum shader_type)
{
	GLuint shader_id = 0;
	GLint success;
	const GLchar *shader_source_code;
	GLchar *log_buffer = NULL;

	shader_source_code = load_glsl_file(shader_name, shader_type);
	if(shader_source_code == NULL)
	{
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"GLSL shader\t%s is absent\n", shader_name);
	}

	shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &shader_source_code, NULL);
	glCompileShader(shader_id);
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		if((log_buffer = (char *)calloc(1, SOURCE_CODE_BUFFER_SIZE)) == NULL)
		{
			oglh_program_error(__FILE__, __LINE__, __FUNC__,
				"GLSL out of memory opening log buffer");
			// we really are in trouble if this happens
		}
		
		glGetShaderInfoLog(shader_id, SOURCE_CODE_BUFFER_SIZE, NULL, log_buffer);
		printf("compilation log\t:\n%s\n", log_buffer);
		
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"GLSL compiling shader '%s' failed",
			shader_name);
	}

 	oglh_error_check(__FILE__, __LINE__, __FUNC__);
	return shader_id;
}
/*------------------------------------------------------------------------------
	This code compiles both frag and vert shaders then links a shader and 
	activates it.
	
	File extensions for shaders: 
	shader_name.frag, shader_name.vert and shader_name.h 
	
	An optional common header file shader_name.h is "included" in both shader 
	files and can be used elsewhere

------------------------------------------------------------------------------*/
GLuint oglh_install_shader(const char *shader_name)
{
	GLuint vertex_shader_id, fragment_shader_id, program_id;
	GLint success;
	
	printf("Compiling shader\t: %s\n", shader_name);

	program_id = glCreateProgram();

	vertex_shader_id	= compile_shader(shader_name, GL_VERTEX_SHADER);
	if(vertex_shader_id != 0) 
		glAttachShader(program_id, vertex_shader_id);
	
	fragment_shader_id	= compile_shader(shader_name, GL_FRAGMENT_SHADER);
	if(fragment_shader_id != 0) 
		glAttachShader(program_id, fragment_shader_id);

	printf("GLSL linking\t\t: %s\n", shader_name);
	glLinkProgram(program_id);

	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if(!success)
	{
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"GLSL linking\tfile %s failed", shader_name);
	}

	glUseProgram(program_id);
 	oglh_error_check(__FILE__, __LINE__, __FUNC__);
	printf("Compilation done\t: %s\n", shader_name);
	return program_id;
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
static void oglh_check_framebuffer_completeness_status
(
	const char *file, int line, const char *func
)
{
	switch(glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		case GL_FRAMEBUFFER_COMPLETE: 
		// we are good to go, the framebuffer is complete
		break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			oglh_program_error(file, line, func,
				"Not all framebuffer attachment points are framebuffer "
				"attachment complete.\n");
		break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			oglh_program_error(file, line, func,
				"Not all attached images have the same width and height.");
		break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			oglh_program_error(file, line, func,
				"No images are attached to the framebuffer.");
		break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			oglh_program_error(file, line, func,
				"Color attached images have different internal formats.");
		break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			oglh_program_error(file, line, func,
				"Framebuffer incomplete: Draw buffer.");
		break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			oglh_program_error(file, line, func,
				"Framebuffer incomplete: Read buffer.");
		break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			oglh_program_error(file, line, func,
				"The combination of internal formats of the attached "
				"images violates an\n"
				"implementation-dependent set of restrictions.");
		break;

		default:
			oglh_program_error(file, line, func,
				"glCheckFramebufferStatusEXT failed for an unknown reason");
	}
}
/*------------------------------------------------------------------------------
	glBlitFramebuffer

	transfers a rectangle of pixel values from one region
	of the read framebuffer to another region in the draw framebuffer. mask
	is the bitwise OR of a number of values indicating which buffers are to
	be copied.

	The values are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, and
	GL_STENCIL_BUFFER_BIT. The pixels corresponding to these buffers are
	copied from the source rectangle bounded by the locations (srcX0; srcY0)
	and (srcX1; srcY1) to the destination rectangle bounded by the locations
	(dstX0; dstY0) and (dstX1; dstY1). The lower bounds of the rectangle are
	inclusive, while the upper bounds are exclusive.

	The name comes from BLIT, which is not an acronym
	A typical use for a blitter is the movement of a bitmap

------------------------------------------------------------------------------*/
void oglh_blit_fbo_to_front_buffer(void)
{
	GLint frame_buffer_name;
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &frame_buffer_name);
	// change draw framebuffer to be the front buffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_FRONT);

	glBlitFramebuffer		// copy FBO data to the front buffer
	(
		viewport[0],  viewport[1],
		viewport[2] + viewport[0],
		viewport[3] + viewport[1],
		viewport[0],  viewport[1],
		viewport[2] + viewport[0],
		viewport[3] + viewport[1],
		GL_COLOR_BUFFER_BIT,
		GL_NEAREST
	);

	// restore draw framebuffer to be the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_name);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}
/*------------------------------------------------------------------------------
	http://www.songho.ca/opengl/gl_fbo.html -- this helped me a lot

	GL_READ_FRAMEBUFFER, or GL_DRAW_FRAMEBUFFER.
	allow you to bind an FBO so that
	reading commands (front_buffer_glReadPixels, glCopyPixels, etc) and
	writing commands (any command of the form glDraw*)
	can happen to two different buffers.

------------------------------------------------------------------------------*/
void oglh_set_rendering_to_fbo(int width, int height)
{
	GLuint frame_buffer_id = 0, render_buffer_id = 0;
	// whine: why does a user care about the frame_buffer and render_buffer
	// id's they are just interior details -- that is the compiler's job
	// this is, again, much to close to assembly language

	if(width > GL_MAX_RENDERBUFFER_SIZE || height > GL_MAX_RENDERBUFFER_SIZE)
	{
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"width %d or height %d\nexceeds GL_MAX_RENDERBUFFER_SIZE of %d",
			width, height, GL_MAX_RENDERBUFFER_SIZE);
	}

	// create a framebuffer object
	glGenFramebuffers(1, &frame_buffer_id);

	// GL_FRAMEBUFFER target simply sets both the read and the write to
	// the same FBO.
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);
	if(!glIsFramebuffer(frame_buffer_id))
	{
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"glIsFramebuffer failed (frame_buffer_id=%d)",
			frame_buffer_id);
	}

	// create a renderbuffer object to store the image
	glGenRenderbuffers(1, &render_buffer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_id);
	if(!glIsRenderbuffer(render_buffer_id))
	{
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"glIsRenderbuffer failed (render_buffer_id=%d)",
			render_buffer_id);
	}

	// We are guaranteed to be able to have at least color attachment 0
	// attach the renderbuffer to GL_COLOR_ATTACHMENT0
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_RENDERBUFFER, render_buffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, width, height);

	oglh_check_framebuffer_completeness_status(__FILE__, __LINE__, __FUNC__);
	glViewport(0, 0, width, height);

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glEnable(GL_BLEND);					// enable blending etc.
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	oglh_error_check(__FILE__, __LINE__, __FUNC__);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
GLSL_UNIFORM_TYPE oglh_uniform_variable_type_table[] =
{
{	1,	"f",	GL_FLOAT,	"GL_FLOAT",	"float",	"glUniform1f(location",	"glGetUniform1f(location"	},
{	2,	"f",	GL_FLOAT_VEC2,	"GL_FLOAT_VEC2",	"vec2",	"glUniform2f(location",	"glGetUniform2f(location"	},
{	3,	"f",	GL_FLOAT_VEC3,	"GL_FLOAT_VEC3",	"vec3",	"glUniform3f(location",	"glGetUniform3f(location"	},
{	4,	"f",	GL_FLOAT_VEC4,	"GL_FLOAT_VEC4",	"vec4",	"glUniform4f(location",	"glGetUniform4f(location"	},
{	1,	"d",	GL_DOUBLE,	"GL_DOUBLE",	"double",	"glUniform1d(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_INT,	"GL_INT",	"int",	"glUniform1i(location",	"glGetUniform1i(location"	},
{	2,	"i",	GL_INT_VEC2,	"GL_INT_VEC2",	"ivec2",	"glUniform2i(location",	"glGetUniform2i(location"	},
{	3,	"i",	GL_INT_VEC3,	"GL_INT_VEC3",	"ivec3",	"glUniform3i(location",	"glGetUniform3i(location"	},
{	4,	"i",	GL_INT_VEC4,	"GL_INT_VEC4",	"ivec4",	"glUniform4i(location",	"glGetUniform4i(location"	},
{	1,	"i",	GL_UNSIGNED_INT,	"GL_UNSIGNED_INT",	"unsigned int",	"glUniform1i(location",	"glGetUniform1i(location"	},
{	2,	"i",	GL_UNSIGNED_INT_VEC2,	"GL_UNSIGNED_INT_VEC2",	"uvec2",	"glUniform2i(location",	"glGetUniform2i(location"	},
{	3,	"i",	GL_UNSIGNED_INT_VEC3,	"GL_UNSIGNED_INT_VEC3",	"uvec3",	"glUniform3i(location",	"glGetUniform3i(location"	},
{	4,	"i",	GL_UNSIGNED_INT_VEC4,	"GL_UNSIGNED_INT_VEC4",	"uvec4",	"glUniform4i(location",	"glGetUniform4i(location"	},
{	1,	"i",	GL_BOOL,	"GL_BOOL",	"bool",	"glUniform1i(location",	"glGetUniform1i(location"	},
{	2,	"i",	GL_BOOL_VEC2,	"GL_BOOL_VEC2",	"bvec2",	"glUniform2i(location",	"glGetUniform2i(location"	},
{	3,	"i",	GL_BOOL_VEC3,	"GL_BOOL_VEC3",	"bvec3",	"glUniform3i(location",	"glGetUniform3i(location"	},
{	4,	"i",	GL_BOOL_VEC4,	"GL_BOOL_VEC4",	"bvec4",	"glUniform4i(location",	"glGetUniform4i(location"	},
{	2,	"f",	GL_FLOAT_MAT2,	"GL_FLOAT_MAT2",	"mat2",	"glUniformMatrix2f(locationcount, transpose, ",	"glGetUniformMatrix2f(locationcount, transpose, "	},
{	3,	"f",	GL_FLOAT_MAT3,	"GL_FLOAT_MAT3",	"mat3",	"glUniformMatrix3f(locationcount, transpose, ",	"glGetUniformMatrix3f(locationcount, transpose, "	},
{	4,	"f",	GL_FLOAT_MAT4,	"GL_FLOAT_MAT4",	"mat4",	"glUniformMatrix4f(locationcount, transpose, ",	"glGetUniformMatrix4f(locationcount, transpose, "	},
{	3,	"f",	GL_FLOAT_MAT2x3,	"GL_FLOAT_MAT2x3",	"mat2x3",	"glUniformMatrix3f(locationcount, transpose, ",	"glGetUniformMatrix3f(locationcount, transpose, "	},
{	4,	"f",	GL_FLOAT_MAT2x4,	"GL_FLOAT_MAT2x4",	"mat2x4",	"glUniformMatrix4f(locationcount, transpose, ",	"glGetUniformMatrix4f(locationcount, transpose, "	},
{	2,	"f",	GL_FLOAT_MAT3x2,	"GL_FLOAT_MAT3x2",	"mat3x2",	"glUniformMatrix2f(locationcount, transpose, ",	"glGetUniformMatrix2f(locationcount, transpose, "	},
{	4,	"f",	GL_FLOAT_MAT3x4,	"GL_FLOAT_MAT3x4",	"mat3x4",	"glUniformMatrix4f(locationcount, transpose, ",	"glGetUniformMatrix4f(locationcount, transpose, "	},
{	2,	"f",	GL_FLOAT_MAT4x2,	"GL_FLOAT_MAT4x2",	"mat4x2",	"glUniformMatrix2f(locationcount, transpose, ",	"glGetUniformMatrix2f(locationcount, transpose, "	},
{	3,	"f",	GL_FLOAT_MAT4x3,	"GL_FLOAT_MAT4x3",	"mat4x3",	"glUniformMatrix3f(locationcount, transpose, ",	"glGetUniformMatrix3f(locationcount, transpose, "	},
{	1,	"i",	GL_SAMPLER_1D,	"GL_SAMPLER_1D",	"sampler1D",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_SAMPLER_2D,	"GL_SAMPLER_2D",	"sampler2D",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_SAMPLER_3D,	"GL_SAMPLER_3D",	"sampler3D",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_SAMPLER_CUBE,	"GL_SAMPLER_CUBE",	"samplerCube",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_SAMPLER_1D_SHADOW,	"GL_SAMPLER_1D_SHADOW",	"sampler1DShadow",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_SAMPLER_2D_SHADOW,	"GL_SAMPLER_2D_SHADOW",	"sampler2DShadow",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_SAMPLER_1D_ARRAY,	"GL_SAMPLER_1D_ARRAY",	"sampler1DArray",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_SAMPLER_2D_ARRAY,	"GL_SAMPLER_2D_ARRAY",	"sampler2DArray",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_SAMPLER_1D_ARRAY_SHADOW,	"GL_SAMPLER_1D_ARRAY_SHADOW",	"sampler1DArrayShadow",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_SAMPLER_2D_ARRAY_SHADOW,	"GL_SAMPLER_2D_ARRAY_SHADOW",	"sampler2DArrayShadow",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_INT_SAMPLER_1D,	"GL_INT_SAMPLER_1D",	"isampler1D",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_INT_SAMPLER_2D,	"GL_INT_SAMPLER_2D",	"isampler2D",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_INT_SAMPLER_3D,	"GL_INT_SAMPLER_3D",	"isampler3D",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_INT_SAMPLER_CUBE,	"GL_INT_SAMPLER_CUBE",	"isamplerCube",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_INT_SAMPLER_1D_ARRAY,	"GL_INT_SAMPLER_1D_ARRAY",	"isampler1DArray",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_INT_SAMPLER_2D_ARRAY,	"GL_INT_SAMPLER_2D_ARRAY",	"isampler2DArray",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_UNSIGNED_INT_SAMPLER_1D,	"GL_UNSIGNED_INT_SAMPLER_1D",	"usampler1D",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_UNSIGNED_INT_SAMPLER_2D,	"GL_UNSIGNED_INT_SAMPLER_2D",	"usampler2D",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_UNSIGNED_INT_SAMPLER_3D,	"GL_UNSIGNED_INT_SAMPLER_3D",	"usampler3D",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_UNSIGNED_INT_SAMPLER_CUBE,	"GL_UNSIGNED_INT_SAMPLER_CUBE",	"usamplerCube",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_UNSIGNED_INT_SAMPLER_1D_ARRAY,	"GL_UNSIGNED_INT_SAMPLER_1D_ARRAY",	"usampler2DArray",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,	"GL_UNSIGNED_INT_SAMPLER_2D_ARRAY",	"usampler2DArray",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	1,	"i",	GL_SAMPLER_CUBE_SHADOW,	"GL_SAMPLER_CUBE_SHADOW",	"samplerCubeShadow",	"glUniform1i(location",	"glGetUniform1d(location"	},
{	0,	NULL, 0, NULL,	NULL,	NULL,	NULL	}
};
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
GLSL_UNIFORM_TYPE *oglh_find_uniform_variable_template(GLint gl_type)
{
	GLSL_UNIFORM_TYPE *uniform_variable_template = NULL;

	uniform_variable_template = oglh_uniform_variable_type_table;
	while(uniform_variable_template->gl_type_name != NULL)
	{
		if(uniform_variable_template->gl_type == gl_type) break;
		uniform_variable_template++;
	}
	return uniform_variable_template;
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void oglh_display_active_uniform_variables(void)
{
	GLint number, max_length, length, size, index, location;
	GLSL_UNIFORM_TYPE *uniform_variable_template;
	GLchar *variable_name;
	GLint program_id, max_texture_units;
	GLenum gl_type;
	GLint int_data;
	GLfloat float_data;
	GLfloat vec_data[4];
	GLfloat mat_data[16];

	printf(ANSI_COLOR_GREEN);
	glGetIntegerv(GL_CURRENT_PROGRAM, &program_id);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_texture_units);

	glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &number);
	if(number < 1)
	{
		oglh_program_warning(__FILE__, __LINE__, __FUNC__,
			"there are no uniform variables");
		return;
	}

	glGetProgramiv(program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);

	if((variable_name = (GLchar *)calloc(1, max_length)) == NULL)
	{
		oglh_program_error(__FILE__, __LINE__, __FUNC__,
			"out of memory opening variable_name buffer");
	}

	printf(ANSI_COLOR_RESET);
    printf
	(
		"        "
		"--------------------------------------------"
		"---------------------------"
		"\n"
	);
	printf("\tGLSL uniform variables:\n");
	for(index = 0; index < number; index++)
	{
		glGetActiveUniform(program_id, index, max_length, &length, &size, &gl_type, variable_name);
		location = get_uniform_location(variable_name);
		if(location >= 0)	// show only the non-built-in variables
		{
			printf("\t%-20s", variable_name);
			uniform_variable_template = oglh_find_uniform_variable_template(gl_type);

			printf(" @%2d %-15s = ",
				location, uniform_variable_template->gl_type_name);

			switch(gl_type) // switch GL types
			{
				case GL_INT:
				case GL_BOOL:
					// the same code for bool, int
					glGetUniformiv(program_id, location, &int_data);
					printf("%6d\n", int_data);
				break;

				case GL_SAMPLER_2D:
					printf(ANSI_COLOR_YELLOW);
					glGetUniformiv(program_id, location, &int_data);
					//printf("%6d of 0 to %d\n", int_data, max_texture_units - 1);
					printf("%6d\n", int_data);
					printf(ANSI_COLOR_RESET);
				break;

				case GL_FLOAT:
					glGetUniformfv(program_id, location, &float_data);
					if(fabs(float_data) < 100000.0)
						printf("%6.3f\n", float_data);
					else
						printf("??????\n");
				break;

				case GL_FLOAT_VEC2:
					glGetUniformfv(program_id, location, vec_data);
					printf("%5.2f %5.2f\n",
						vec_data[0], vec_data[1]);
				break;

				case GL_FLOAT_VEC3:
					glGetUniformfv(program_id, location, vec_data);
					printf("%5.2f %5.2f %5.2f\n",
						vec_data[0], vec_data[1], vec_data[2]);
				break;

				case GL_FLOAT_VEC4:
					glGetUniformfv(program_id, location, vec_data);
					printf("%5.2f   %5.2f   %5.2f   %5.2f\n",
						vec_data[0], vec_data[1], vec_data[2], vec_data[3]);
				break;

				case GL_FLOAT_MAT3:
					glGetUniformfv(program_id, location, mat_data);
					// diagonal data
					printf("%4.1f \\%6.1f \\%6.1f\n",
						mat_data[0], mat_data[4], mat_data[8]);
				break;

				case GL_FLOAT_MAT4:
					printf("\n");
					glGetUniformfv(program_id, location, mat_data);
					// all the data
					printf("\t\t\t\t%9.4f %9.4f %9.4f %9.4f\n",
						mat_data[0], mat_data[1], mat_data[2], mat_data[3]);
					printf("\t\t\t\t%9.4f %9.4f %9.4f %9.4f\n",
						mat_data[4], mat_data[5], mat_data[6], mat_data[7]);
					printf("\t\t\t\t%9.4f %9.4f %9.4f %9.4f\n",
						mat_data[8], mat_data[9], mat_data[10], mat_data[11]);
					printf("\t\t\t\t%9.4f %9.4f %9.4f %9.4f\n",
						mat_data[12], mat_data[13], mat_data[14], mat_data[15]);
				break;
				
				case GL_DOUBLE:
				default:
				oglh_program_error(__FILE__, __LINE__, __FUNC__,
					"\tGLSL bad uniform data type %d\n", gl_type);
			}

		}
	}
	
	free(variable_name);
    printf
	(
		"        "
		"--------------------------------------------"
		"---------------------------"
		"\n"
	);
	oglh_error_check(__FILE__, __LINE__, __FUNC__);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
