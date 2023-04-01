#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Vertex attributes
attribute vec3 a_posL;
attribute vec2 a_uv;
attribute vec3 a_normal;

varying vec2 v_uv;
varying vec3 v_posS;
varying vec3 v_world_normal;
varying vec3 v_world_position;

// Uniform data
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_persp;

void main()
{
	// Calculate vertex position
	vec4 posL = vec4(a_posL, 1.0);
	vec4 posS = u_model * posL;
	gl_Position = u_persp * u_view * posS;

	// calculate fragment data
	v_uv = a_uv;
	v_posS = vec3(posS);
	v_world_normal = normalize(mat3(u_model) * a_normal);
	v_world_position = ( u_model * vec4(a_posL, 1) ).xyz;
}
   