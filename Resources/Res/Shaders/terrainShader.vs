#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Vertex attributes
attribute vec3 a_posL;
attribute vec2 a_uv;

varying vec2 v_uv;
varying vec2 v_uv2;
varying vec3 v_posS;

// Uniform data
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_persp;

uniform float u_nr_cells;
uniform vec2 u_offset_uv2;
uniform vec3 u_height;

uniform sampler2D u_texture_3;

void main()
{

	// Calculate vertex position
	vec4 c_blend = texture2D(u_texture_3, a_uv);

	// Calculate vertex height
	vec4 posL = vec4(a_posL, 1.0);
	posL.y += c_blend.r * u_height.r + c_blend.g * u_height.g + c_blend.b * u_height.b;

	// Set vertex position
	vec4 posS = u_model * posL;
	gl_Position = u_persp * u_view * posS;

	// Calculate fragment data
	v_uv = a_uv;
	v_uv2 = vec2(a_uv.x / u_nr_cells, a_uv.y / u_nr_cells) + u_offset_uv2;
	v_posS = vec3(posS);
}
   