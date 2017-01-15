#pragma once

#include "Vec3.h"

void polar2Cartesian (float phi, float theta, float r, float & x, float & y, float & z) {
  x = r * sin (theta) * cos (phi);
  y = r * sin (theta) * sin (phi);
  z = r * cos (theta);
}

class LightSource{

  public:
    LightSource(Vec3f pos, Vec3<float> color, float intensity=1.0): pos(pos), color(color), intensity(intensity)
    {

    }

    Vec3f getPos() const {

      float x,y,z;
      polar2Cartesian(pos[1], pos[2], pos[0], x, y, z);
      return Vec3f(x,y,z);
     }

    void addP(float toAddP){

      pos[1] = std::fmod(pos[1] + toAddP,2*M_PI);
    }

    void addT(float toAddT){

      pos[2] = std::fmod(pos[2] + toAddT,2*M_PI);
    }

    void addR(float toAddR){

      pos[0] = fmax(0.0, pos[0] + toAddR);
    }

    void addLum(float toAddLum){

      intensity+=toAddLum;
    }

    float getIntensity() const {

      return intensity;
    }

    Vec3<float> getColor() const {

      return color;
    }

  private:
    Vec3<float> pos;
    Vec3<float> color;
    double intensity;
};
