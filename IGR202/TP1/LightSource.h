#pragma once

#include "Vec3.h"

class LightSource{

  public:
    LightSource(Vec3<float> pos, Vec3<float> color, float intensity=1.0): pos(pos), color(color), intensity(intensity)
    {

    }

    Vec3<float> getPos(){

      return pos;
    }

    void addX(float toAddX){

      pos[0]+=toAddX;
    }

    void addY(float toAddY){

      pos[1]+=toAddY;
    }

    void addZ(float toAddZ){

      pos[2]+=toAddZ;
    }

    void addLum(float toAddLum){

      intensity+=toAddLum;
    }

    float getIntensity(){

      return intensity;
    }

    Vec3<float> getColor(){

      return color;
    }

  private:
    Vec3<float> pos;
    Vec3<float> color;
    double intensity;
};
