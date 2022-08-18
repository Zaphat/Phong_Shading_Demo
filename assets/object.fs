#version 330 core
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;


uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {


	// ambient component
	vec3 ambient = light.ambient * material.ambient;

	// diffuse component
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular component
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	//Phong specular
		//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 512.0);
	//Blinn specular
		vec3 halfWay = (viewDir+lightDir)/length(viewDir+lightDir);
		float spec = pow(max(dot(norm,halfWay),0.0),material.shininess * 512.0);
	vec3 specular = light.specular * (spec * material.specular);

	//Combined
	FragColor = vec4(vec3(ambient + diffuse + specular), 1.0);
} 
