#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec3 v_Color;
varying vec2 v_uv;

uniform sampler2D u_texture_0;

void main()
{
	// Set fragment color
	vec4 color =  texture2D(u_texture_0, v_uv);
	if (color.a < 0.2) {
		discard;
	}
	gl_FragColor = texture2D(u_texture_0, v_uv);
}
