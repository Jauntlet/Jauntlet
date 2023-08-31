#include"UiElement.h"
namespace Jauntlet {
	UiElement::UiElement()
	{
	
	}
	UiElement::UiElement(glm::vec2 pos, Camera2D*  camera, bool achored)
	{
		_position = pos;
		_currentCamera = camera;
		_isAchored = achored;
	}
	void UiElement::Anchor(Camera2D* camera) {

	}
	void UiElement::Update()
	{
		_Scale(_currentCamera);
	}
	void UiElement::SetPos(glm::vec2 h)
	{
		_position = h;
		Update();
	}
	void UiElement::_Scale(Camera2D* camera)
	{
	//_position = camera->convertScreenToWorld(_position);
	//TODO 
	}
	
}

