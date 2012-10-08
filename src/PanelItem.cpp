#include "common.hpp"

#include "PanelItem.hpp"

#include "Item.hpp"
#include "SDLBase.hpp"

using namespace common;
using namespace lalge;

using std::set;

PanelItem::PanelItem() {
	sprite = new Sprite("img/levelmaker/panel_item.png");
	setupRectangle();
	
	// inputs
	
	sprite_input_mass = new Sprite("img/levelmaker/input_mass.png");
	sprite_input_mass->clip(0, 0, sprite_input_mass->srcW(), sprite_input_mass->srcH()/2);
	button_input_mass = new Button(new Sprite("img/levelmaker/input_mass_button.png"));
	button_input_mass->connect(Button::CLICKED, this, &PanelItem::handleInputMassButton);
	input_mass_position = r2vec(105 + button_input_mass->sprite->rectW()/2, 156 - 29);
	invalid_input_mass = false;
	input_mass.setMaxSize(21);
	input_mass.connect(InputString::UPDATE, this, &PanelItem::handleInputMass);
	text_input_mass = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_mass->setText(input_mass.get());
	
	sprite_input_elasticity = new Sprite("img/levelmaker/input_elasticity.png");
	sprite_input_elasticity->clip(0, 0, sprite_input_elasticity->srcW(), sprite_input_elasticity->srcH()/2);
	button_input_elasticity = new Button(new Sprite("img/levelmaker/input_elasticity_button.png"));
	button_input_elasticity->connect(Button::CLICKED, this, &PanelItem::handleInputElasticityButton);
	input_elasticity_position = r2vec(159 + button_input_elasticity->sprite->rectW()/2, 156);
	invalid_input_elasticity = false;
	input_elasticity.setMaxSize(15);
	input_elasticity.connect(InputString::UPDATE, this, &PanelItem::handleInputElasticity);
	text_input_elasticity = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_elasticity->setText(input_elasticity.get());
	
	sprite_input_value = new Sprite("img/levelmaker/input_item_value.png");
	sprite_input_value->clip(0, 0, sprite_input_value->srcW(), sprite_input_value->srcH()/2);
	button_input_value = new Button(new Sprite("img/levelmaker/input_item_value_button.png"));
	button_input_value->connect(Button::CLICKED, this, &PanelItem::handleInputValueButton);
	input_value_position = r2vec(109 + button_input_value->sprite->rectW()/2, 156 + 29);
	invalid_input_value = false;
	input_value.setMaxSize(21);
	input_value.connect(InputString::UPDATE, this, &PanelItem::handleInputValue);
	text_input_value = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_value->setText(input_value.get());
	
	// operations
	button_time = new Button(new Sprite("img/levelmaker/icon_operation_button.png"));
	button_time->play_sounds = false;
	button_time_position = r2vec(sprite->rectW()/2 - 120, 73);
	button_time->connect(Button::CLICKED, this, &PanelItem::handleTime);
	
	button_point = new Button(new Sprite("img/levelmaker/icon_operation_button.png"));
	button_point->play_sounds = false;
	button_point_position = r2vec(sprite->rectW()/2 - 60, 73);
	button_point->connect(Button::CLICKED, this, &PanelItem::handlePoint);
	
	button_life = new Button(new Sprite("img/levelmaker/icon_operation_button.png"));
	button_life->play_sounds = false;
	button_life_position = r2vec(sprite->rectW()/2, 73);
	button_life->connect(Button::CLICKED, this, &PanelItem::handleLife);
	
	button_mass = new Button(new Sprite("img/levelmaker/icon_operation_button.png"));
	button_mass->play_sounds = false;
	button_mass_position = r2vec(sprite->rectW()/2 + 60, 73);
	button_mass->connect(Button::CLICKED, this, &PanelItem::handleMass);
	
	button_barrier = new Button(new Sprite("img/levelmaker/icon_operation_button.png"));
	button_barrier->play_sounds = false;
	button_barrier_position = r2vec(sprite->rectW()/2 + 120, 73);
	button_barrier->connect(Button::CLICKED, this, &PanelItem::handleBarrier);
	
	updatePositions();
	hide();
}

