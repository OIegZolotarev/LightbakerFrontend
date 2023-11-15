#version 330

layout (location = 0) in vec3 xyz; 
layout (location = 1) in vec3 normal; 
layout (location = 2) in vec3 tangent; 
layout (location = 3) in vec4 color; 
layout (location = 4) in vec3 uv; 
  
out vec4 pixelCoord;

uniform vec3 origin;
uniform mat4 view;
uniform mat4 transform;
uniform mat4 projection;

void main()
{
    gl_Position = projection * (view * (transform * vec4(xyz, 1.0)));	
    pixelCoord = vec4(xyz,1);

    /*
    bool is1024th = (int(color.r) % 1024) < 1;
    bool is64th   = (int(color.r) % 64) < 1;
    bool isCustom = (int(color.r) % gridHiglightCustom) < 1;
    bool isAxis   = color.g > 0;

    if (isAxis)
        vertexColor = m_GridAxisColor;
    else if (is1024th)
        vertexColor = m_Grid1024thLineColor;
    else if (is64th)
        vertexColor = m_Grid64thLineColor;
    else if (isCustom && gridHiglightCustom > 0)
        vertexColor = m_GridCustomColor;
    else 
        vertexColor = m_GridMainColor;

    vertexColor.a = visible  ? 0.f : 1.f;
    */
}