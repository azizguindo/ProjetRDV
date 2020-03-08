#include <cmath>
#include <vector> 
#include "model.h"
#include "geometry.h"
const int width  = 800;
const int height = 800;

Vec3f barycentric(Vec2i *pts, Vec2i P);
void triangle(Vec2f t1,Vec2f t2,Vec2f t3,TGAImage & image,TGAColor color);
void line(std::vector<Vec3f> &framebuffer,int x0, int y0, int x1, int y1,Vec3f color);

Vec3f barycentric(Vec2i *pts, Vec2i P) { 
    Vec3f u = Vec3f(pts[2][0]-pts[0][0], pts[1][0]-pts[0][0], pts[0][0]-P[0])^ Vec3f(pts[2][1]-pts[0][1], pts[1][1]-pts[0][1], pts[0][1]-P[1]);
    /* `pts` and `P` has integer value as coordinates
       so `abs(u[2])` < 1 means `u[2]` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    if (std::abs(u[2])<1) return Vec3f(-1,1,1);
    return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z); 
} 
 
void triangle(std::vector<Vec3f> &framebuffer,Vec2i *pts, Vec3f color) {
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(width-1, height-1);

    for (int i=0; i<3; i++) { 
        for (int j=0; j<2; j++) { 
            bboxmin[j] = std::max(0.f, std::min<float>(bboxmin[j], pts[i][j])); 
            bboxmax[j] = std::min(clamp[j], std::max<float>(bboxmax[j], pts[i][j])); 
        } 
    } 

    Vec2i P; 
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) { 
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) { 
            Vec3f bc_screen  = barycentric(pts, P); 
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue; 
            framebuffer[P.x+P.y*width] = color;
        } 
    } 
}
void line(std::vector<Vec3f> &framebuffer,int x0, int y0, int x1, int y1,Vec3f color) {
	bool steep = false;
	if (std::abs(x0-x1)<std::abs(y0-y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	for (int x=x0; x<=x1; x++) {
        float t = (x-x0)/(float)(x1-x0);
        int y = y0*(1.-t) + y1*t;
        if (steep) {
            framebuffer[x+y*width] = color;
        } else {
            framebuffer[y+x*width] = color;
        }
    }

}