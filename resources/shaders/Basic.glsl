#shader vertex
#version 330 core
                            
layout(location = 0) in vec4 position;

// For orthographic matrix
uniform mat4 u_MVP;

void main()
{  
    // Note that matrix multiplication is NOT commutative.
    gl_Position = u_MVP * position;
};

#shader fragment
#version 330 core
                             
layout(location = 0) out vec4 colour;

uniform vec4 u_Colour;

void main()
{  
    colour = u_Colour;
};