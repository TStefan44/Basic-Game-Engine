#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

const int MAX_LIGHTS = 5;

struct light_source
{
   int type;
   vec3 light_position;
   vec3 light_direction;
   vec3 light_color;
   vec3 spec_light_color;
   vec3 diff_light_color;

   // Spotlight component
   float cut_off;
};

varying vec2 v_uv;
varying vec3 v_posS;
varying vec3 v_world_normal;
varying vec3 v_world_position;

uniform sampler2D u_texture_0;

uniform vec3 u_cameraPos;

uniform float u_fog_r;
uniform float u_fog_R;
uniform vec3 u_fogColor;

uniform float u_ratio;
uniform vec3 u_colorAmbientalGlobal;
uniform float u_material_kd;
uniform float u_material_ks;
uniform float u_material_shininess;
uniform int u_light_number;
uniform light_source u_lights[MAX_LIGHTS];

vec3 glob_L;


vec4 light_contribution(light_source light, vec4 color) {
    vec3 diffuse_specular;
    float dist;
    float att_fact;

    float cut_off;
    float spot_light;
    float spot_light_limit;
    float linear_att = 0.0;
    float light_att_factor = 0.0;

    vec3 L;
    if (light.type == 0) {
        L = vec3(0.0, 1.0, 0.0);
    } else {
        L = normalize( light.light_position - v_world_position );
    }

    glob_L = L;

     if (light.type == 1) {
        dist = distance(light.light_position, v_world_position);
        att_fact = 1.0 / ( pow(dist, 2.0) * 0.1 + dist * 0.5 + 1.0 );

        //if (att_fact < 0.05)
            //return vec4(1, 1, 1, 1);
    } else if (light.type == 2) {
        cut_off = radians(light.cut_off);
        spot_light = dot(-L, light.light_direction);
        spot_light_limit = cos(cut_off);

        if (spot_light > spot_light_limit) {
            linear_att = (spot_light - spot_light_limit) / (1.0 - spot_light_limit);
            light_att_factor = pow(linear_att, 2.0);
        }

       // if (light_att_factor < 0.005)
                //return vec4(0, 0, 0, 0);
    }

    vec3 V = normalize( u_cameraPos - v_world_position );
    vec3 H = normalize( L + V );
    vec3 R =  reflect (-L, normalize(v_world_normal));

    float receive_light = dot( normalize(v_world_normal), L); 

    vec3 diffuse_light = u_material_kd * color.xyz * light.diff_light_color * max(receive_light, 0.0);
    vec3 specular_light = u_material_ks * light.spec_light_color * receive_light * pow(max(dot(V, R), 0.0), u_material_shininess );

    // Directional
    if (light.type == 0) {
        diffuse_specular = diffuse_light + specular_light;

    // Punctiform
    } else if (light.type == 1) {
        diffuse_specular = ( diffuse_light + specular_light );

    // Spotlight
    } else if (light.type == 2) {
        diffuse_specular = light_att_factor * ( diffuse_light + specular_light );
    }

    return vec4(diffuse_specular, 1);

}

void main()
{
	// Calculate fragment color
	vec4 color =  texture2D(u_texture_0, v_uv);
	if (color.a < 0.2) {
		discard;
	}

	// Calculate ambient color
	vec4 ambient_color = u_ratio * color * vec4(u_colorAmbientalGlobal, 1);

	// Calculate diffuse + specular color
	vec4 final_diffuse_specular = vec4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (i >= u_light_number)
			break;
		vec4 diffuse_specular = light_contribution(u_lights[i], color);
		final_diffuse_specular += diffuse_specular;
	}

	// Calculate final color
	vec4 final = ambient_color + (1.0 - u_ratio) * final_diffuse_specular;

	// Calculate fog
	float dis = distance(u_cameraPos, v_posS);
	float alpha = clamp((dis - u_fog_r) / (u_fog_R - u_fog_r), 0.0, 1.0);

	// set fragment color
	gl_FragColor = alpha * vec4(u_fogColor, 1) + (1.0 - alpha) * final;
    //gl_FragColor = vec4(glob_L, 1);
}
