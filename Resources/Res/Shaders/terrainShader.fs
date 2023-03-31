#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec2 v_uv;
varying vec2 v_uv2;
varying vec3 v_posS;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;

uniform vec3 u_cameraPos;

uniform float u_fog_r;
uniform float u_fog_R;
uniform vec3 u_fogColor;

void main()
{
	// Calculate fragment color
	vec4 c_dirt =  texture2D(u_texture_0, v_uv);
	vec4 c_rock =  texture2D(u_texture_1, v_uv);
	vec4 c_grass =  texture2D(u_texture_2, v_uv);
	vec4 c_blend =  texture2D(u_texture_3, v_uv2);
	vec4 c_final;

	// Use blend texture
	c_final = c_blend.r * c_rock + c_blend.g * c_grass + c_blend.b * c_dirt;
	c_final.a = 1.0;

	// Calculate fog
	float dis = distance(u_cameraPos, v_posS);
	float alpha = clamp((dis - u_fog_r) / (u_fog_R - u_fog_r), 0.0, 1.0);

	// Set fragment color
	gl_FragColor = alpha * vec4(u_fogColor, 1) + (1.0 - alpha) * c_final;
}
