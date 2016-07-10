//#include <math.h>
#include "programCore.h"
#define PI 3.14159265358979323846

ProgramCore::ProgramCore(GUIElements* gui) {
	if (!initializeGUIElements(gui)) {
		printf("Error initializing guiElements\n");
	}
	resEnd = NOTOVER;
	resStart = NOTOVER;
	selectedResIndex = -1;
	vcc = Node(gui->vccImage);
	gnd = Node(gui->gndImage);
	backupPos.x = 0;
	backupPos.y = 0;
}

BOOL ProgramCore::eventHandler(ALL* allegro, ProgramElements* elements, GUIElements* gui) {
	int stillPlaying = TRUE;
	ALLEGRO_EVENT ev;
	al_wait_for_event(allegro->events_queue, &ev);

	if (ev.type == ALLEGRO_EVENT_TIMER) { elements->redraw = TRUE; }
	else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		stillPlaying = FALSE;
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
		elements->mouse.x = ev.mouse.x;
		elements->mouse.y = ev.mouse.y;
		if (elements->modeEnum == MOVEMODE) {
			resistorArray[selectedResIndex].moveResistor(elements->mouse.x, elements->mouse.y);
		}
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) { 
		if (ev.mouse.button == 2) { //Right Click
			rightClick(elements, gui);
		}
		else { click(allegro, elements->mouse, gui, elements); } //Left Click
	}
	else if (ev.type == ALLEGRO_EVENT_KEY_UP) { //Keyboard
		switch (ev.keyboard.keycode) {
		case ALLEGRO_KEY_SPACE:
			if (gui->resistorAngle == 0) { gui->resistorAngle = PI / 2; }
			else { gui->resistorAngle = 0; }
		}
	}
	//if redraw then update screen
	if ( elements->redraw == TRUE ) {
		updateScreen(allegro, gui, elements->mouse, elements->modeEnum);
		elements->redraw = FALSE;
	}
	return stillPlaying;
}
//Initializers
BOOL ProgramCore::initializeGUIElements(GUIElements* gui) {
	if (initializeMenu(gui)) {
		if (gui->resistorButton.buttonImage = al_load_bitmap("Resources/Buttons/resistorButtonImage1.png")) {
			if (gui->resistorButton.mouseOverButtonImage = al_load_bitmap("Resources/Buttons/resistorButtonImage2.png")) {
				gui->resistorButton.buttonPos.x = SCREEN_X - 3 * al_get_bitmap_width(gui->resistorButton.buttonImage) / 2;
				gui->resistorButton.buttonPos.y = 0 * al_get_bitmap_height(gui->resistorButton.buttonImage);
				if (gui->wireButton.buttonImage = al_load_bitmap("Resources/Buttons/wireButtonImage1.png")) {
					if (gui->wireButton.mouseOverButtonImage = al_load_bitmap("Resources/Buttons/wireButtonImage2.png")) {
						gui->wireButton.buttonPos.x = SCREEN_X - 3 * al_get_bitmap_width(gui->resistorButton.buttonImage) / 2;
						gui->wireButton.buttonPos.y = 1 * al_get_bitmap_height(gui->resistorButton.buttonImage);
						if (gui->gndButton.buttonImage = al_load_bitmap("Resources/Buttons/gndButtonImage1.png")) {
							if (gui->gndButton.mouseOverButtonImage = al_load_bitmap("Resources/Buttons/gndButtonImage2.png")) {
								gui->gndButton.buttonPos.x = SCREEN_X - 3 * al_get_bitmap_width(gui->resistorButton.buttonImage) / 2;
								gui->gndButton.buttonPos.y = 2 * al_get_bitmap_height(gui->resistorButton.buttonImage);
								if (gui->vccButton.buttonImage = al_load_bitmap("Resources/Buttons/vccButtonImage1.png")) {
									if (gui->vccButton.mouseOverButtonImage = al_load_bitmap("Resources/Buttons/vccButtonImage2.png")) {
										gui->vccButton.buttonPos.x = SCREEN_X - 3 * al_get_bitmap_width(gui->resistorButton.buttonImage) / 2;
										gui->vccButton.buttonPos.y = 3 * al_get_bitmap_height(gui->resistorButton.buttonImage);
										if (gui->resistorImage = al_load_bitmap("Resources/resistor.png")) {
											if (gui->gndImage = al_load_bitmap("Resources/Ground.png")) {
												if (gui->vccImage = al_load_bitmap("Resources/connector.png")) {
													if (gui->runCalcul.buttonImage = al_load_bitmap("Resources/Buttons/runButtonImage1.png")) {
														if (gui->runCalcul.mouseOverButtonImage = al_load_bitmap("Resources/Buttons/runButtonImage2.png")) {
															gui->runCalcul.buttonPos.x = SCREEN_X - al_get_bitmap_width(gui->runCalcul.buttonImage) - al_get_bitmap_width(gui->resistorButton.buttonImage) / 2;
															gui->runCalcul.buttonPos.y = SCREEN_Y - 2 * al_get_bitmap_height(gui->runCalcul.buttonImage);
															gui->resistorAngle = 0;
															return TRUE;
														}
													}
												}
												else { printf("Unable to load connector Image\n"); }
											}
											else { printf("Unable to load gnd image\n"); }
										}
										else { printf("Unable to load resistor Image\n"); }
									}
									else { printf("Unable to load connect Image 2\n"); }
								}
								else { printf("Unable to load connector Image 1\n"); }
							}
							else { printf("unable to load gnd image 2\n"); }
						}
						else { printf("unable to load gnd image 1\n"); }
					}
					else { printf("Unable to load wire button Image 2\n"); }
				}
				else { printf("Unable to load wire button Image 1\n"); }
			}
			else { printf("Unable to load resistor button image 2\n"); }
		}
		else { printf("Unable to load resistor button image 1\n"); }
	}
	else { printf("Failed to initialize the Menu\n"); }

	return FALSE;
}
BOOL ProgramCore::initializeMenu(GUIElements* gui) {
	int i = 0;
	if (gui->menuButtons[0].buttonImage = al_load_bitmap("Resources/Desplegable/DeleteMenu.png")) {
		if (gui->menuButtons[0].mouseOverButtonImage = al_load_bitmap("Resources/Desplegable/mouseOverDeleteMenu.png")) {
			if (gui->menuButtons[1].buttonImage = al_load_bitmap("Resources/Desplegable/MoveMenu.png")) {
				if (gui->menuButtons[1].mouseOverButtonImage = al_load_bitmap("Resources/Desplegable/mouseOverMoveMenu.png")) {
					if (gui->menuButtons[2].buttonImage = al_load_bitmap("Resources/Desplegable/RotateMenu.png")) {
						if (gui->menuButtons[2].mouseOverButtonImage = al_load_bitmap("Resources/Desplegable/mouseOverRotateMenu.png")) {
							if (gui->menuButtons[3].buttonImage = al_load_bitmap("Resources/Desplegable/SetValueMenu.png")) {
								if (gui->menuButtons[3].mouseOverButtonImage = al_load_bitmap("Resources/Desplegable/mouseOverSetValueMenu.png")) {
									for (i = 0; i < MENUSIZE; i++) {
										gui->menuButtons[i].buttonPos.x = 0;
										gui->menuButtons[i].buttonPos.y = 0;
									}
									return TRUE;
								}
								else { printf("Failed to load mouseOverSetValueMenu.png\n"); }
							}
							else { printf("Failed to load SetValueMenu.png\n"); }
						}
						else { printf("Failed to load mouseOverRotateMenu.png\n"); }
					}
					else { printf("Failed to load RotateMenu.png\n"); }
				}
				else { printf("Failed to load mouseOverMoveMenu.png\n"); }
			}
			else { printf("Failed to load MoveMenu.png\n"); }
		}
		else { printf("Failed to load mouseOverDeleteMenu.png\n"); }
	}
	else { printf("Failed to load DeleteMenu.png\n"); }

	return FALSE;
}
//Update
void ProgramCore::updateScreen(ALL* allegro, GUIElements* gui, pos mouse, enum ModeEnum modeEnum) {
	int i = 0;
	al_set_target_backbuffer(allegro->display);
	al_clear_to_color(al_map_rgb(WHITE));
	//al_draw_bitmap(allegro->fondo, 0, 0, 0);
	updateResistors(gui, allegro->font);
	updateAllButtons(gui, mouse);
	updateModes(gui, mouse, modeEnum);
	al_draw_text(allegro->font, al_map_rgb(BLACK), 20, allegro->screenHeight - 20, ALLEGRO_ALIGN_LEFT, "Resistor Calculator Beta Version 1.0");
	al_flip_display();
}
void ProgramCore::updateAllButtons(GUIElements* gui, pos mouse) {
	updateButton(gui->resistorButton, mouse);
	updateButton(gui->wireButton, mouse);
	updateButton(gui->gndButton, mouse);
	updateButton(gui->vccButton, mouse);
	if (updateCalculButton()) { updateButton(gui->runCalcul, mouse); }
}
void ProgramCore::updateButton(Button button, pos mouse) {
	if (checkButton(button, mouse)) {
		al_draw_bitmap(button.mouseOverButtonImage, button.buttonPos.x, button.buttonPos.y, 0);
	}
	else {
		al_draw_bitmap(button.buttonImage, button.buttonPos.x, button.buttonPos.y, 0);
	}
}
void ProgramCore::updateModes(GUIElements* gui, pos mouse, enum ModeEnum modeEnum) {
	int i = 0;
	if (modeEnum == RESISTORPLACE) { //Draw a resistor that follows the mouse
		if (gui->resistorAngle == 0) {
			al_draw_bitmap(gui->resistorImage, mouse.x, mouse.y - al_get_bitmap_height(gui->resistorImage) / 2, 0);
		}
		else {
			al_draw_rotated_bitmap(gui->resistorImage, al_get_bitmap_width(gui->resistorImage) / 2, al_get_bitmap_width(gui->resistorImage) / 2, mouse.x - al_get_bitmap_height(gui->resistorImage), mouse.y + al_get_bitmap_width(gui->resistorImage) / 2, gui->resistorAngle, 0);
		}
	}
	else if (modeEnum == GNDPLACE) {
		al_draw_bitmap(gui->gndImage, mouse.x - al_get_bitmap_width(gui->gndImage) / 2, mouse.y, 0);
	}
	else if (modeEnum == VCCPLACE) {
		al_draw_bitmap(gui->vccImage, mouse.x - al_get_bitmap_width(gui->vccImage) / 2, mouse.y - al_get_bitmap_height(gui->vccImage), 0);
	}
	else if (modeEnum == RIGHTCLICK) { //If I made right click I have to update the menu over the mouse
		for (i = 0; i < MENUSIZE; i++) {
			updateButton(gui->menuButtons[i], mouse);
		}
	}
	else if (modeEnum == WIREPLACE) {
		if (selectedResIndex != -1) {
			pos resTime = resistorArray[selectedResIndex].getCoords();
			if (resStart == UPPERPART) { al_draw_line(mouse.x, mouse.y, resTime.x, resTime.y, al_map_rgb(BLACK), LINEWIDTH); }
			else if (resStart == LOWERPART) {
				if (resistorArray[selectedResIndex].getHoriz()) { al_draw_line(mouse.x, mouse.y, resTime.x + al_get_bitmap_width(gui->resistorImage), resTime.y, al_map_rgb(BLACK), LINEWIDTH); }
				else { al_draw_line(mouse.x, mouse.y, resTime.x, resTime.y + al_get_bitmap_width(gui->resistorImage), al_map_rgb(BLACK), LINEWIDTH); }
			}
		}
		else if (resStart == NODE) {
			al_draw_line(vcc.getCorrds().x + al_get_bitmap_width(gui->vccImage) / 2, vcc.getCorrds().y + al_get_bitmap_height(gui->vccImage), mouse.x, mouse.y, al_map_rgb(BLACK), LINEWIDTH);
		}
	}
}
void ProgramCore::updateResistors(GUIElements* gui, ALLEGRO_FONT *font) {
	if (!resistorArray.empty()) {
		for (int i = 0; i < resistorArray.size(); i++) {
			resistorArray[i].updateResistor(gui->resistorImage, resistorArray, font);
		}
	}
	//UpdateNodes
	vcc.updateNode();
	gnd.updateNode();
	if (vcc.getIndex() != -1) {
		pos firstRes = resistorArray[vcc.getIndex()].getCoords();
		al_draw_line(vcc.getCorrds().x + al_get_bitmap_width(gui->vccImage) / 2, vcc.getCorrds().y + al_get_bitmap_height(gui->vccImage), firstRes.x, firstRes.y, al_map_rgb(BLACK), LINEWIDTH);
	}
	if (gnd.getIndex() != -1) {
		pos lastRes = resistorArray[gnd.getIndex()].getCoords();
		if (resistorArray[gnd.getIndex()].getHoriz()) {
			al_draw_line(gnd.getCorrds().x + al_get_bitmap_width(gui->gndImage) / 2, gnd.getCorrds().y, lastRes.x + al_get_bitmap_width(gui->resistorImage), lastRes.y, al_map_rgb(BLACK), LINEWIDTH);
		}
		else {
			al_draw_line(gnd.getCorrds().x + al_get_bitmap_width(gui->gndImage) / 2, gnd.getCorrds().y, lastRes.x, lastRes.y + al_get_bitmap_width(gui->resistorImage), al_map_rgb(BLACK), LINEWIDTH);
		}
	}
}
//Mouse Functions
BOOL ProgramCore::checkButton(Button button, pos mouse) {
	int mouseOverButton = FALSE;
	if ((mouse.x > button.buttonPos.x) && (mouse.x < button.buttonPos.x + al_get_bitmap_width(button.buttonImage))) {
		if ((mouse.y > button.buttonPos.y) && (mouse.y < button.buttonPos.y + al_get_bitmap_height(button.buttonImage))) {
			mouseOverButton = TRUE;
		}
		else { mouseOverButton = FALSE; }
	}
	else { mouseOverButton = FALSE; }
	return mouseOverButton;
}
BOOL ProgramCore::click(ALL* allegro, pos mouse, GUIElements* gui, ProgramElements* elements) {
	if (elements->modeEnum == NORMAL) {
		if (checkButton(gui->resistorButton, mouse)) { elements->modeEnum = RESISTORPLACE;	}
		else if (checkButton(gui->wireButton, mouse)) { elements->modeEnum = WIREPLACE; }
		else if (checkButton(gui->gndButton, mouse)) { elements->modeEnum = GNDPLACE; }
		else if (checkButton(gui->vccButton, mouse)) { elements->modeEnum = VCCPLACE; }
		if (updateCalculButton() && checkButton(gui->runCalcul, mouse)) { startCalculation(); }
	}
	else if (elements->modeEnum == RESISTORPLACE) {
		Resistor res = Resistor(false, mouse.x, mouse.y);
		if (gui->resistorAngle == 0) { res.rotate(); }
		resistorArray.push_back(res);
		//elements->modeEnum = NORMAL;
	}
	else if (elements->modeEnum == WIREPLACE) {
		wired(mouse, gui, elements);
	}
	else if (elements->modeEnum == VCCPLACE) { 
		vcc.setCoords(mouse.x - al_get_bitmap_width(gui->vccImage) / 2, mouse.y - al_get_bitmap_height(gui->vccImage));
		vcc.setIndex(-1);
		elements->modeEnum = NORMAL;
	}
	else if (elements->modeEnum == GNDPLACE) { 
		gnd.setCoords(mouse.x - al_get_bitmap_width(gui->gndImage) / 2, mouse.y); 
		gnd.setIndex(-1);
		elements->modeEnum = NORMAL;
	}
	else if (elements->modeEnum == RIGHTCLICK) {
		int i = 0;
		for (i = 0; i < MENUSIZE; i++) {
			if (checkButton(gui->menuButtons[i], mouse)) {
				switch (i) {
				case 0:	//Delete
					deleteResistor(selectedResIndex);
					selectedResIndex = -1;
					elements->modeEnum = NORMAL;
					break;
				case 1:	//Move
					elements->modeEnum = MOVEMODE;
					backupPos = resistorArray[selectedResIndex].getCoords();
					break;
				case 2: //Rotate
					resistorArray[selectedResIndex].rotate();
					selectedResIndex = -1;
					elements->modeEnum = NORMAL;
					break;
				case 3:	//Set Value
					break;
				}
			}
		}
		//elements->modeEnum = NORMAL;
	}
	else if (elements->modeEnum == MOVEMODE) {
		resistorArray[selectedResIndex].moveResistor(mouse.x, mouse.y);
		elements->modeEnum = NORMAL;
		selectedResIndex = -1;
	}
	return true;
}
void ProgramCore::rightClick(ProgramElements* elements, GUIElements* gui) {
	if (elements->modeEnum == NORMAL) {
		for (int j = 0; j < resistorArray.size(); j++) {
			if (resistorArray[j].mouseOverRes(gui->resistorImage, elements->mouse) != NOTOVER) {
				selectedResIndex = j;
				int i = 0;
				for (i = 0; i < MENUSIZE; i++) {
					gui->menuButtons[i].buttonPos.x = elements->mouse.x;
					gui->menuButtons[i].buttonPos.y = elements->mouse.y + i * al_get_bitmap_height(gui->menuButtons[i].buttonImage);
				}
				elements->modeEnum = RIGHTCLICK;
			}
		}
	}
	else if (elements->modeEnum == RESISTORPLACE) {
		selectedResIndex = -1;
		resStart = NOTOVER;
		elements->modeEnum = NORMAL;
	}
	else if (elements->modeEnum == MOVEMODE) {
		resistorArray[selectedResIndex].moveResistor(backupPos.x, backupPos.y);
		backupPos.x = 0;
		backupPos.y = 0;
		selectedResIndex = -1;
		elements->modeEnum = NORMAL;
	}
	else { 
		selectedResIndex = -1;
		resStart = NOTOVER;
		elements->modeEnum = NORMAL; 
	}
} 

