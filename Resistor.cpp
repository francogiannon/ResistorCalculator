#include "Resistor.h"
#define PI 3.14159265358979323846
#define DELTA 33
#define BLACK 0,0,0

Resistor::Resistor(bool vertical, int x, int y) {
	value = 1000;
	this->horizontal = vertical;
	rPos.x = x;
	rPos.y = y;
	ptr2brother = -1;
	ptr2father = -1;
	ptr2son = -1;
	ptr2stepBro = -1;
}
void Resistor::updateResistor(ALLEGRO_BITMAP* resistorImage, vector<Resistor> &resistorArray) {
	if (horizontal) {
		al_draw_bitmap(resistorImage, rPos.x, rPos.y - al_get_bitmap_height(resistorImage) / 2, 0);
	}
	else {
		al_draw_rotated_bitmap(resistorImage, al_get_bitmap_width(resistorImage) / 2, al_get_bitmap_width(resistorImage) / 2, rPos.x - al_get_bitmap_height(resistorImage), rPos.y + al_get_bitmap_width(resistorImage) / 2, PI / 2, 0);
	}
	if (ptr2son != -1) {
		pos sonPos = resistorArray[ptr2son].getCoords();
		if (horizontal) { al_draw_line(rPos.x + al_get_bitmap_width(resistorImage), rPos.y, sonPos.x, sonPos.y, al_map_rgb(BLACK), 1); }
		else { al_draw_line(rPos.x, rPos.y + al_get_bitmap_width(resistorImage), sonPos.x, sonPos.y, al_map_rgb(BLACK), 1); }
	}
	if (ptr2brother != -1) {
		pos broPos = resistorArray[ptr2brother].getCoords();
		al_draw_line(rPos.x, rPos.y, broPos.x, broPos.y, al_map_rgb(BLACK), 1);
	}
	if (ptr2stepBro != -1) {
		pos broPos = resistorArray[ptr2stepBro].getCoords();
		bool broHorizontal = resistorArray[ptr2stepBro].getHoriz();
		if (horizontal) { 
			if (broHorizontal) { al_draw_line(rPos.x + al_get_bitmap_width(resistorImage), rPos.y, broPos.x + al_get_bitmap_width(resistorImage), broPos.y, al_map_rgb(BLACK), 1); }
			else { al_draw_line(rPos.x + al_get_bitmap_width(resistorImage), rPos.y, broPos.x, broPos.y + al_get_bitmap_width(resistorImage), al_map_rgb(BLACK), 1); }
		}
		else { 
			if (broHorizontal) { al_draw_line(rPos.x, rPos.y + al_get_bitmap_width(resistorImage), broPos.x + al_get_bitmap_width(resistorImage), broPos.y, al_map_rgb(BLACK), 1); }
			else { al_draw_line(rPos.x, rPos.y + al_get_bitmap_width(resistorImage), broPos.x , broPos.y + al_get_bitmap_width(resistorImage), al_map_rgb(BLACK), 1); }
		}
	}
}
void Resistor::moveResistor(int x, int y) {
	rPos.x = x;
	rPos.y = y;
}
UpperLowerEnum Resistor::mouseOverRes(ALLEGRO_BITMAP* resistorImage, pos mouse) {
	UpperLowerEnum ans = NOTOVER;
	if (!horizontal) {
		if ((mouse.x < rPos.x + al_get_bitmap_height(resistorImage) / 2) && (mouse.x > rPos.x - al_get_bitmap_height(resistorImage) / 2)) {
			if ((mouse.y > rPos.y) && (mouse.y < rPos.y + al_get_bitmap_width(resistorImage) / 2)) { //The upper Part
				ans = UPPERPART;
			}
			else if ((mouse.y >= rPos.y + al_get_bitmap_width(resistorImage) / 2) && (mouse.y < rPos.y + al_get_bitmap_width(resistorImage))) {
				ans = LOWERPART;
			}
		}
	}
	else {
		if ((mouse.y > rPos.y - al_get_bitmap_height(resistorImage) / 2) && (mouse.y < rPos.y + al_get_bitmap_height(resistorImage) / 2)) {
			if ((mouse.x > rPos.x) && (mouse.x < rPos.x + al_get_bitmap_width(resistorImage) / 2)) {
				ans = UPPERPART;
			}
			else if ((mouse.x >= rPos.x + al_get_bitmap_width(resistorImage) / 2) && (mouse.x < rPos.x + al_get_bitmap_width(resistorImage))) {
				ans = LOWERPART;
			}
		}
	}
	return ans;
}