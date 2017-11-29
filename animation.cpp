/*
  CS475 Assignment 4

  Modified from Tutorial 4

  Written by Tejesh Raut and Ashna Gaur, 2017
*/


#include "animation.hpp"
#include "texture.hpp"
#include "humanoid_coordinates.hpp"
#include "windmill_coordinates.hpp"
#include "cube_coordinates.hpp"

GLuint shaderProgram;
GLuint vbo, vao, vbo2, vao2, vbo_env, vao_env;
GLuint uModelViewMatrix;
GLuint normalMatrix;
GLuint viewMatrix;
GLuint tex1, tex_env1, tex_env2;


float foV = 45.0f;

float dir_light1_status = 1.0f;
float dir_light2_status = 1.0f;
float spot_light_status = 1.0f;

std::vector<std::vector<float> > keyframes;
std::vector<std::vector<float> > bezier_camera_coordinates;
int num_keyframes = 0;
int num_keys = 0;

bool playback_mode = false;
bool use_bezier_camera_coordinates = false;
bool animation_completed = false;
int last_step = -1;
std::string anim_name = "image_frames/default_animation";
//-----------------------------------------------------------------
void take_screenshot(int framenum){
  unsigned char *pRGB = new unsigned char [3 * (mywindow_width+1) * (mywindow_height + 1) ];
  glReadBuffer(GL_BACK);
  glPixelStoref(GL_PACK_ALIGNMENT,1);//for word allignment
  glReadPixels(0, 0, mywindow_width, mywindow_height, GL_RGB, GL_UNSIGNED_BYTE, pRGB);
  char filename[300];
  // filename = anim_name;
  std::string anim_name1 = "image_frames/"+anim_name;
  strcpy(filename, anim_name1.c_str());
  // sprintf(filename, anim_name.c_str());
  sprintf(filename+ strlen(filename), "_");
  sprintf(filename + strlen(filename),"frame_%04d.ppm",framenum);
  std::ofstream out(filename, std::ios::out);
  out<<"P6"<<std::endl;
  out<<mywindow_width<<" "<<mywindow_height<<" 255"<<std::endl;
  out.write(reinterpret_cast<char const *>(pRGB), (3 * (mywindow_width+1) * (mywindow_height + 1)) * sizeof(int));
  out.close();
  // std::cout<<"Saved "<<filename<<std::endl;
  delete pRGB;
  return;
}
void my_interpolation(int first_keyframe, int sub_frame){
  std::vector<float> curr_frame(num_keys, 0.0);
  if((first_keyframe<0)||(first_keyframe>=(num_keyframes -1))){
    return;
  }
  float t = ((float)sub_frame)/((float)num_sub_frames);
  for(int i=0; i<num_keys; i++){
    curr_frame[i] = (1-t)*keyframes[first_keyframe][i] + (t)*keyframes[first_keyframe+1][i];
  }
  cam_x = curr_frame[0];
  cam_y = curr_frame[1];
  cam_z = curr_frame[2];

  if(use_bezier_camera_coordinates){
    cam_x = bezier_camera_coordinates[first_keyframe*num_sub_frames + sub_frame][0];
    cam_y = bezier_camera_coordinates[first_keyframe*num_sub_frames + sub_frame][1];
    cam_z = bezier_camera_coordinates[first_keyframe*num_sub_frames + sub_frame][2];
  }

  foV = curr_frame[3];
  dir_light1_status = curr_frame[4];
  dir_light2_status = curr_frame[5];
  spot_light_status = curr_frame[6];
  x_trans = curr_frame[7];
  y_trans = curr_frame[8];
  z_trans = curr_frame[9];
  for(int i=0; i<num_objects; i++){
    xrot[i] = curr_frame[3*i + 10];
    yrot[i] = curr_frame[3*i + 11];
    zrot[i] = curr_frame[3*i + 12];
  }
  x_trans2 = curr_frame[10 + 3*num_objects];
  y_trans2 = curr_frame[11 + 3*num_objects];
  z_trans2 = curr_frame[12 + 3*num_objects];
  for(int i=0; i<num_objects2; i++){
    xrot2[i] = curr_frame[3*i + 3*num_objects + 13];
    yrot2[i] = curr_frame[3*i + 3*num_objects + 14];
    zrot2[i] = curr_frame[3*i + 3*num_objects + 15];
  }
  return;
}

