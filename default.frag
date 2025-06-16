#version 330 core
out vec4 FragColor;

in vec3 color;

in vec3 Normal;
in vec3 crntPos;

uniform vec4 lightColor;

uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight(){

	//get distance between light and object
	vec3 lightVec = lightPos - crntPos; 
	float dist = length(lightVec);
	float a = 5.0;//constants for the dropoff of the light intensity
	float b = 0.9;
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);

	float ambient = 0.5f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);	// creating a vector to point towards the light source, to then be able to calculate the intensity of the diffuse lighting

	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.5f; // Max intensity of this type of light
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal); // want the vector to be towards the plane so we take the negative
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 32); //ctores amount of spec light with have at specific angle wrt our camera, 
	float specular = specularLight * specAmount;  

	return ((diffuse * intensity + ambient) + specular * intensity) * lightColor;
}


vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));	// point in opp direction then area you want to effect due to way we wrote the code
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return ((diffuse + ambient) + specular) * lightColor;
}


vec4 spotLight()
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return ((diffuse * inten + ambient) + specular * inten) * lightColor;
}

void main()
{
	
	FragColor = vec4(color, 1.0f); 
}