#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int deformation;

uniform vec3 object_color;
uniform mediump float u_time;

// Output value to fragment shader
out vec3 color;
out vec3 normal;
out vec2 texture;


float rand(float n){return fract(sin(n) * 43758.5453123);}
//pentru functia de noise am folosit acest link https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83

void main()
{
     //pentru modelul de cum creez deformarea m am bazat pe acest link 
	 float varf = 0.0;
	 float size = 1.0;
     vec3 posistion = v_position ;
    
     if(deformation == 1) {
     for( int i = 0; i < 100; i++ )
	 {
		varf += rand( i*length(posistion.xy)  )/ size; //length ca deformarea sa fie aproape same size ca si meshul
		size *= 1.5;
	 }
   

     vec3 c = mix(vec3(.23,.72,.21), vec3(.53,.12,.01), varf) * 50;
	
	 posistion = v_position * varf;
    
    // TODO(student): Compute world space vectors
    vec3 world_position = vec3 (Model * vec4(posistion, 1));
	vec3 world_normal = normalize (vec3 (Model * vec4(v_normal, 0)));

    vec3 L = normalize (light_position - world_position);
	vec3 V = normalize (eye_position - world_position);
	vec3 H = normalize (L + V);
	vec3 R = reflect(-L,world_normal);

    // TODO(student): Define ambient light component
    float ambient_light = 0.25 * material_kd;

    // TODO(student): Compute diffuse light component
    float diffuse_light =  material_kd * max(dot(world_normal,L), 0);

    // TODO(student): Compute specular light component
    float specular_light = 0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    if (diffuse_light > 0)
    {
            specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

    // TODO(student): Compute light
    float d = distance(light_position, world_position);
    float factorAtenuare 	= 1 / (1 + pow(d,2));
	float culoare = ambient_light + factorAtenuare * (diffuse_light + specular_light);

    // TODO(student): Send color light output to fragment shader
    color = vec3(1);
    color = object_color * culoare * c;

     normal = v_normal;
   texture = v_texture_coord;

   gl_Position = Projection * View * Model * vec4(posistion, 1.0);
   } 
   else {
    vec3 world_position = vec3 (Model * vec4(v_position, 1));
	vec3 world_normal = normalize (vec3 (Model * vec4(v_normal, 0)));

    vec3 L = normalize (light_position - world_position);
	vec3 V = normalize (eye_position - world_position);
	vec3 H = normalize (L + V);
	vec3 R = reflect(-L,world_normal);

    // TODO(student): Define ambient light component
    float ambient_light = 0.25 * material_kd;

    // TODO(student): Compute diffuse light component
    float diffuse_light =  material_kd * max(dot(world_normal,L), 0);

    // TODO(student): Compute specular light component
    float specular_light = 0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    if (diffuse_light > 0)
    {
            specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

    // TODO(student): Compute light
    float d = distance(light_position, world_position);
    float factorAtenuare 	= 1 / (1 + pow(d,2));
	float culoare = ambient_light + factorAtenuare * (diffuse_light + specular_light);

    // TODO(student): Send color light output to fragment shader
    color = vec3(1);
    color = object_color * culoare;
   

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    }
}
