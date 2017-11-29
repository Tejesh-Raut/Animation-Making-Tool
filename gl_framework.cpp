#include "gl_framework.hpp"

extern int mywindow_width;
extern int mywindow_height;

extern float cam_x, cam_y, cam_z;

// 1st object
extern int num_objects;
extern int freedom_constraints[];

extern float x_trans, y_trans, z_trans;

extern std::vector<float> xrot;
extern std::vector<float> yrot;
extern std::vector<float> zrot;

// 2nd object
extern int num_objects2;
extern int freedom_constraints2[];

extern float x_trans2, y_trans2, z_trans2;

extern std::vector<float> xrot2;
extern std::vector<float> yrot2;
extern std::vector<float> zrot2;

extern float foV;

extern float dir_light1_status;
extern float dir_light2_status;
extern float spot_light_status;

extern std::vector<std::vector<float> > keyframes;
extern std::vector<std::vector<float> > bezier_camera_coordinates;
extern int num_keyframes;
extern int num_keys;
extern int num_sub_frames;

extern bool playback_mode;
extern bool use_bezier_camera_coordinates;
extern bool animation_completed;
extern int last_step;

extern std::string anim_name;

int curr_obj=0;
int curr_model=1;

std::vector<float> getBezierPoint(int num_original_points, std::vector<std::vector<float> > original_vertices, float ratio){
  if(num_original_points==1){
    return original_vertices[0];
  }
  std::vector<std::vector<float> > new_vertices;
  for(int i=0; i<(num_original_points -1); i++){
    std::vector<float> temp;
    temp.push_back(original_vertices[i][0]*(1-ratio) + original_vertices[i+1][0]*ratio);
    temp.push_back(original_vertices[i][1]*(1-ratio) + original_vertices[i+1][1]*ratio);
    temp.push_back(original_vertices[i][2]*(1-ratio) + original_vertices[i+1][2]*ratio);
    new_vertices.push_back(temp);
  }
  return (getBezierPoint((num_original_points-1), new_vertices, ratio));
}
void createBezierVertices(){
  int num_original_points = num_keyframes;
  int num_new_points = (num_keyframes -1)*num_sub_frames;
  float inc = 1.0f / num_new_points;
  float ratio = 0;
  bezier_camera_coordinates.clear();
  std::vector<std::vector<float> > original_vertices;
  for(int i=0; i<num_original_points; i++){
    std::vector<float> temp(3, 0.0f);
    temp[0] = keyframes[i][0];
    temp[1] = keyframes[i][1];
    temp[2] = keyframes[i][2];
    original_vertices.push_back(temp);
  }
  // std::cout<<"*"<<num_new_points<<std::endl;
  for (int i = 0; i < num_new_points; i++){
    std::vector<float> temp = getBezierPoint(num_original_points, original_vertices, ratio);
    bezier_camera_coordinates.push_back(temp);
    ratio += inc;
  }
  return;
}

namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW mouse callback
  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
  {
    
  }

  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_P && action == GLFW_PRESS){
      playback_mode = !playback_mode;
      animation_completed = false;
      last_step = -1;
      use_bezier_camera_coordinates = false;
      if(playback_mode){
        std::string s;
        std::cout<<"Enter name of keyframe file(with extension): "<<std::endl;
        std::cin>>s;
        anim_name = "";
        int len = s.length();
        for(int i=0; i<len; i++){
          if(s[i]=='.'){
            break;
          }
          anim_name += s[i];
        }
        std::cin.clear();
        std::ifstream loadfile;
        loadfile.open(s.c_str(), std::ifstream::in);
        if(loadfile.is_open()){
          std::cout<<"PLAYBACK"<<std::endl;
          keyframes.clear();
          num_keyframes = 0;
          double temp;
          num_keys = 3 + 1 + 3 + 3 + 3*num_objects + 3 + 3*num_objects2;
          while(loadfile>>temp){
            std::vector<float> curr_keyframe;
            curr_keyframe.push_back(temp);
            for(int i=0; i<(num_keys-1); i++){
              loadfile>>temp;
              curr_keyframe.push_back(temp);
            }
            keyframes.push_back(curr_keyframe);
          }
          num_keyframes = keyframes.size();
          glfwSetTime(0.0);
          if(num_keyframes == 0){
            std::cout<<"No keyframes found"<<std::endl;
            playback_mode = !playback_mode;
          }
          else{
            char c;
            std::cout<<"Use Bezier camera coordinates? (Y/N)"<<std::endl;
            std::cin>>c;
            if((c=='y')||(c=='Y')){
              bezier_camera_coordinates.clear();
              std::string sb;
              std::cout<<"Enter name of file containing Bezier camera coordinates(with extension):"<<std::endl;
              std::cin>>sb;
              std::cin.clear();
              std::ifstream loadfileb;
              loadfileb.open(sb.c_str(), std::ifstream::in);
              if(loadfileb.is_open()){
                int n = (num_keyframes -1)*num_sub_frames;
                use_bezier_camera_coordinates = true;
                for(int i=0; i<n; i++){
                  if(loadfileb>>temp){
                    std::vector<float> curr_coord;
                    curr_coord.push_back(temp);
                    loadfileb>>temp;
                    curr_coord.push_back(temp);
                    loadfileb>>temp;
                    curr_coord.push_back(temp);
                    bezier_camera_coordinates.push_back(curr_coord);
                  }
                  else{
                    std::cout<<"Number of coordinates in file are less than number of frames in animation"<<std::endl;
                    std::cout<<"Using simple interpolation of camera coordinates"<<std::endl;
                    use_bezier_camera_coordinates = false;
                    break;
                  }
                }
                // mode_t mt;
                // if(mkdir(anim_name.c_str(), mt)){
                // }
                // else{
                //   std::cout<<"Error creating the folder "<<anim_name<<std::endl;
                // }
              }
              else{
                std::cout<<"Cannot open "<<sb<<std::endl;
                std::cout<<"Using simple interpolation of camera coordinates"<<std::endl;
                use_bezier_camera_coordinates = false;
              }
            }
            else{
              std::cout<<"Using simple interpolation of camera coordinates"<<std::endl;
              use_bezier_camera_coordinates = false;
            }
          }
          // loadfile.close();
        }
        else{
          std::cout<<"Cannot open "<<s<<std::endl;
          playback_mode = !playback_mode;
        }
      }
      else{
        std::cout<<"RECORD"<<std::endl;
      }
    }
    else if(!playback_mode){
      if (key == GLFW_KEY_K && action == GLFW_PRESS){
        // code to save keyframe
        std::vector<float> curr_keyframe;

        curr_keyframe.push_back(cam_x); // camera coordinates
        curr_keyframe.push_back(cam_y);
        curr_keyframe.push_back(cam_z);
        curr_keyframe.push_back(foV); // field of view
        curr_keyframe.push_back(dir_light1_status); // light statuses
        curr_keyframe.push_back(dir_light2_status);
        curr_keyframe.push_back(spot_light_status);

        // Humanoid
        curr_keyframe.push_back(x_trans);
        curr_keyframe.push_back(y_trans);
        curr_keyframe.push_back(z_trans);

        for(int i=0; i<num_objects; i++){
          curr_keyframe.push_back(xrot[i]);
          curr_keyframe.push_back(yrot[i]);
          curr_keyframe.push_back(zrot[i]);
        }

        // Non-humanoid
        curr_keyframe.push_back(x_trans2);
        curr_keyframe.push_back(y_trans2);
        curr_keyframe.push_back(z_trans2);

        for(int i=0; i<num_objects2; i++){
          curr_keyframe.push_back(xrot2[i]);
          curr_keyframe.push_back(yrot2[i]);
          curr_keyframe.push_back(zrot2[i]);
        }

        keyframes.push_back(curr_keyframe);
        std::cout<<"Keyframe "<<keyframes.size()<<" captured!"<<std::endl;
      }
      else if (key == GLFW_KEY_L && action == GLFW_PRESS){
        // code to save keyframes to file
        std::ofstream savefile;
        std::string s, s1;
        std::cout<<"Enter name of file(without extension) for saving keyframes: "<<std::endl;
        std::cin>>s1;
        s = s1;
        s += ".txt";
        std::cin.clear();
        savefile.open(s.c_str());
        int keyframes_size = keyframes.size();
        int keyframe_size = 0;
        if(keyframes_size==0){
          keyframe_size = 0;
        }
        else{
          keyframe_size = keyframes[0].size();
        }
        for(int i=0; i<keyframes_size; i++){
          for(int j=0; j<(keyframe_size - 1); j++){
            savefile<<keyframes[i][j]<<" ";
          }
          savefile<<keyframes[i][keyframe_size - 1];
          savefile<<std::endl;
        }
        savefile.close();
        std::cout<<"All keyframes saved to the file "<<s<<std::endl;
        num_keyframes = keyframes.size();
        createBezierVertices();
        // std::cout<<"Created bezier vertices "<<std::endl;
        std::ofstream savefileb;
        std::string sb;
        sb = s1+"_bezier.txt";
        savefileb.open(sb.c_str());
        int n = (keyframes_size -1)*num_sub_frames;
        std::cout<<n<<" "<<bezier_camera_coordinates.size()<<std::endl;
        for(int i=0; i<n; i++){
          savefileb<<bezier_camera_coordinates[i][0]<<" ";
          savefileb<<bezier_camera_coordinates[i][1]<<" ";
          savefileb<<bezier_camera_coordinates[i][2]<<std::endl;
        }
        savefileb.close();
        std::cout<<"All bezier camera coordinates saved to the file "<<sb<<std::endl;
        // keyframes.clear();
        num_keyframes = 0;
      }
      else if (key == GLFW_KEY_W && action == GLFW_PRESS){
        if(curr_model==1){
          y_trans +=0.1;
          if((y_trans<0.01)&&(y_trans>-0.01)){
            y_trans =0;
          }
        }
        if(curr_model==2){
          y_trans2 +=0.1;
          if((y_trans2<0.01)&&(y_trans2>-0.01)){
            y_trans2 =0;
          }
        }
      }
      else if (key == GLFW_KEY_S && action == GLFW_PRESS){
        if(curr_model==1){
          y_trans -=0.1;
          if((y_trans<0.01)&&(y_trans>-0.01)){
            y_trans =0;
          }
        }
        if(curr_model==2){
          y_trans2 -=0.1;
          if((y_trans2<0.01)&&(y_trans2>-0.01)){
            y_trans2 =0;
          }
        }
      }
      else if (key == GLFW_KEY_A && action == GLFW_PRESS){
        if(curr_model==1){
          x_trans -=0.1;
          if((x_trans<0.01)&&(x_trans>-0.01)){
            x_trans =0;
          }
        }
        if(curr_model==2){
          x_trans2 -=0.1;
          if((x_trans2<0.01)&&(x_trans2>-0.01)){
            x_trans2 =0;
          }
        }
      }
      else if (key == GLFW_KEY_D && action == GLFW_PRESS){
        if(curr_model==1){
          x_trans +=0.1;
          if((x_trans<0.01)&&(x_trans>-0.01)){
            x_trans =0;
          }
        }
        if(curr_model==2){
          x_trans2 +=0.1;
          if((x_trans2<0.01)&&(x_trans2>-0.01)){
            x_trans2 =0;
          }
        }
      }
      else if (key == GLFW_KEY_Z && action == GLFW_PRESS){
        if(curr_model==1){
          z_trans -=0.1;
          if((z_trans<0.01)&&(z_trans>-0.01)){
            z_trans =0;
          }
        }
        if(curr_model==2){
          z_trans2 -=0.1;
          if((z_trans2<0.01)&&(z_trans2>-0.01)){
            z_trans2 =0;
          }
        }
      }
      else if (key == GLFW_KEY_X && action == GLFW_PRESS){
        if(curr_model==1){
          z_trans +=0.1;
          if((z_trans<0.01)&&(z_trans>-0.01)){
            z_trans =0;
          }
        }
        if(curr_model==2){
          z_trans2 +=0.1;
          if((z_trans2<0.01)&&(z_trans2>-0.01)){
            z_trans2 =0;
          }
        }
      }
      else if (key == GLFW_KEY_N && action == GLFW_PRESS){
        foV -= 5.0f;
        if((foV<10.0f)){
          foV =10.0f;
        }
        std::cout<<"Field of view: "<<foV<<" degrees"<<std::endl;
      }
      else if (key == GLFW_KEY_M && action == GLFW_PRESS){
        foV +=5.0f;
        if((foV>170.0f)){
          foV = 170.0f;
        }
        std::cout<<"Field of view: "<<foV<<" degrees"<<std::endl;
      }
      else if (key == GLFW_KEY_T && action == GLFW_PRESS){
        cam_y += 0.1;
        if((cam_y<0.01)&&(cam_y>-0.01)){
          cam_y =0;
        }
        std::cout<<"Camera coordinates: ( "<<cam_x<<", "<<cam_y<<", "<<cam_z<<")"<<std::endl;
      }
      else if (key == GLFW_KEY_G && action == GLFW_PRESS){
        cam_y -= 0.1;
        if((cam_y<0.01)&&(cam_y>-0.01)){
          cam_y =0;
        }
        std::cout<<"Camera coordinates: ( "<<cam_x<<", "<<cam_y<<", "<<cam_z<<")"<<std::endl;
      }
      else if (key == GLFW_KEY_F && action == GLFW_PRESS){
        cam_x -= 0.1;
        if((cam_x<0.01)&&(cam_x>-0.01)){
          cam_x =0;
        }
        std::cout<<"Camera coordinates: ( "<<cam_x<<", "<<cam_y<<", "<<cam_z<<")"<<std::endl;
      }
      else if (key == GLFW_KEY_H && action == GLFW_PRESS){
        cam_x += 0.1;
        if((cam_x<0.01)&&(cam_x>-0.01)){
          cam_x =0;
        }
        std::cout<<"Camera coordinates: ( "<<cam_x<<", "<<cam_y<<", "<<cam_z<<")"<<std::endl;
      }
      else if (key == GLFW_KEY_V && action == GLFW_PRESS){
        cam_z -= 0.1;
        if((cam_z<0.01)&&(cam_z>-0.01)){
          cam_z =0;
        }
        std::cout<<"Camera coordinates: ( "<<cam_x<<", "<<cam_y<<", "<<cam_z<<")"<<std::endl;
      }
      else if (key == GLFW_KEY_B && action == GLFW_PRESS){
        cam_z += 0.1;
        if((cam_z<0.01)&&(cam_z>-0.01)){
          cam_z =0;
        }
        std::cout<<"Camera coordinates: ( "<<cam_x<<", "<<cam_y<<", "<<cam_z<<")"<<std::endl;
      }
      else if (key == GLFW_KEY_LEFT ){
        if(curr_model==1){
          if((freedom_constraints[curr_obj]!=3)&&(freedom_constraints[curr_obj]!=1))
          {
            yrot[curr_obj] -= 1.0;
            // if(yrot[curr_obj]<=-360){
            //   yrot[curr_obj] += 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
        if(curr_model==2){
          if((freedom_constraints2[curr_obj]!=3)&&(freedom_constraints2[curr_obj]!=1))
          {
            yrot2[curr_obj] -= 1.0;
            // if(yrot2[curr_obj]<=-360){
            //   yrot2[curr_obj] += 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
      }
      else if (key == GLFW_KEY_RIGHT ){
        if(curr_model==1){
          if((freedom_constraints[curr_obj]!=3)&&(freedom_constraints[curr_obj]!=1))
          {
            yrot[curr_obj] += 1.0;
            // if(yrot[curr_obj]>=360){
            //   yrot[curr_obj] -= 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
        if(curr_model==2){
          if((freedom_constraints2[curr_obj]!=3)&&(freedom_constraints2[curr_obj]!=1))
          {
            yrot2[curr_obj] += 1.0;
            // if(yrot2[curr_obj]>=360){
            //   yrot2[curr_obj] -= 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
      }
      else if (key == GLFW_KEY_UP ){
        if(curr_model==1){
          if((freedom_constraints[curr_obj]!=3)&&(freedom_constraints[curr_obj]!=2))
          {
            xrot[curr_obj] -= 1.0;
            // if(xrot[curr_obj]<=-360){
            //   xrot[curr_obj] += 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
        if(curr_model==2){
          if((freedom_constraints2[curr_obj]!=3)&&(freedom_constraints2[curr_obj]!=2))
          {
            xrot2[curr_obj] -= 1.0;
            // if(xrot2[curr_obj]<=-360){
            //   xrot2[curr_obj] += 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
      }
      else if (key == GLFW_KEY_DOWN ){
        if(curr_model==1){
          if((freedom_constraints[curr_obj]!=3)&&(freedom_constraints[curr_obj]!=2))
          {
            xrot[curr_obj] += 1.0;
            // if(xrot[curr_obj]>=360){
            //   xrot[curr_obj] -= 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
        if(curr_model==2){
          if((freedom_constraints2[curr_obj]!=3)&&(freedom_constraints2[curr_obj]!=2))
          {
            xrot2[curr_obj] += 1.0;
            // if(xrot2[curr_obj]>=360){
            //   xrot2[curr_obj] -= 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
      }
      else if (key == GLFW_KEY_PAGE_UP ){
        if(curr_model==1){
          if((freedom_constraints[curr_obj]!=1)&&(freedom_constraints[curr_obj]!=2))
          {
            zrot[curr_obj] -= 1.0;
            // if(zrot[curr_obj]<=-360){
            //   zrot[curr_obj] += 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
        if(curr_model==2){
          if((freedom_constraints2[curr_obj]!=1)&&(freedom_constraints2[curr_obj]!=2))
          {
            zrot2[curr_obj] -= 1.0;
            // if(zrot2[curr_obj]<=-360){
            //   zrot2[curr_obj] += 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
      }
      else if (key == GLFW_KEY_PAGE_DOWN ){
        if(curr_model==1){
          if((freedom_constraints[curr_obj]!=1)&&(freedom_constraints[curr_obj]!=2))
          {
            zrot[curr_obj] += 1.0;
            // if(zrot[curr_obj]>=360){
            //   zrot[curr_obj] -= 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
        if(curr_model==2){
          if((freedom_constraints2[curr_obj]!=1)&&(freedom_constraints2[curr_obj]!=2))
          {
            zrot2[curr_obj] += 1.0;
            // if(zrot2[curr_obj]>=360){
            //   zrot2[curr_obj] -= 360;
            // }
          }
          else{
            std::cout<<"Can't rotate along this axis"<<std::endl;
          }
        }
      }
      else if (key == GLFW_KEY_1 && action == GLFW_PRESS){
        curr_obj--;
        if(curr_obj<0){
          curr_obj = 0;
        }
        std::cout<<"Selected object: "<<curr_obj<<std::endl;
      }
      else if (key == GLFW_KEY_2 && action == GLFW_PRESS){
        curr_obj++;
        if(curr_model==1){
          if(curr_obj>=num_objects){
            curr_obj = num_objects-1;
          }
        }
        if(curr_model==2){
          if(curr_obj>=num_objects2){
            curr_obj = num_objects2-1;
          }
        }
        std::cout<<"Selected object: "<<curr_obj<<std::endl;
      }
      else if (key == GLFW_KEY_3 && action == GLFW_PRESS){
        if(curr_model==1){
          curr_model = 2;
          curr_obj = 0;
          std::cout<<"Simple toy Selected"<<std::endl;
        }
        else{
          curr_model = 1;
          curr_obj = 0;
          std::cout<<"Humonoid toy Selected"<<std::endl;
        }
      }
      else if (key == GLFW_KEY_4 && action == GLFW_PRESS){
        if(dir_light1_status>0.5){
          dir_light1_status = 0.0f;
          std::cout<<"Turned off first directional light "<<std::endl;
        }
        else{
          dir_light1_status = 1.0f;
          std::cout<<"Turned on first directional light "<<std::endl;
        }
      }
      else if (key == GLFW_KEY_5 && action == GLFW_PRESS){
        if(dir_light2_status>0.5){
          dir_light2_status = 0.0f;
          std::cout<<"Turned off second directional light "<<std::endl;
        }
        else{
          dir_light2_status = 1.0f;
          std::cout<<"Turned on second directional light "<<std::endl;
        }
      }
      else if (key == GLFW_KEY_6 && action == GLFW_PRESS){
        if(spot_light_status>0.5){
          spot_light_status = 0.0f;
          std::cout<<"Turned off the spot-light"<<std::endl;
        }
        else{
          spot_light_status = 1.0f;
          std::cout<<"Turned on the spot-light"<<std::endl;
        }
      }
    }
  }
};  
  


