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

Vec3f world2screen_anaglyph(Vec3f v,int sep) {
    return Vec3f(int((v.x+1.)*width/2.)+sep, int((v.y+1.)*height/2.), v.z);
}

void render(bool _3d)
{
    const float sep   = 15;

    float *zbuffer = new float[width*height];
    float *zbuffer_blue = new float[width*height];
    float *zbuffer_red = new float[width*height];
    for (int i = 0; i < width * height; i++)
    {
        zbuffer[i] = -std::numeric_limits<float>::max();
        zbuffer_blue[i] = -std::numeric_limits<float>::max();
        zbuffer_red[i] = -std::numeric_limits<float>::max();
    }
    
    std::vector<Vec3f> framebuffer(width * height);

    std::vector<Vec3f> framebuffer_blue(width*height);
    std::vector<Vec3f> framebuffer_red(width*height);


    Vec3f light_dir(0,0,-1);
    model = new Model("obj/diablo3_pose.obj");
    for (int i = 0; i < model->nfaces(); i++){
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3]; 

        Vec3f pts[3];

        Vec3f pts_right[3];
        Vec3f pts_left[3];

        for (int j=0; j<3; j++){ 
            pts[j] = world2screen(model->vert(face[j]));
            pts_right[j] = world2screen_anaglyph(model->vert(face[j]),-sep);
            pts_left[j] = world2screen_anaglyph(model->vert(face[j]),sep);
        }
        for (int j = 0; j < 3; j++){
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec2i((v.x + 1.)*width/2.,(v.y + 1.)*height/2.);
            world_coords[j]  = v; 
        }

        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0){
            if(!_3d){
                triangle(framebuffer,zbuffer, pts, Vec3f(intensity,intensity,intensity)); 
            }
            else{
                triangle(framebuffer_blue,zbuffer_blue, pts_left, Vec3f(0,0,intensity)); 
                triangle(framebuffer_red,zbuffer_red, pts_right, Vec3f(intensity,0,0)); 
            }
        }
    }

    if(_3d){
        for(int i = 0 ;i<width*height;i++){
            framebuffer[i]= Vec3f(framebuffer_blue[i].x+framebuffer_red[i].x,
                                    framebuffer_blue[i].y+framebuffer_red[i].y,
                                    framebuffer_blue[i].z+framebuffer_red[i].z);
        }
    }

    std::vector<Vec3f> framebuffer_tmp(width * height);
    std::vector<Vec3f> anaglyph_tmp(width * height);
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++){
                framebuffer_tmp[ x + y * width ] = framebuffer[ x + (height - 1 - y ) * width ];
        }
    }
    
    framebuffer = framebuffer_tmp;
    


    std::ofstream ofs; 
    ofs.open("./out.ppm");
    ofs << "P6\n"<< width << " " << height << std::endl << "\n255\n";
    for (size_t i = 0; i < height * width; ++i){
        for (size_t j = 0; j < 3; j++){
            ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();

    delete model;
}

int main(int argc, char **argv)
{
    bool _3d = false;

    if(argc == 2){
        if(strcmp(argv[1], "3d")==0)
            _3d = true;
    }

    render(_3d);

    return 0;
}
