#shader vertex
#version 330 core
                            
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 vertexInputColour;

out vec3 vertexOutputColour;

// For orthographic matrix
uniform mat4 u_MVP;

void main()
{  
    // Note that matrix multiplication is NOT commutative.
    gl_Position = u_MVP * position;

    vertexOutputColour = vertexInputColour;
};

#shader fragment
#version 330 core

// layout(location = 0) out vec4 colour;
// uniform vec4 u_Colour;

in vec3 vertexInputColour;      
out vec4 fragmentColour;

void main()
{  
    fragmentColour = vec4(vertexInputColour, 1.0);
};