PanelItem::~PanelItem() {
	delete sprite;
	
	// inputs
	
	delete sprite_input_mass;
	delete button_input_mass->sprite;
	delete button_input_mass;
	delete text_input_mass;
	
	delete sprite_input_elasticity;
	delete button_input_elasticity->sprite;
	delete button_input_elasticity;
	delete text_input_elasticity;
	
	delete sprite_input_value;
	delete button_input_value->sprite;
	delete button_input_value;
	delete text_input_value;
	
	// operations
	
	delete button_time->sprite;
	delete button_time;
	
	delete button_point->sprite;
	delete button_point;
	
	delete button_life->sprite;
	delete button_life;
	
	delete button_mass->sprite;
	delete button_mass;
	
	delete button_barrier->sprite;
	delete button_barrier;
}

void PanelItem::show() {
	// inputs
	button_input_mass->enable(true);
	button_input_elasticity->enable(true);
	button_input_value->enable(true);
	if (LevelMakerObject::selected.size() == 1) {
		input_mass.set(eval(((Particle*)(*LevelMakerObject::selected.begin())->getGameObject())->getMass()));
		input_elasticity.set(eval(((Particle*)(*LevelMakerObject::selected.begin())->getGameObject())->getElasticity()));
		input_value.set(eval(((Item*)(*LevelMakerObject::selected.begin())->getGameObject())->getValue()));
	}
	else {
		Scalar mass, elasticity, value;
		bool set_mass = true;
		bool set_elasticity = true;
		bool set_value = true;
		
		set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin();
		
		mass = ((Particle*)(*it)->getGameObject())->getMass();
		elasticity = ((Particle*)(*it)->getGameObject())->getElasticity();
		value = ((Item*)(*it)->getGameObject())->getValue();
		
		++it;
		
		for (it = it; it != LevelMakerObject::selected.end(); ++it) {
			if ((!set_mass) && (!set_elasticity) && (!set_value))
				break;
			
			if (((Particle*)(*it)->getGameObject())->getMass() != mass)
				set_mass = false;
			if (((Particle*)(*it)->getGameObject())->getElasticity() != elasticity)
				set_elasticity = false;
			if (((Item*)(*it)->getGameObject())->getValue() != value)
				set_value = false;
		}
		
		if (set_mass)
			input_mass.set(eval(mass));
		else {
			text_input_mass->setText("");
			input_mass.clear();
		}
		
		if (set_elasticity)
			input_elasticity.set(eval(elasticity));
		else {
			text_input_elasticity->setText("");
			input_elasticity.clear();
		}
		
		if (set_value)
			input_value.set(eval(value));
		else {
			text_input_value->setText("");
			input_value.clear();
		}
	}
	
	// operations
	button_time->enable(true);
	button_point->enable(true);
	button_life->enable(true);
	button_mass->enable(true);
	button_barrier->enable(true);
	
	updatePositions();
}

void PanelItem::hide() {
	// inputs
	button_input_mass->enable(false);
	input_mass.enabled = false;
	button_input_elasticity->enable(false);
	input_elasticity.enabled = false;
	button_input_value->enable(false);
	input_value.enabled = false;
	
	// operations
	button_time->enable(false);
	button_point->enable(false);
	button_life->enable(false);
	button_mass->enable(false);
	button_barrier->enable(false);
}

void PanelItem::update() {
	// inputs
	input_mass.update();
	input_elasticity.update();
	input_value.update();
	
	updatePositions();
}

