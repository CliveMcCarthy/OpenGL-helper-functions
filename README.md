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

