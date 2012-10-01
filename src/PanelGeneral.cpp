#include "PanelGeneral.hpp"

#include "Rectangle.hpp"
#include "InputManager.hpp"

using namespace lalge;

PanelGeneral::PanelGeneral() {
	sprite = new Sprite("img/levelmaker/panel_general.png");
	setupRectangle();
	
	// borders
	
	border_top = new Button(new Sprite("img/levelmaker/icon_border_button.png"));
	border_top->play_sounds = false;
	border_top_position = r2vec(sprite->rectW()/2, 215);
	border_top->connect(Button::CLICKED, this, &PanelGeneral::handleBorderTop);
	
	border_right = new Button(new Sprite("img/levelmaker/icon_border_button.png"));
	border_right->play_sounds = false;
	border_right_position = r2vec(sprite->rectW() - border_right->sprite->rectW()/2 - 39, 215 + 77);
	border_right->connect(Button::CLICKED, this, &PanelGeneral::handleBorderRight);
	
	border_bottom = new Button(new Sprite("img/levelmaker/icon_border_button.png"));
	border_bottom->play_sounds = false;
	border_bottom_position = r2vec(sprite->rectW()/2, 215 + 77 + 77);
	border_bottom->connect(Button::CLICKED, this, &PanelGeneral::handleBorderBottom);
	
	border_left = new Button(new Sprite("img/levelmaker/icon_border_button.png"));
	border_left->play_sounds = false;
	border_left_position = r2vec(border_left->sprite->rectW()/2 + 39, 215 + 77);
	border_left->connect(Button::CLICKED, this, &PanelGeneral::handleBorderLeft);
	
	updateBordersPosition();
}

PanelGeneral::~PanelGeneral() {
	delete sprite;
	
	// borders
	
	delete border_top->sprite;
	delete border_top;
	
	delete border_right->sprite;
	delete border_right;
	
	delete border_bottom->sprite;
	delete border_bottom;
	
	delete border_left->sprite;
	delete border_left;
}

void PanelGeneral::show() {
	updateBordersPosition();
	
	
}

void PanelGeneral::hide() {
	
}

void PanelGeneral::update() {
	updateBordersPosition();
}

void PanelGeneral::render() {
	
}

void PanelGeneral::updateBordersPosition() {
	R2Vector origin = getOrigin();
	
	border_top->getShape()->position = origin + border_top_position;
	border_right->getShape()->position = origin + border_right_position;
	border_bottom->getShape()->position = origin + border_bottom_position;
	border_left->getShape()->position = origin + border_left_position;
}

void PanelGeneral::handleBorderTop(const observer::Event& event, bool& stop) {
	data->has_top = (!data->has_top);
}

void PanelGeneral::handleBorderRight(const observer::Event& event, bool& stop) {
	data->has_right = (!data->has_right);
}

void PanelGeneral::handleBorderBottom(const observer::Event& event, bool& stop) {
	data->has_bottom = (!data->has_bottom);
}

void PanelGeneral::handleBorderLeft(const observer::Event& event, bool& stop) {
	data->has_left = (!data->has_left);
}