void PanelItem::render() {
	if ((invalid_input_mass) || (input_mass.enabled))
		sprite_input_mass->render(button_input_mass->getShape()->position.x(0), button_input_mass->getShape()->position.x(1), true);
	text_input_mass->render(button_input_mass->getShape()->position.x(0), button_input_mass->getShape()->position.x(1));
	
	if ((invalid_input_elasticity) || (input_elasticity.enabled))
		sprite_input_elasticity->render(button_input_elasticity->getShape()->position.x(0), button_input_elasticity->getShape()->position.x(1), true);
	text_input_elasticity->render(button_input_elasticity->getShape()->position.x(0), button_input_elasticity->getShape()->position.x(1));
	
	if ((invalid_input_value) || (input_value.enabled))
		sprite_input_value->render(button_input_value->getShape()->position.x(0), button_input_value->getShape()->position.x(1), true);
	text_input_value->render(button_input_value->getShape()->position.x(0), button_input_value->getShape()->position.x(1));
}

void PanelItem::updatePositions() {
	R2Vector origin = getOrigin();
	
	// inputs
	button_input_mass->getShape()->position = origin + input_mass_position;
	button_input_elasticity->getShape()->position = origin + input_elasticity_position;
	button_input_value->getShape()->position = origin + input_value_position;
	
	// operations
	button_time->getShape()->position = origin + button_time_position;
	button_point->getShape()->position = origin + button_point_position;
	button_life->getShape()->position = origin + button_life_position;
	button_mass->getShape()->position = origin + button_mass_position;
	button_barrier->getShape()->position = origin + button_barrier_position;
}

void PanelItem::handleInputMass(const observer::Event& event, bool& stop) {
	Scalar mass;
	
	text_input_mass->setText(input_mass.get());
	
	// check invalid input
	if (eval(input_mass.get(), mass)) {
		if (mass <= 0) {
			invalid_input_mass = true;
			sprite_input_mass->clip(0, sprite_input_mass->srcH()/2, sprite_input_mass->srcW(), sprite_input_mass->srcH()/2);
			return;
		}
		// updating data
		for (set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin(); it != LevelMakerObject::selected.end(); ++it)
			((Particle*)(*it)->getGameObject())->setMass(mass);
		
		invalid_input_mass = false;
		sprite_input_mass->clip(0, 0, sprite_input_mass->srcW(), sprite_input_mass->srcH()/2);
	}
	else {
		invalid_input_mass = true;
		sprite_input_mass->clip(0, sprite_input_mass->srcH()/2, sprite_input_mass->srcW(), sprite_input_mass->srcH()/2);
	}
}

void PanelItem::handleInputMassButton(const observer::Event& event, bool& stop) {
	input_mass.enabled = true;
	input_elasticity.enabled = false;
	input_value.enabled = false;
}

void PanelItem::handleInputElasticity(const observer::Event& event, bool& stop) {
	Scalar elasticity;
	
	text_input_elasticity->setText(input_elasticity.get());
	
	// check invalid input
	if (eval(input_elasticity.get(), elasticity)) {
		if ((elasticity < 0) || (elasticity > 0.5)) {
			invalid_input_elasticity = true;
			sprite_input_elasticity->clip(0, sprite_input_elasticity->srcH()/2, sprite_input_elasticity->srcW(), sprite_input_elasticity->srcH()/2);
			return;
		}
		// updating data
		for (set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin(); it != LevelMakerObject::selected.end(); ++it)
			((Particle*)(*it)->getGameObject())->setElasticity(elasticity);
		
		invalid_input_elasticity = false;
		sprite_input_elasticity->clip(0, 0, sprite_input_elasticity->srcW(), sprite_input_elasticity->srcH()/2);
	}
	else {
		invalid_input_elasticity = true;
		sprite_input_elasticity->clip(0, sprite_input_elasticity->srcH()/2, sprite_input_elasticity->srcW(), sprite_input_elasticity->srcH()/2);
	}
}

void PanelItem::handleInputElasticityButton(const observer::Event& event, bool& stop) {
	input_mass.enabled = false;
	input_elasticity.enabled = true;
	input_value.enabled = false;
}

