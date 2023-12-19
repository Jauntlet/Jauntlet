#include "ProgressBar.h"

using namespace Jauntlet;
ProgressBar::ProgressBar(const std::string& texture1, const glm::vec4& texture1UV, const std::string& texture2, const glm::vec4& texture2UV, const glm::vec4& dest) :
	_emptyTexture(ResourceManager::getTexture(texture1).id),
	_fullTexture(ResourceManager::getTexture(texture2).id),
	_UV1(texture1UV),
	_UV2(texture2UV),
	_position(glm::vec2(dest.x, dest.y)),
	_size(glm::vec2(dest.z, dest.w))
{
	// Empty
}
ProgressBar::ProgressBar(const std::string& texture, const glm::vec4& texture1UV, const glm::vec4& texture2UV, const glm::vec4& dest) : 
	_emptyTexture(ResourceManager::getTexture(texture).id),
	_fullTexture(ResourceManager::getTexture(texture).id),
	_UV1(texture1UV),
	_UV2(texture2UV),
	_position(glm::vec2(dest.x, dest.y)),
	_size(glm::vec2(dest.z, dest.w))
{
	// Empty
}

void ProgressBar::setProgress(const float newProgress) {
	_progress = newProgress;
}
void ProgressBar::setPosition(const glm::vec2& newPosition) {
	_position = newPosition;
}

float ProgressBar::getProgress() const {
	return _progress;
}
glm::vec2 ProgressBar::getPosition() const {
	return _position;
}

void ProgressBar::draw(SpriteBatch& batch) {
	if (_progress != 0) {
		batch.draw(glm::vec4(_position, _size.x * _progress, _size.y), glm::vec4(_UV1.x, _UV1.y, _UV1.z * _progress, _UV1.w), _fullTexture);
	}
	if (_progress != 1) {
		batch.draw(glm::vec4(_position.x + _size.x * _progress, _position.y, _size.x * (1 - _progress), _size.y), glm::vec4(_UV2.x + _UV2.z * _progress, _UV2.y, _UV2.z * (1 - _progress), _UV1.w), _emptyTexture);
	}
}