void ProgramCore::wired(pos mouse, GUIElements* gui, ProgramElements* elements) {
	for (int i = 0; i < resistorArray.size(); i++) {
		UpperLowerEnum upperLowerEnum = resistorArray[i].mouseOverRes(gui->resistorImage, mouse);
		if (upperLowerEnum != NOTOVER) {
			if (resStart == NOTOVER) {
				resStart = upperLowerEnum;
				selectedResIndex = i;
			}
			else {
				//resEnd = upperLowerEnum; //TODO: ESTO NO TIENE SENTIDO, FIJARSE SI SIRVE RESEND.
				if ((resStart != LOWERPART) || (upperLowerEnum != LOWERPART)) { //Unless closing brotherhood
					if (resistorArray[i].getFather() != -1) { //It it ha s another son, leave it.
						int last = resistorArray[i].getFather();
						if (resistorArray[last].getSon() == i) { resistorArray[last].deleteSon(); }
						else if (resistorArray[last].getBrother() == i) { resistorArray[last].deleteBrother(); }
					}
					resistorArray[i].setFather(selectedResIndex); //If it's NODE it will be the same as delete father.
				}
				if (resStart == LOWERPART) { 
					if (upperLowerEnum == LOWERPART) {
						resistorArray[selectedResIndex].setStepBro(i);
					}
					else if (upperLowerEnum == UPPERPART) {
						resistorArray[selectedResIndex].setSon(i);
					}
				}
				else if (resStart == UPPERPART) { resistorArray[selectedResIndex].setBrother(i); }
				else if (resStart == NODE) { vcc.setIndex(i); }
				else { cout << "Error, resStart wasn't Lower nor Upper" << endl; } //Normally it should never get here.
				resStart = NOTOVER;
				//resEnd = NOTOVER;
				selectedResIndex = -1;
				//elements->modeEnum = NORMAL;
			}
		}
	}
	if (vcc.mouseOverNode(mouse)) {
		if (resStart == NOTOVER) { resStart = NODE; }
		else { elements->modeEnum = NORMAL; }
	}
	if (gnd.mouseOverNode(mouse)) {
		if (resStart != NOTOVER) { 
			gnd.setIndex(selectedResIndex);
			elements->modeEnum = NORMAL;
			resStart = NOTOVER;
			selectedResIndex = -1;
		}
		else { elements->modeEnum = NORMAL; }
	}
}
bool ProgramCore::updateCalculButton() {
	int index = gnd.getIndex();
	if (index != -1) {
		while (resistorArray[index].getFather() != -1) {
			index = resistorArray[index].getFather();
		}
		if (vcc.getIndex() == index) { return true; }
	}
	return false;
}
void ProgramCore::deleteResistor(int indexToDelete) {
	Resistor resistorToDelete = resistorArray[indexToDelete];
	if (resistorToDelete.getFather() != -1) { //Make sure you leave no broken heart
		if (resistorArray[resistorToDelete.getFather()].getSon() == indexToDelete) {
			resistorArray[resistorToDelete.getFather()].deleteSon();
		}
		else if (resistorArray[resistorToDelete.getFather()].getBrother() == indexToDelete) {
			resistorArray[resistorToDelete.getFather()].deleteBrother();
		}
		else {
			cout << "Error, unrecognized child or brother" << endl; //It should never get here
		}
	}
	if (resistorToDelete.getSon() != -1) { //Take care of your son
		resistorArray[resistorToDelete.getSon()].deleteFather();
	}
	if (resistorToDelete.getBrother() != -1) { //Take care of your brother
		resistorArray[resistorToDelete.getBrother()].deleteFather();
	}
	for (int i = 0; i < resistorArray.size(); i++) {
		if (resistorArray[i].getStepBro() == indexToDelete) { resistorArray[i].deleteStepBro(); }
	}
	//resistorArray[indexToDelete].deleteResistor();
	reorder(indexToDelete);
	resistorArray.erase(resistorArray.begin() + indexToDelete);
}
void ProgramCore::reorder(int indexToDelete) {
	if (gnd.getIndex() > indexToDelete) { gnd.decreseIndex(); }
	else if (gnd.getIndex() == indexToDelete) { gnd.setIndex(-1); }
	if (vcc.getIndex() > indexToDelete) { vcc.decreseIndex(); }
	else if (vcc.getIndex() == indexToDelete) { vcc.setIndex(-1); }
	for (int i = 0; i < resistorArray.size(); i++) {
		if (resistorArray[i].getBrother() > indexToDelete) { resistorArray[i].decreseBrother(); }
		if (resistorArray[i].getFather() > indexToDelete) { resistorArray[i].decreseFather(); }
		if (resistorArray[i].getSon() > indexToDelete) { resistorArray[i].decreseSon(); }
		if (resistorArray[i].getStepBro() > indexToDelete) { resistorArray[i].decreseStepBro(); }
	}
}
bool ProgramCore::startCalculation() {
	double ans = calculate(vcc.getIndex()); //If I got here I already checked vcc points the correct one and that I have a limit.
	return true;
}
double ProgramCore::calculate(int index) {
	if (resistorArray[index].getBrother() != -1) { //Have Brother
		if (resistorArray[index].getBrother() == resistorArray[index].getStepBro()) {
			double broCalculate = calculate(resistorArray[index].getBrother());
			resistorArray[index].setValue( (resistorArray[index].getValue() * broCalculate) / ( resistorArray[index].getValue() + broCalculate) );
			//if(resistorArray[index].getBrother())
			deleteResistor(resistorArray[index].getBrother());
		}
	}
	if (resistorArray[index].getSon() != -1) { //Have Son but no brother
		return resistorArray[index].getValue() + calculate(resistorArray[index].getSon());
	}
	else {
		return resistorArray[index].getValue();
	}
}