void PanelItem::handleInputValue(const observer::Event& event, bool& stop) {
	Scalar value;
	
	text_input_value->setText(input_value.get());
	
	// check invalid input
	if (eval(input_value.get(), value)) {
		if (value < 0) {
			invalid_input_value = true;
			sprite_input_value->clip(0, sprite_input_value->srcH()/2, sprite_input_value->srcW(), sprite_input_value->srcH()/2);
			return;
		}
		invalid_input_value = false;
		sprite_input_value->clip(0, 0, sprite_input_value->srcW(), sprite_input_value->srcH()/2);
	}
	else {
		invalid_input_value = true;
		sprite_input_value->clip(0, sprite_input_value->srcH()/2, sprite_input_value->srcW(), sprite_input_value->srcH()/2);
		return;
	}
	
	// updating data
	for (set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin(); it != LevelMakerObject::selected.end(); ++it) {
		((Item*)(*it)->getGameObject())->setValue(value);
		
		// sprite for barriers
		if (((Item*)(*it)->getGameObject())->operation == Item::BARRIER) {
			if (!value) {
				(*it)->getGameObject()->sprite = LevelMakerData::sprite_item_barrier;
				(*it)->selection = LevelMakerData::sprite_item_barrier_selection;
			}
			else {
				(*it)->getGameObject()->sprite = LevelMakerData::sprite_item_lethal_barrier;
				(*it)->selection = LevelMakerData::sprite_item_lethal_barrier_selection;
			}
		}
	}
}

void PanelItem::handleInputValueButton(const observer::Event& event, bool& stop) {
	input_mass.enabled = false;
	input_elasticity.enabled = false;
	input_value.enabled = true;
}

void PanelItem::handleTime(const observer::Event& event, bool& stop) {
	// updating data
	for (set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin(); it != LevelMakerObject::selected.end(); ++it) {
		((Item*)(*it)->getGameObject())->operation = Item::TIME;
		
		// sprite
		(*it)->getGameObject()->sprite = LevelMakerData::sprite_item_time;
		(*it)->selection = LevelMakerData::sprite_item_time_selection;
	}
}

void PanelItem::handlePoint(const observer::Event& event, bool& stop) {
	// updating data
	for (set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin(); it != LevelMakerObject::selected.end(); ++it) {
		((Item*)(*it)->getGameObject())->operation = Item::POINT;
		
		// sprite
		(*it)->getGameObject()->sprite = LevelMakerData::sprite_item_point;
		(*it)->selection = LevelMakerData::sprite_item_point_selection;
	}
}

void PanelItem::handleLife(const observer::Event& event, bool& stop) {
	// updating data
	for (set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin(); it != LevelMakerObject::selected.end(); ++it) {
		((Item*)(*it)->getGameObject())->operation = Item::LIFE;
		
		// sprite
		(*it)->getGameObject()->sprite = LevelMakerData::sprite_item_life;
		(*it)->selection = LevelMakerData::sprite_item_life_selection;
	}
}

void PanelItem::handleMass(const observer::Event& event, bool& stop) {
	// updating data
	for (set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin(); it != LevelMakerObject::selected.end(); ++it) {
		((Item*)(*it)->getGameObject())->operation = Item::MASS;
		
		// sprite
		(*it)->getGameObject()->sprite = LevelMakerData::sprite_item_mass;
		(*it)->selection = LevelMakerData::sprite_item_mass_selection;
	}
}

void PanelItem::handleBarrier(const observer::Event& event, bool& stop) {
	// updating data
	for (set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin(); it != LevelMakerObject::selected.end(); ++it) {
		((Item*)(*it)->getGameObject())->operation = Item::BARRIER;
		
		// sprite
		if (!((Item*)(*it)->getGameObject())->operation) {
			(*it)->getGameObject()->sprite = LevelMakerData::sprite_item_barrier;
			(*it)->selection = LevelMakerData::sprite_item_barrier_selection;
		}
		else {
			(*it)->getGameObject()->sprite = LevelMakerData::sprite_item_lethal_barrier;
			(*it)->selection = LevelMakerData::sprite_item_lethal_barrier_selection;
		}
	}
}
