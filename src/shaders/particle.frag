#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{    
    //color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
	//float temp = texture2D(image, TexCoords).r;
	//color = vec4(spriteColor.rgb, temp);
	color = vec4(spriteColor.rgb, texture(image,TexCoords).r);
}  
