#version 330 core //Koju verziju GLSL jezika da koristim (za programabilni pipeline: GL verzija * 100) i koji profil (core - samo programabilni pipeline, compatibility - core + zastarjele stvari)

in vec4 channelCol; //Kanal iz Verteks sejdera - mora biti ISTOG IMENA I TIPA kao u vertex sejderu
out vec4 outCol; //Izlazni kanal koji ce biti zavrsna boja tjemena (ukoliko se ispisuju podaci u memoriju, koristiti layout kao za ulaze verteks sejdera)
uniform float time;

void main() //Glavna funkcija sejdera
{
	vec3 greenColor = vec3(0.0, 1.0, 0.0); // Normal green color
    vec3 darkGreenColor = vec3(0.0, 0.7, 0.0); // Darker green color

    // Interpolate between normal green and darker green based on time
    vec3 interpolatedColor = mix(greenColor, darkGreenColor, time);

    outCol = vec4(interpolatedColor, 1.0);
}