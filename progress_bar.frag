#version 330 core 

in vec4 channelCol;
out vec4 outCol; 
uniform float time;

void main()
{
	vec3 greenColor = vec3(0.0, 1.0, 0.0); 
    vec3 darkGreenColor = vec3(0.0, 0.7, 0.0); 

    vec3 interpolatedColor = mix(greenColor, darkGreenColor, time);

    outCol = vec4(interpolatedColor, 1.0);
}