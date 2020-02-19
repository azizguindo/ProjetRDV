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
	imodel = new Model("obj/diablo3_pose.obj");
	TGAImage image(width, height, TGAImage::RGB);
	for (int i=0; i<model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j=0; j<3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j+1)%3]);
			int x0 = (v0.x+1.)*width/2.;
			int y0 = (v0.y+1.)*height/2.;
			Vec2f p1(x0,y0);
			int x1 = (v1.x+1.)*width/2.;
			int y1 = (v1.y+1.)*height/2.;
			Vec2f p2(x1,y1);
			line(p1,p2, image, white);
		}
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	delete model;
	return 0;
}
