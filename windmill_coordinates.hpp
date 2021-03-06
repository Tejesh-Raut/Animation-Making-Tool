int num_objects2 = 3;

int num_vertices2[3] = {18, 9, 18};
int freedom_constraints2[3] = {0, 1, 3};
int parent2[3]=  {
					-1, // base
					0, // pivot
					1 // fan
				};

float x_trans2 = 0.0f;
float y_trans2 = 0.0f;
float z_trans2 = 0.0f;

std::vector<float> xrot2(num_objects2, 0.0f);
std::vector<float> yrot2(num_objects2, 0.0f);
std::vector<float> zrot2(num_objects2, 0.0f);


//Vertices in homogenous coordinates
glm::vec4 v_positions2[1000] = {

	// 0 base 21
	glm::vec4(0.2, -0.99, 0.55, 1.0),//base
	glm::vec4(0.2, -0.99, 0.45, 1.0),
	glm::vec4(0.3, -0.99, 0.45, 1.0),
	glm::vec4(0.2, -0.99, 0.55, 1.0),
	glm::vec4(0.3, -0.99, 0.55, 1.0),
	glm::vec4(0.3, -0.99, 0.45, 1.0),
	glm::vec4(0.2, -0.99, 0.55, 1.0),//front face
	glm::vec4(0.3, -0.99, 0.55, 1.0),
	glm::vec4(0.25, -0.6, 0.5, 1.0),
	glm::vec4(0.2, -0.99, 0.55, 1.0),//left face
	glm::vec4(0.2, -0.99, 0.45, 1.0),
	glm::vec4(0.25, -0.6, 0.5, 1.0),
	glm::vec4(0.3, -0.99, 0.55, 1.0),//right face
	glm::vec4(0.3, -0.99, 0.45, 1.0),
	glm::vec4(0.25, -0.6, 0.5, 1.0),
	glm::vec4(0.2, -0.99, 0.45, 1.0),//back face
	glm::vec4(0.3, -0.99, 0.45, 1.0),
	glm::vec4(0.25, -0.6, 0.5, 1.0),

	glm::vec4(0.245, -0.61, 0.5, 1.0),//pivot
	glm::vec4(0.255, -0.61, 0.5, 1.0),
	glm::vec4(0.25, -0.6, 0.6, 1.0),//windmill centre
	glm::vec4(0.245, -0.61, 0.5, 1.0),
	glm::vec4(0.25, -0.59, 0.5, 1.0),
	glm::vec4(0.25, -0.6, 0.6, 1.0),
	glm::vec4(0.255, -0.61, 0.5, 1.0),
	glm::vec4(0.25, -0.59, 0.5, 1.0),
	glm::vec4(0.25, -0.6, 0.6, 1.0),

	// 1 fan 18
	glm::vec4(0.25, -0.6, 0.6, 1.0),//first one
	glm::vec4(0.21, -0.41, 0.62, 1.0),
	glm::vec4(0.29, -0.41, 0.58, 1.0),
	glm::vec4(0.25, -0.6, 0.599, 1.0),
	glm::vec4(0.21, -0.41, 0.619, 1.0),
	glm::vec4(0.29, -0.41, 0.579, 1.0),
	glm::vec4(0.25, -0.6, 0.6, 1.0),//second one
	glm::vec4(0.10, -0.7, 0.62, 1.0),
	glm::vec4(0.06, -0.65, 0.58, 1.0),
	glm::vec4(0.25, -0.6, 0.599, 1.0),
	glm::vec4(0.10, -0.7, 0.619, 1.0),
	glm::vec4(0.06, -0.65, 0.579, 1.0),
	glm::vec4(0.25, -0.6, 0.6, 1.0),//third one
	glm::vec4(0.44, -0.65, 0.62, 1.0),
	glm::vec4(0.40, -0.7, 0.58, 1.0),
	glm::vec4(0.25, -0.6, 0.599, 1.0),
	glm::vec4(0.44, -0.65, 0.619, 1.0),
	glm::vec4(0.40, -0.7, 0.579, 1.0)
};

