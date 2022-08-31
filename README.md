# OpenGL-helper-functions

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

  void oglh_get_uniform_variable(const char *variable_name, int type, void *data);
  void oglh_set_uniform_variable(const char *variable_name, int type, void *data);

	Set a uniform to an immediate constant value
	An example of setting values for ads lighting (Phong lighting:
	
	   oglh_set_uniform_value("ads_lighting", GL_FLOAT_VEC3, 0.5, 0.7, 0.3);
	
	The uniform variable named "ads_lighting" is a vec3 so the function
	passes three doubles which are formed into a vec3 in the GLSL uniform 
	variable. This is done inside the oglh_set_uniform_value function.

   void oglh_set_uniform_value(const char *variable_name, int type, ...);

	This function compiles both frag and vert shaders then links the shader and 
	activates it.
	
	The file extensions for shaders are: 
	shader_name.frag, shader_name.vert and shader_name.h 
	
	An optional common header file shader_name.h is "included" in both shader 
	files and can be used elsewhere. If the optional header is used then the 
	GLSL version needs to be on the first line.

   GLuint oglh_install_shader(const char *shader_name);

	Now for simple FBO use. Use the normal glDraw routines and periodically
	blit the FBO to the front buffer

    void oglh_set_rendering_to_fbo(int width, int height);
    void oglh_blit_fbo_to_front_buffer(void);

	This function shows all active uniform variables (and their values) that are 
	in the shader program. If a variable is present but isn't used it gets 
	optimized out and won't appear -- which can be a surprise.
	
   void oglh_display_active_uniform_variables(void);
