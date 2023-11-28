#version 330 core //Koju verziju GLSL jezika da koristim (za programabilni pipeline: GL verzija * 100) i koji profil (core - samo programabilni pipeline, compatibility - core + zastarjele stvari)

//Kanali (in, out, uniform)
layout(location = 0) in vec2 inPos; //Pozicija tjemena
layout(location = 1) in vec3 inCol; //Boja tjemena - ovo saljemo u fragment sejder
out vec3 channelCol; //Izlazni kanal kroz koji saljemo boju do fragment sejdera
uniform vec2 uPos;
void main() //Glavna funkcija sejdera
{	

   gl_Position = vec4(inPos.x-uPos.x , inPos.y+uPos.y, 0.0, 1.0); //Pozitivan matematicki smer ukoliko je teme iznad x ose
	channelCol = inCol;
	
}