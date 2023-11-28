#version 330
out vec4 FragColor;
  

in vec4 pixelCoord; 

uniform	int gridStep;
uniform	int gridHiglightCustom;

uniform	vec4    m_GridAxisColor;
uniform	vec4    m_GridMainColor;
uniform	vec4    m_Grid64thLineColor;
uniform	vec4    m_Grid1024thLineColor;
uniform	vec4    m_GridCustomColor;

bool isNthLine(int step)
{
    int x = int((pixelCoord.x));
    int y = int((pixelCoord.y));

    
    int xFrac = (x % step);
    int yFrac = (y % step);


    int w = 4;

    if (abs(xFrac - step) <= w)
        return true;

    if (abs(yFrac - step) <= w)
        return true;

    /*
    if (xFrac == 0)
        return true;
    else if (yFrac == 0)
        return true;*/

    return false;
}

vec4 calcLineColor()
{
    
    if (int(pixelCoord.x) == 0 || int(pixelCoord.y) == 0)
        return vec4(m_GridAxisColor.rgb,1);
        
    if (isNthLine(1024))                    return m_Grid1024thLineColor;
    if (isNthLine(64))                      return m_Grid64thLineColor;
    if (isNthLine(gridHiglightCustom) && gridHiglightCustom != 0)      return m_GridCustomColor;
    
    return vec4(m_GridMainColor.rgb, 0);
}

void main()
{    
    
    FragColor = calcLineColor();
    FragColor.a = 0.f;


    if (isNthLine(gridStep))
        FragColor.a = 1.f;
      
} 