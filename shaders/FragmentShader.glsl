#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 color;
in vec3 normal;
in vec2 texture;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec3 out_texture;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(color ,1);
    out_normal = vec4(normal, 1.f);
	out_texture = vec3(texture, 1.f);

}
