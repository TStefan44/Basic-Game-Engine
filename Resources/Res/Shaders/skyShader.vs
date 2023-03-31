#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

// Vertex attributes
attribute vec3 a_posL;

varying vec3 v_coord;

// Uniform data
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_persp;

void main()
{

	// Calculate vertex position
	vec4 posL = vec4(a_posL, 1.0);
	posL = u_persp * u_view * u_model * posL;
	gl_Position = posL;

	// calculate fragment data
	v_coord = a_posL;
}
   