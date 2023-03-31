#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec3 v_coord;

uniform samplerCube u_texture_0;

void main()
{
	// Set fragment color
	gl_FragColor = textureCube(u_texture_0, v_coord);
}
