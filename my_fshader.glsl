#version 130

in vec3 normal;
in vec4 eye;
in vec4 COLOR;
in vec4 position;

uniform mat4 viewMatrix;
uniform sampler2D texture;
uniform float dir_light1;
uniform float dir_light2;
uniform float spot_light;

out vec4 frag_color;
varying vec2 tex;

void main () 
{
  // Defining Materials
  vec4 diffuse = vec4(1.0, 1.0, 1.0, 1.0); 
  vec4 ambient = vec4(0.05, 0.05, 0.05, 1.0);
  vec4 specular = vec4(0.5, 0.5, 0.5, 1.0);
  float shininess = 100.0;
  vec4 spec_1 = vec4(0.0); 
  vec4 spec_2 = vec4(0.0); 
  vec4 spec_3 = vec4(0.0);

  // Defining Light 
  vec4 lightPos_1 = vec4(0.5, 0.0, 1.0, 0.0);
  vec4 lightPos_2 = vec4(0.0, 1.0, 0.0, 0.0);
  vec3 lightDir_1 = vec3(viewMatrix * lightPos_1);  // Transforms with camera
  vec3 lightDir_2 = vec3(viewMatrix * lightPos_2);  // Transforms with camera
  lightDir_1 = normalize( vec3(lightDir_1));  
  lightDir_2 = normalize( vec3(lightDir_2));  
  vec4 lightPos_3 = vec4(5.0, 5.0, 4.0, 1.0);
  vec3 lightDir_3 = normalize(vec3(viewMatrix*lightPos_3 - position)); // Transforms with camera // position of light
  vec3 spotDir = normalize(vec3(viewMatrix*(lightPos_3))); // Transforms with camera // direction of axis of cone from pos to center(0,0,0)
  float spot_cutOff = 0.97;

  //Diffuse
  vec3 n = normalize(vec3(normal));	     
  float dotProduct_1 = dot(n, lightDir_1);
  float dotProduct_2 = dot(n, lightDir_2);
  float intensity_1 =  max( dotProduct_1, 0.0);
  float intensity_2 =  max( dotProduct_2, 0.0);
  float intensity_3 = 0.0;

  // Compute specular component only if light falls on vertex
  if(intensity_1 > 0.0)
  {
	vec3 e = normalize(vec3(eye));
	vec3 h_1 = normalize(lightDir_1 + e );
   	float intSpec_1 = max(dot(h_1,n), 0.0);	
        spec_1 = specular * pow(intSpec_1, shininess);
  }  
  if(intensity_2 > 0.0)
  {
	vec3 e = normalize(vec3(eye));
	vec3 h_2 = normalize(lightDir_2 + e );
   	float intSpec_2 = max(dot(h_2,n), 0.0);	
        spec_2 = specular * pow(intSpec_2, shininess);
  }  
  if(dot(spotDir, lightDir_3)> spot_cutOff){
  	intensity_3 = max(dot(n, lightDir_3), 0.0);
  	if(intensity_3>0.0){
  		vec3 e = normalize(vec3(eye));
  		vec3 h_3 = normalize(lightDir_3 + e);
  		float intSpec_3 = max(dot(h_3, n), 0.0);
  		spec_3 = specular * pow(intSpec_3, shininess);
  	}
  }

  // vec4 color = max(((intensity_1+intensity_2+intensity_3) * diffuse  + (spec_1+spec_2+spec_3))*COLOR , ambient*COLOR); // All
  // vec4 color = diffuse* COLOR; // Only Diffuse  
  // vec4 color = max(((intensity_3) * diffuse  + (spec_3))*COLOR , ambient*COLOR); // only spotlight
  vec4 color1 = vec4(0.0);
  if(dir_light1>0.5){
    color1 += (intensity_1*diffuse + spec_1)*texture2D(texture, tex)*COLOR;
  }
  if(dir_light2>0.5){
    color1 += (intensity_2*diffuse + spec_2)*texture2D(texture, tex)*COLOR;
  }
  if(spot_light>0.5){
    color1 += (intensity_3*diffuse + spec_3)*texture2D(texture, tex)*COLOR;
    // add code of adding spot-light here
  }
  vec4 color = max(color1 , ambient*texture2D(texture, tex)*COLOR);
  frag_color = color;
  // frag_color = texture2D(texture, tex);
}