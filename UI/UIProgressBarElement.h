/* Purpose: Support for Progress Bars within the UI system. Used to interpolate between two images between a scale of 0-1.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "UIElement.h"
#include "../Rendering/ProgressBar.h"
namespace Jauntlet {
class UIProgressBarElement : public UIElement, public ProgressBar {
public:
	UIProgressBarElement(const std::string& texture1, const glm::vec4& texture1UV, const std::string& texture2, const glm::vec4& texture2UV, const glm::vec4& dest, UIElement::ORIGIN_PIN pinType = UIElement::ORIGIN_PIN::TOP_LEFT);
	UIProgressBarElement(const std::string& texture, const glm::vec4& texture1UV, const glm::vec4& texture2UV, const glm::vec4& dest, UIElement::ORIGIN_PIN pinType = UIElement::ORIGIN_PIN::TOP_LEFT);

	void draw(Camera2D* camera, SpriteBatch* spriteBatch, float scale) override;
	void resolvePosition(Camera2D* camera, glm::vec2* resolvedPins, float scale) override;
protected:
	glm::vec2 _resolvedSize, _resolvedPosition;
};
}

