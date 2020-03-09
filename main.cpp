#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include "geometry.h"
#include "model.h"
#include "rasterizer.hpp"


Model *model = NULL;

Vec3f world2screen(Vec3f v) {
    return Vec3f(int((v.x+1.)*width/2.), int((v.y+1.)*height/2.), v.z);
}

void render()
{
    float *zbuffer = new float[width*height];
    for (int i = 0; i < width * height; i++)
    {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }
    std::vector<Vec3f> framebuffer(width * height);


    Vec3f light_dir(0,0,-1);
    model = new Model("obj/diablo3_pose.obj");
    for (int i = 0; i < model->nfaces(); i++){
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3]; 
        Vec3f pts[3];
        for (int j=0; j<3; j++) pts[j] = world2screen(model->vert(face[j]));
        for (int j = 0; j < 3; j++){
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec2i((v.x + 1.)*width/2.,(v.y + 1.)*height/2.);
            world_coords[j]  = v; 
        }

        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0){
            triangle(framebuffer,zbuffer, pts, Vec3f(intensity,intensity,intensity)); 
        }
    }

    std::vector<Vec3f> framebuffer_tmp(width * height);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
           // if ( type ==  HORIZONTAL_FLIP ) {
                //framebuffer_tmp[ x + y * width ] = framebuffer[ ( width - 1 - x ) + ( y ) * width ];
            //} else {  
                framebuffer_tmp[ x + y * width ] = framebuffer[ x + (height - 1 - y ) * width ];
            //  }
        }
    }
    
    framebuffer = framebuffer_tmp;
    
    

    std::ofstream ofs; // save the framebuffer to file
    ofs.open("./out.ppm");
    ofs << "P6\n"<< width << " " << height << std::endl << "\n255\n";
    for (size_t i = 0; i < height * width; ++i)
    {
        for (size_t j = 0; j < 3; j++)
        {
            ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();

    delete model;
}

int main(int argc, char **argv)
{

    render();

    return 0;
}
