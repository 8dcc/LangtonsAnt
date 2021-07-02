#include <iostream>
#include <vector>
#include <algorithm>

#include "bitmap_image.hpp"

using namespace std;

// Define the color list
enum color { WHITE, BLACK, RED, BAK };
// Define the direction list
enum direction { UP, RIGHT, DOWN, LEFT };

int steps;
int x, y, x_s, y_s, x_l, y_l;
int dir;

struct c {
	int x, y;
	int color;

	c(int _x, int _y, int _c) { x = _x; y = _y; color = _c; }
};

vector<c> t;

int e(int, int);

void p();
void m();

void turn_r();
void turn_l();

int main() {
    x = 0, y = 0;
    dir = UP;
    x_s = 0, y_s = 0;
    x_l = 0, y_l = 0;

    cout << "Enter number of steps: ";
    cin >> steps;
    cout << endl;

	for(int i = 0; i < steps; i++) {
		int colour = e(x, y);

		if(colour == WHITE) {
			turn_r();
		} else if(colour == RED) {
			turn_l();
		} else if(colour == BLACK){
			turn_l();
		}
	}

	p();

    return 0;
}

// Function for changing the x and y coordinates
void m() {
	switch(dir) {
		case UP: 	y--; break;
		case DOWN: 	y++; break;
		case LEFT: 	x++; break;
		case RIGHT:	x--; break;
	}
}

// If the dir is UP, change it to RIGHT, ... (Turn right)
void turn_r() {
	switch(dir) {
		case UP: 	dir = RIGHT; break;
		case DOWN: 	dir = LEFT; break;
		case LEFT: 	dir = UP; break;
		case RIGHT:	dir = DOWN; break;
	}

	m();
}

// Same for left
void turn_l() {
	switch(dir) {
		case UP: 	dir = LEFT; break;
		case DOWN: 	dir = RIGHT; break;
		case LEFT: 	dir = DOWN; break;
		case RIGHT:	dir = UP; break;
	}

	m();
}

int e(int i, int j) {
	for(auto &k : t) {
		if(k.x == i && k.y == j) {
			int colour = k.color;

			// Color change rules
			if(colour == WHITE)
				k.color = BLACK;
			else if(colour == BLACK)
				k.color = RED;
			else if(colour == RED)
				k.color = WHITE;

			return colour;
		}
	}

	t.push_back(c(i, j, BLACK));

	return WHITE;
}

void p() {
	for(auto x : t) {
		int cx = x.x, cy = x.y;

		if(cx < x_s)
			x_s = cx;

		if(cy < y_s)
			y_s = cy;

		if(cx > x_l)
			x_l = cx;

		if(cy > y_l)
			y_l = cy;
	}

	// Print the dimensions
	cout << "xs: " << x_s << "\txl: " << x_l << endl;
	cout << "ys: " << y_s << "\tyl: " << y_l << endl;

	int off_x = x_l + abs(x_s) + 1;
	int off_y = y_l + abs(y_s) + 1;

	int **g = new int*[off_y];

	for(int i = 0; i < off_y; i++)
		g[i] = new int[off_x];

	for(int y = 0; y < off_y; y++) {
		for(int x = 0; x < off_x; x++) {
			g[y][x] = BAK;  // The color for the background
		}
	}

	for(auto x : t) {
		g[x.y + abs(y_s)][x.x + abs(x_s)] = x.color;
	}

	bitmap_image img(off_x, off_y);

	for(int y = 0; y < off_y; y++) {
		for(int x = 0; x < off_x; x++) {
			if(g[y][x] == BLACK) {  				// All the pixel colors in RGB
				img.set_pixel(x, y, 0, 0, 0);
			} else if(g[y][x] == RED) {
				img.set_pixel(x, y, 255, 0, 0);
			} else if(g[y][x] == BAK) {
				img.set_pixel(x, y, 230, 230, 230);
			} else {
				img.set_pixel(x, y, 255, 255, 255);
			}
		}
	}

	img.save_image("ant.bmp");  // Write into the bmp file
}
