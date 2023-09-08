#include "UIElement.h"

namespace Jauntlet {
	UIElement::UIElement() {
	
	}
	UIElement::UIElement(glm::vec2 pos, Camera2D*  camera, bool achored) {
		_position = pos;
		_currentCamera = camera;
		_isAchored = achored;
	}
	void UIElement::anchor(Camera2D* camera) {

	}
	void UIElement::update() {
		_scale(_currentCamera);
	}
	void UIElement::setPos(glm::vec2 h) {
		_position = h;
		update();
	}
	void UIElement::_scale(Camera2D* camera) {
		//_position = camera->convertScreenToWorld(_position);
		//TODO 
	}
	
}

