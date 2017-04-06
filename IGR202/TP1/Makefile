.PHONY: clean
CIBLE = main
SRCS =  Main.cpp Camera.cpp Mesh.cpp GLProgram.cpp GLShader.cpp GLError.cpp BVH.cpp LightRay.cpp
OPENGL_PATH = /usr/lib/nvidia-358 # change this for your own environment
LIBS = -L$(OPENGL_PATH) -lglut -lGLU -lGL -lGLEW -lm -lpthread

CC = g++
CXX = g++

FLAGS = -g -O3 -pthread -std=c++11

CFLAGS = $(FLAGS)
CXXFLAGS = $(FLAGS)

OBJS = $(SRCS:.cpp=.o)

$(CIBLE): $(OBJS)
	$(CXX) $^ -o $@ $(LIBS) 

-include $(subst .cpp,.d,$(SRCS))

%.d : %.cpp
	 $(CXX) -std=c++11 -MM -MF $@ -MP $<

clean:
	rm -f $(CIBLE)
	rm -f *.o
	rm -f *.d
run:
	./$(CIBLE)