CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -I./

BIN=animation
SRCS=animation.cpp gl_framework.cpp shader_util.cpp texture.cpp
INCLUDES=gl_framework.hpp shader_util.hpp animation.hpp humanoid_coordinates.hpp windmill_coordinates.hpp cube_coordinates.hpp texture.hpp

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN)

jpg_flip:
	mogrify -flip -format jpg image_frames/$(anim_name)_frame*.ppm
	rm -rf image_frames/$(anim_name)_frame*.ppm

myvideo:
	ffmpeg -r 20 -f image2 -i 'image_frames/$(anim_name)_frame_%04d.jpg' -qscale 0 'video/$(anim_name).avi'
	echo "----------Video saved to the file video/$(anim_name).avi------------"

attach_sound:
	ffmpeg -i video/$(anim_name).avi -i sample_audio/audio1.mp3 -codec copy final_video/$(anim_name).avi
	echo "----------Final video saved to the file video/$(anim_name).avi------------"