#version 330 core 

in vec3 channelCol; 
out vec4 outCol;
uniform float time;
uniform float type;

void main()
{
	if (type == 1) //yellow
		outCol = vec4(channelCol.r ,channelCol.g,time,1.0);
	else if (type == 2) //red
		outCol = vec4(channelCol.r ,time, time, 1.0);
	else if (type == 3) //blue
		outCol = vec4(time,time,channelCol.b,1.0);
	else if (type == 4) //purple
		outCol = vec4(channelCol.r,time,channelCol.b,1.0);
}