glm::vec4 v_normals2[1000] = {

	// 0 base 21
	glm::vec4(0.0, -1.0, 0.0, 1.0),//base
	glm::vec4(0.0, -1.0, 0.0, 1.0),
	glm::vec4(0.0, -1.0, 0.0, 1.0),
	glm::vec4(0.0, -1.0, 0.0, 1.0),
	glm::vec4(0.0, -1.0, 0.0, 1.0),
	glm::vec4(0.0, -1.0, 0.0, 1.0),
	glm::vec4(0.0, 0.03, 1.0, 1.0),//front face
	glm::vec4(0.0, 0.03, 1.0, 1.0),
	glm::vec4(0.0, 0.03, 1.0, 1.0),
	glm::vec4(-1.0, 0.03, 0.0, 1.0),//left face
	glm::vec4(-1.0, 0.03, 0.0, 1.0),
	glm::vec4(-1.0, 0.03, 0.0, 1.0),
	glm::vec4(1.0, 0.03, 0.0, 1.0),//right face
	glm::vec4(1.0, 0.03, 0.0, 1.0),
	glm::vec4(1.0, 0.03, 0.0, 1.0),
	glm::vec4(0.0, 0.03, -1.0, 1.0),//back face
	glm::vec4(0.0, 0.03, -1.0, 1.0),
	glm::vec4(0.0, 0.03, -1.0, 1.0),

	glm::vec4(0.0, 1.0, 0.1, 1.0),//pivot
	glm::vec4(0.0, 1.0, 0.1, 1.0),
	glm::vec4(0.0, 1.0, 0.1, 1.0),//windmill centre
	glm::vec4(-0.5, -0.5, 0.1, 1.0),
	glm::vec4(-0.5, -0.5, 0.1, 1.0),
	glm::vec4(-0.5, -0.5, 0.1, 1.0),
	glm::vec4(0.5, -0.5, 0.1, 1.0),
	glm::vec4(0.5, -0.5, 0.1, 1.0),
	glm::vec4(0.5, -0.5, 0.1, 1.0),

	// 1 fan 9
	glm::vec4(0.02, 0.0, 1.0, 1.0),//first one
	glm::vec4(0.02, 0.0, 1.0, 1.0),
	glm::vec4(0.02, 0.0, 1.0, 1.0),
	glm::vec4(-0.02, 0.0, -1.0, 1.0),
	glm::vec4(-0.02, 0.0, -1.0, 1.0),
	glm::vec4(-0.02, 0.0, -1.0, 1.0),
	glm::vec4(-0.01, 0.01, 1.0, 1.0),//second one
	glm::vec4(-0.01, 0.01, 1.0, 1.0),
	glm::vec4(-0.01, 0.01, 1.0, 1.0),
	glm::vec4(0.01, -0.01, -1.0, 1.0),
	glm::vec4(0.01, -0.01, -1.0, 1.0),
	glm::vec4(0.01, -0.01, -1.0, 1.0),
	glm::vec4(-0.01, -0.01, 1.0, 1.0),//third one
	glm::vec4(-0.01, -0.01, 1.0, 1.0),
	glm::vec4(-0.01, -0.01, 1.0, 1.0),
	glm::vec4(0.01, 0.01, -1.0, 1.0),
	glm::vec4(0.01, 0.01, -1.0, 1.0),
	glm::vec4(0.01, 0.01, -1.0, 1.0)
};

//RGBA colors
glm::vec4 v_colors2[1000] = {
	glm::vec4(0.1, 0.2, 0.3, 1.0),//base
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	glm::vec4(0.1, 0.2, 0.3, 1.0),
	
	glm::vec4(1.0, 0.0, 0.0, 1.0),//pivot
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),

	glm::vec4(1.0, 0.0, 0.0, 1.0),//fan1
	glm::vec4(0.0, 1.0, 0.0, 1.0),
	glm::vec4(0.0, 0.0, 1.5, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),//fan2
	glm::vec4(0.0, 1.0, 0.0, 1.0),
	glm::vec4(0.0, 0.0, 1.5, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 0.0, 0.0, 1.0),//fan3
	glm::vec4(0.0, 1.0, 0.0, 1.0),
	glm::vec4(0.0, 0.0, 1.5, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 1.0, 1.0)
};