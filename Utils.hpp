#ifndef UTILS_H
#define UTILS_H
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
void triangle(Vec2f p1,Vec2f p2,Vec2f p3,TGAImage & image,TGAColor color);
void line(Vec2f p0, Vec2f p1 , TGAImage &image, TGAColor color) ;
//algo Bresenham
void line2(Vec2f p0, Vec2f p1 , TGAImage &image, TGAColor color);


void triangle(Vec2f p1,Vec2f p2,Vec2f p3,TGAImage & image,TGAColor color){
    if (p1.y>p2.y) std::swap(p1, p2); 
    if (p1.y>p3.y) std::swap(p1, p3); 
    if (p2.y>p3.y) std::swap(p2, p3); 
    line(p1,p2,image,color);
    line(p2,p3,image,color);
    line(p1,p3,image,color);
}
void line(Vec2f p0, Vec2f p1 , TGAImage &image, TGAColor color) {
    double a=(p0.y-p1.y)/(p0.x-p1.x);
    double b=p0.y-a*p0.x;
		for (int x=p0.x; x<=p1.x; x++) {
            float y=a*x+b;
			image.set(x, y, color);
	 } 
}
//algo Bresenham
void line2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
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
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
	
}


#endif