void initBuffersGL(void)
{
  //----------------------------------------------------------------
  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (1, &vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao);

  //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (1, &vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (v_positions) + sizeof(v_colors) + sizeof(v_normals) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_colors), v_colors );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions)+ sizeof(v_colors), sizeof(v_normals), v_normals);
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions)+ sizeof(v_colors)+ sizeof(v_normals), sizeof(tex_coords), tex_coords);

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("my_vshader.glsl");
  std::string fragment_shader_file("my_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  vColor = glGetAttribLocation( shaderProgram, "vColor" );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions)) );
  vNormal = glGetAttribLocation(shaderProgram, "vNormal");
  glEnableVertexAttribArray( vNormal );
  glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions) + sizeof(v_colors)));
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
  normalMatrix =  glGetUniformLocation( shaderProgram, "normalMatrix");
  viewMatrix = glGetUniformLocation( shaderProgram, "viewMatrix");
  GLuint texCoord = glGetAttribLocation(shaderProgram, "texCoord");
  dir_light1 = glGetUniformLocation(shaderProgram, "dir_light1");
  dir_light2 = glGetUniformLocation(shaderProgram, "dir_light2");
  spot_light = glGetUniformLocation(shaderProgram, "spot_light");
  glEnableVertexAttribArray(texCoord);
  glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions) + sizeof(v_colors) +sizeof(v_normals)));
  // Load Textures
  tex1 = LoadTexture("images/shirt.bmp", 256, 256);
  glBindTexture(GL_TEXTURE_2D, tex1);
  //-------------------------------------------------------------------------
  glGenVertexArrays (1, &vao2);
  glBindVertexArray (vao2);

  glGenBuffers (1, &vbo2);
  glBindBuffer (GL_ARRAY_BUFFER, vbo2);
  glBufferData (GL_ARRAY_BUFFER, sizeof (v_positions2) + sizeof(v_colors2) + sizeof(v_normals2), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_positions2), v_positions2);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions2), sizeof(v_colors2), v_colors2);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_colors2)+sizeof(v_positions2), sizeof(v_normals2), v_normals2);
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions2)));
  glEnableVertexAttribArray(vNormal);
  glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions2)+ sizeof(v_colors2)));
  glEnableVertexAttribArray(texCoord);
  glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions2)+sizeof(v_colors2)+sizeof(v_normals2)));

  glGenVertexArrays(1, &vao_env);
  glBindVertexArray(vao_env);
  glGenBuffers(1, &vbo_env);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_env);
  glBufferData(GL_ARRAY_BUFFER, sizeof(v_positions_env) + sizeof(v_normals_env) + sizeof(tex_coords_env)+ sizeof(v_colors_env), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_positions_env), v_positions_env);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions_env), sizeof(v_normals_env), v_normals_env);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions_env)+sizeof(v_normals_env), sizeof(tex_coords_env), tex_coords_env);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions_env)+sizeof(v_normals_env)+sizeof(tex_coords_env), sizeof(v_colors_env), v_colors_env);
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vNormal);
  glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions_env)));
  glEnableVertexAttribArray(texCoord);
  glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions_env)+sizeof(v_normals_env)));
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions_env)+sizeof(v_normals_env)+ sizeof(tex_coords_env)));

  tex_env1 = LoadTexture("images/sky.bmp", 256, 256);
  tex_env2 = LoadTexture("images/grass.bmp", 256, 256);
}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glm::mat4 projection_matrix, view_matrix, modelview_matrix, lookat_matrix;
  glm::mat3 normal_matrix;
  std::vector<glm::mat4> model_matrix(num_objects, glm::mat4(1.0f));
  std::vector<glm::mat4> model_matrix2(num_objects2, glm::mat4(1.0f));

  int step = 0;
  if(playback_mode){
    double total_animation_time = ((double)(num_keyframes -1)*num_sub_frames)/((double) frames_per_sec);
    double curr_time = glfwGetTime();
    if(last_step==-1){
      curr_time = 0.0;
      animation_completed = false;
      glfwSetTime(curr_time);
    }
    if(curr_time>=total_animation_time){
      // std::cout<<"* "<<curr_time<<" "<<total_animation_time<<std::endl;
      curr_time = curr_time - total_animation_time;
      glfwSetTime(curr_time);
    }
    step = frames_per_sec*curr_time;
    if((step==0)&&(step!=last_step)&&(last_step!=-1)){
      if(!animation_completed){
        animation_completed = true;
        std::cout<<(last_step+1)<<" frames saved for making video"<<std::endl;
        std::cout<<"Use anim_name="<<anim_name<<" for making video"<<std::endl;
      }
    }
    if(step!=last_step){
      my_interpolation((step/num_sub_frames), (step%num_sub_frames));
    }
  }
  // creating the projection matrix
  projection_matrix = glm::mat4(1.0f);
  projection_matrix = glm::perspective(
                                        glm::radians(foV), // field of view
                                        (float) mywindow_width / (float)mywindow_height, // aspect ratio
                                        0.1f, // Near plane
                                        100.0f // far plane
                                      );
  lookat_matrix = glm::mat4(1.0f);
  lookat_matrix = glm::lookAt(glm::vec3(cam_x,cam_y,cam_z), // Camera is at (2,2,2), in World Space
      glm::vec3(0,0,0), // and looks at the point(0,0,-1)
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
  view_matrix = projection_matrix*lookat_matrix;
  glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
  glUniform1f(dir_light1, dir_light1_status);
  glUniform1f(dir_light2, dir_light2_status);
  glUniform1f(spot_light, spot_light_status);

  glBindVertexArray (vao);

  //root
  glm::mat4 translation_matrix, shift_to_origin_matrix, rotation_matrix, shift_back_matrix;
  translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x_trans, y_trans, z_trans));
  shift_to_origin_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(x_trans+v_positions[0][0]), -(y_trans+v_positions[0][1]), -(z_trans+v_positions[0][2])));
  rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(xrot[0]), glm::vec3(1.0f,0.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(yrot[0]), glm::vec3(0.0f,1.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(zrot[0]), glm::vec3(0.0f,0.0f,1.0f));
  shift_back_matrix = glm::translate(glm::mat4(1.0f), glm::vec3((x_trans+v_positions[0][0]), (y_trans+v_positions[0][1]), (z_trans+v_positions[0][2])));
  model_matrix[0] = shift_back_matrix*rotation_matrix*shift_to_origin_matrix*translation_matrix;

  modelview_matrix = view_matrix*model_matrix[0];
  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  normal_matrix = glm::transpose (glm::inverse(glm::mat3(modelview_matrix)));
  glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
  glBindTexture(GL_TEXTURE_2D, tex1);
  glDrawArrays(GL_TRIANGLES, 0, num_vertices[0]);
  int num_vertices_till_now = num_vertices[0];

  for(int i=1; i<num_objects; i++){
    glm::vec4 pivot = v_positions[num_vertices_till_now];
    shift_to_origin_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(pivot[0]), -(pivot[1]), -(pivot[2])));
    rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(xrot[i]), glm::vec3(1.0f,0.0f,0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, glm::radians(yrot[i]), glm::vec3(0.0f,1.0f,0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, glm::radians(zrot[i]), glm::vec3(0.0f,0.0f,1.0f));
    shift_back_matrix = glm::translate(glm::mat4(1.0f), glm::vec3((pivot[0]), (pivot[1]), (pivot[2])));
    model_matrix[i] = model_matrix[parent[i]]*shift_back_matrix*rotation_matrix*shift_to_origin_matrix;
    modelview_matrix = view_matrix*model_matrix[i];
    glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
    normal_matrix = glm::transpose (glm::inverse(glm::mat3(modelview_matrix)));
    glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
    glDrawArrays(GL_TRIANGLES, num_vertices_till_now, num_vertices[i]);
    num_vertices_till_now += num_vertices[i];
  }

  //----------------------------------------------------------------------------------------
  glBindVertexArray (vao2);
  translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x_trans2, y_trans2, z_trans2));
  shift_to_origin_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(x_trans2+v_positions2[0][0]), -(y_trans2+v_positions2[0][1]), -(z_trans2+v_positions2[0][2])));
  rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(xrot2[0]), glm::vec3(1.0f,0.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(yrot2[0]), glm::vec3(0.0f,1.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(zrot2[0]), glm::vec3(0.0f,0.0f,1.0f));
  shift_back_matrix = glm::translate(glm::mat4(1.0f), glm::vec3((x_trans2+v_positions2[0][0]), (y_trans2+v_positions2[0][1]), (z_trans2+v_positions2[0][2])));
  model_matrix2[0] = shift_back_matrix*rotation_matrix*shift_to_origin_matrix*translation_matrix;

  modelview_matrix = view_matrix*model_matrix2[0];
  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  normal_matrix = glm::transpose (glm::inverse(glm::mat3(modelview_matrix)));
  glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
  glDrawArrays(GL_TRIANGLES, 0, num_vertices2[0]);
  num_vertices_till_now = num_vertices2[0];

  for(int i=1; i<num_objects2; i++){
    glm::vec4 pivot = v_positions2[num_vertices_till_now];
    shift_to_origin_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(pivot[0]), -(pivot[1]), -(pivot[2])));
    rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(xrot2[i]), glm::vec3(1.0f,0.0f,0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, glm::radians(yrot2[i]), glm::vec3(0.0f,1.0f,0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, glm::radians(zrot2[i]), glm::vec3(0.0f,0.0f,1.0f));
    shift_back_matrix = glm::translate(glm::mat4(1.0f), glm::vec3((pivot[0]), (pivot[1]), (pivot[2])));
    model_matrix2[i] = model_matrix2[parent2[i]]*shift_back_matrix*rotation_matrix*shift_to_origin_matrix;
    modelview_matrix = view_matrix*model_matrix2[i];
    glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
    normal_matrix = glm::transpose (glm::inverse(glm::mat3(modelview_matrix)));
    glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
    glDrawArrays(GL_TRIANGLES, num_vertices_till_now, num_vertices2[i]);
    num_vertices_till_now += num_vertices2[i];
  }

  glBindVertexArray (vao_env);
  modelview_matrix = view_matrix;
  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  normal_matrix = glm::transpose(glm::inverse(glm::mat3(modelview_matrix)));
  glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
  glBindTexture(GL_TEXTURE_2D, tex_env1);
  glDrawArrays(GL_TRIANGLES, 0, 30);
  glBindTexture(GL_TEXTURE_2D, tex_env2);
  glDrawArrays(GL_TRIANGLES, 30, 6);

  if(playback_mode){
    if(step!=last_step){
      if(!animation_completed){
        // std::cout<<"call take_screenshot"<<step<<std::endl;
        take_screenshot(step);
      }
    }
    last_step = step;
  }
}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(mywindow_width, mywindow_height, "CS475/CS675 Assignment 4: Toys Animation", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //mouse button Callback
  glfwSetMouseButtonCallback(window, csX75::mouse_button_callback);
  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

