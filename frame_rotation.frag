#version 330 core //Koju verziju GLSL jezika da koristim (za programabilni pipeline: GL verzija * 100) i koji profil (core - samo programabilni pipeline, compatibility - core + zastarjele stvari)

//Kanali (in, out, uniform)
in vec3 channelCol; //Kanal iz Verteks sejdera - mora biti ISTOG IMENA I TIPA kao u vertex sejderu
out vec4 outCol; //Izlazni kanal koji ce biti zavrsna boja tjemena (ukoliko se ispisuju podaci u memoriju, koristiti layout kao za ulaze verteks sejdera)
uniform float time;
uniform float type;

void main() //Glavna funkcija sejdera
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