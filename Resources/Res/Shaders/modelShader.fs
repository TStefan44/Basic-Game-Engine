#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec2 v_uv;
varying vec3 v_posS;

uniform sampler2D u_texture_0;

uniform vec3 u_cameraPos;

uniform float u_fog_r;
uniform float u_fog_R;
uniform vec3 u_fogColor;

void main()
{
	// Calculate fragment color
	vec4 color =  texture2D(u_texture_0, v_uv);
	if (color.a < 0.2) {
		discard;
	}

	// Calculate fog
	float dis = distance(u_cameraPos, v_posS);
	float alpha = clamp((dis - u_fog_r) / (u_fog_R - u_fog_r), 0.0, 1.0);

	// set fragment color
	gl_FragColor = alpha * vec4(u_fogColor, 1) + (1.0 - alpha) * color;
}
