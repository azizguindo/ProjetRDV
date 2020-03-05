#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "Utils.hpp"


Model *model = NULL;
const int width  = 800;
const int height = 800;



int main(int argc, char** argv) {
	model = new Model("obj/diablo3_pose.obj");
	TGAImage image(width, height, TGAImage::RGB);
	for (int i=0; i<model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vec2i screen_coords[3];
    for (int j=0; j<3; j++) {
        Vec3f world_coords = model->vert(face[j]);
        screen_coords[j] = Vec2i((world_coords.x+1.)*width/2., (world_coords.y+1.)*height/2.);
    }
    triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(rand()%255, rand()%255, rand()%255, 255));

	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	delete model;
	return 0;
}
