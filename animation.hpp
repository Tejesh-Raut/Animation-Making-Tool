/*
  CS475 Assignment 4

  Modified from Tutorial 4

  Written by Tejesh Raut and Ashna Gaur, 2017
*/
#ifndef _COLORCUBE_HPP_
#define _COLORCUBE_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

int mywindow_width = 512;
int mywindow_height = 512;

GLuint vPosition;
GLuint vColor;
GLuint vNormal;
GLuint dir_light1;
GLuint dir_light2;
GLuint spot_light;

float cam_x = 1.5f;
float cam_y = 1.5f;
float cam_z = 1.5f;

int frames_per_sec = 20;
int num_sub_frames = 60; // number of interpolated frames between two keyframes
//-------------------------------------------------------------------------

#endif
