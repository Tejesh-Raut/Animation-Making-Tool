glm::vec2 tex_coords_env[36] = {
	glm::vec2( 0.0, 0.0),
	glm::vec2( 0.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 0.0),
	glm::vec2( 0.0, 0.0),

	glm::vec2( 0.0, 0.0),
	glm::vec2( 0.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 0.0),
	glm::vec2( 0.0, 0.0),

	glm::vec2( 0.0, 0.0),
	glm::vec2( 0.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 0.0),
	glm::vec2( 0.0, 0.0),

	glm::vec2( 0.0, 0.0),
	glm::vec2( 0.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 0.0),
	glm::vec2( 0.0, 0.0),

	glm::vec2( 0.0, 0.0),
	glm::vec2( 0.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 0.0),
	glm::vec2( 0.0, 0.0),

	glm::vec2( 0.0, 0.0),
	glm::vec2( 0.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 1.0),
	glm::vec2( 1.0, 0.0),
	glm::vec2( 0.0, 0.0)
};

glm::vec4 v_positions_env[36] = {

	glm::vec4(-5.0, 5.0, 5.0, 1.0), // top face
	glm::vec4(-5.0, 5.0, -5.0, 1.0),
	glm::vec4(5.0, 5.0, -5.0, 1.0),
	glm::vec4(5.0, 5.0, -5.0, 1.0),
	glm::vec4(5.0, 5.0, 5.0, 1.0),
	glm::vec4(-5.0, 5.0, 5.0, 1.0),

	glm::vec4(-5.0, -1.0, -5.0, 1.0), // back face
	glm::vec4(-5.0, 5.0, -5.0, 1.0),
	glm::vec4(5.0, 5.0, -5.0, 1.0),
	glm::vec4(5.0, 5.0, -5.0, 1.0),
	glm::vec4(5.0, -1.0, -5.0, 1.0),
	glm::vec4(-5.0, -1.0, -5.0, 1.0),

	glm::vec4(-5.0, -1.0, -5.0, 1.0), // left face
	glm::vec4(-5.0, 5.0, -5.0, 1.0),
	glm::vec4(-5.0, 5.0, 5.0, 1.0),
	glm::vec4(-5.0, 5.0, 5.0, 1.0),
	glm::vec4(-5.0, -1.0, 5.0, 1.0),
	glm::vec4(-5.0, -1.0, -5.0, 1.0),

	glm::vec4(5.0, -1.0, 5.0, 1.0), // right face
	glm::vec4(5.0, 5.0, 5.0, 1.0),
	glm::vec4(5.0, 5.0, -5.0, 1.0),
	glm::vec4(5.0, 5.0, -5.0, 1.0),
	glm::vec4(5.0, -1.0, -5.0, 1.0),
	glm::vec4(5.0, -1.0, 5.0, 1.0),

	glm::vec4(-5.0, -1.0, 5.0, 1.0), // front face
	glm::vec4(-5.0, 5.0, 5.0, 1.0),
	glm::vec4(5.0, 5.0, 5.0, 1.0),
	glm::vec4(5.0, 5.0, 5.0, 1.0),
	glm::vec4(5.0, -1.0, 5.0, 1.0),
	glm::vec4(-5.0, -1.0, 5.0, 1.0),

	glm::vec4(-5.0, -1.0, 5.0, 1.0), // bottom face
	glm::vec4(-5.0, -1.0, -5.0, 1.0),
	glm::vec4(5.0, -1.0, -5.0, 1.0),
	glm::vec4(5.0, -1.0, -5.0, 1.0),
	glm::vec4(5.0, -1.0, 5.0, 1.0),
	glm::vec4(-5.0, -1.0, 5.0, 1.0)
};

glm::vec4 v_normals_env[36] = {

	glm::vec4(0.0, -1.0, 0.0, 1.0),
	glm::vec4(0.0, -1.0, 0.0, 1.0),
	glm::vec4(0.0, -1.0, 0.0, 1.0),
	glm::vec4(0.0, -1.0, 0.0, 1.0),
	glm::vec4(0.0, -1.0, 0.0, 1.0),
	glm::vec4(0.0, -1.0, 0.0, 1.0),

	glm::vec4(0.0, 0.0, 1.0, 1.0),
	glm::vec4(0.0, 0.0, 1.0, 1.0),
	glm::vec4(0.0, 0.0, 1.0, 1.0),
	glm::vec4(0.0, 0.0, 1.0, 1.0),
	glm::vec4(0.0, 0.0, 1.0, 1.0),
	glm::vec4(0.0, 0.0, 1.0, 1.0),

	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),

	glm::vec4(-1.0, 0.0, 0.0, 1.0),
	glm::vec4(-1.0, 0.0, 0.0, 1.0),
	glm::vec4(-1.0, 0.0, 0.0, 1.0),
	glm::vec4(-1.0, 0.0, 0.0, 1.0),
	glm::vec4(-1.0, 0.0, 0.0, 1.0),
	glm::vec4(-1.0, 0.0, 0.0, 1.0),

	glm::vec4(0.0, 0.0, -1.0, 1.0),
	glm::vec4(0.0, 0.0, -1.0, 1.0),
	glm::vec4(0.0, 0.0, -1.0, 1.0),
	glm::vec4(0.0, 0.0, -1.0, 1.0),
	glm::vec4(0.0, 0.0, -1.0, 1.0),
	glm::vec4(0.0, 0.0, -1.0, 1.0),

	glm::vec4(0.0, 1.0, 0.0, 1.0),
	glm::vec4(0.0, 1.0, 0.0, 1.0),
	glm::vec4(0.0, 1.0, 0.0, 1.0),
	glm::vec4(0.0, 1.0, 0.0, 1.0),
	glm::vec4(0.0, 1.0, 0.0, 1.0),
	glm::vec4(0.0, 1.0, 0.0, 1.0)

};

glm::vec4 v_colors_env[36] = {
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),

	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),

	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),

	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),

	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),

	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0)

};