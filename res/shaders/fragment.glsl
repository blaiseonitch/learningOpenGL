#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float opacity;

void main(){
	FragColor = mix(texture(texture1, TexCoord),
									texture(texture2, vec2(TexCoord.x, TexCoord.y)), opacity); //interpolates between both textures with 20% opacity difference (80% first texture)
}
