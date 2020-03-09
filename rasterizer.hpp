#include <cmath>
#include <vector> 
#include "model.h"
#include "geometry.h"
const int width  = 800;
const int height = 800;

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);
void triangle(std::vector<Vec3f> &framebuffer,float *zbuffer ,Vec3f *pts, Vec3f color);
void line(std::vector<Vec3f> &framebuffer,int x0, int y0, int x1, int y1,Vec3f color);

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) { 
    
    Vec3f s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    Vec3f u = s[0] ^ s[1];
    if (std::abs(u[2])>2) 
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Vec3f(-1,1,1);
} 
 
void triangle(std::vector<Vec3f> &framebuffer,float *zbuffer ,Vec3f *pts, Vec3f color) {
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(width-1, height-1);
    for (int i=0; i<3; i++) { 
        for (int j=0; j<2; j++) { 
            bboxmin[j] = std::max(0.f, std::min<float>(bboxmin[j], pts[i][j])); 
            bboxmax[j] = std::min(clamp[j], std::max<float>(bboxmax[j], pts[i][j])); 
        } 
    } 

    Vec3f P; 
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) { 
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) { 
            Vec3f bc_screen  = barycentric(pts[0],pts[1],pts[2], P); 
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
            P.z = 0;
            for(int i=0;i<3;i++) P.z += pts[i][2]*bc_screen[i];
            if(zbuffer[int(P.x+P.y*width)]<P.z){
                zbuffer[int(P.x+P.y*width)] = P.z;
                framebuffer[P.x+P.y*width] = color;
            }
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