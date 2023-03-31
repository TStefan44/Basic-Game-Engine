#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec2 v_uv;
varying vec3 v_posS;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;

uniform vec3 u_cameraPos;

uniform float u_fog_r;
uniform float u_fog_R;
uniform vec3 u_fogColor;

uniform float u_Time;
uniform float u_dispMax;

void main()
{
	// Calculate displacement
	vec2 disp = (vec4(1.0,1.0,1.0,1.0) - texture2D(u_texture_2, vec2(v_uv.x, v_uv.y + u_Time))).rg;
	vec2 offset = ((disp * 2.0) - vec2(1.0, 1.0)) * u_dispMax;
	vec2 v_uv_displaced = v_uv + offset;

	// Calculate fragment color
	vec4 color =  texture2D(u_texture_1, v_uv_displaced);
	vec4 c_alpha = texture2D(u_texture_0, v_uv);
	color.a = color.a * c_alpha.r;
	if (color.a < 0.2) {
		discard;
	}

	// Calculate fog
	float dis = distance(u_cameraPos, v_posS);
	float alpha = clamp((dis - u_fog_r) / (u_fog_R - u_fog_r), 0.0, 1.0);

	// set fragment color
	gl_FragColor = alpha * vec4(u_fogColor, 1) + (1.0 - alpha) * color;
}
