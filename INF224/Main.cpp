#include <iostream>
#include <string>
#include <algorithm>

#include "MultimediaObject.h"
#include "Video.h"
#include "Picture.h"

int main (int argc, char ** argv) {

  MultimediaObject** multimediaObjectArray = new MultimediaObject*[2];
  multimediaObjectArray[0] = new Video("FILM1", "film1.mp3", 120);
  multimediaObjectArray[1] = new Picture("PIC1", "pic1.png", 1920.0, 1080.0);

  multimediaObjectArray[0]->play();
  multimediaObjectArray[1]->play();

  return 0;
}
