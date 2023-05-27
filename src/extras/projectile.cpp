#include <fstream>
#include <iostream>

#include "include/tuple.h"
#include "include/canvas.h"

struct Projectile
{
  Tuple position;
  Tuple velocity;
};

struct Environment
{
  Tuple gravity;
  Tuple wind;
};

Projectile tick(Environment env, Projectile proj){
  Tuple position = proj.position + proj.velocity;
  Tuple velocity = proj.velocity + env.gravity + env.wind;

  return Projectile(position, velocity);
}

int main(void){
  auto start    = Point(0,1,0);
  auto velocity = Vector(1,1.8,0).normalize() * 11.25;
  auto p        = Projectile(start, velocity);

  auto gravity  = Vector(0, -0.1, 0);
  auto wind     = Vector(-0.01, 0, 0);
  auto e        = Environment(gravity, wind);

  auto canvas = Canvas(900, 500);
  const auto orange = Color(1.0f, 0.65f, 0.0f); 

  while(p.position.y() > 0){
    std::cout <<  canvas.width-p.position.x()  << ' ' << canvas.height-p.position.y() << '\n';
    canvas.pixelWrite(orange, canvas.width-p.position.x()-1, canvas.height-p.position.y()-1);
    p = tick(e, p);
  }

  std::ofstream image{"Projectile.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);
}