#include "ProgressBar.h"

ProgressBar::ProgressBar(const std::string& texture1, const glm::vec4& texture1UV, const std::string& texture2, const glm::vec4& texture2UV, const glm::vec4& dest) :
	_emptyTexture(Jauntlet::ResourceManager::getTexture(texture1).id),
	_fullTexture(Jauntlet::ResourceManager::getTexture(texture2).id),
	_UV1(texture1UV),
	_UV2(texture2UV),
	position(glm::vec2(dest.x, dest.y)),
	_size(glm::vec2(dest.z, dest.w))
{
	// Empty
}
ProgressBar::ProgressBar(const std::string& texture, const glm::vec4& texture1UV, const glm::vec4& texture2UV, const glm::vec4& dest) : 
	_emptyTexture(Jauntlet::ResourceManager::getTexture(texture).id),
	_fullTexture(Jauntlet::ResourceManager::getTexture(texture).id),
	_UV1(texture1UV),
	_UV2(texture2UV),
	position(glm::vec2(dest.x, dest.y)),
	_size(glm::vec2(dest.z, dest.w))
{
	// Empty
}

void ProgressBar::draw(SpriteBatch& batch) {
	if (progress != 0) {
		batch.draw(glm::vec4(position, _size.x * progress, _size.y), glm::vec4(_UV1.x, _UV1.y, _UV1.z * progress, _UV1.w), _fullTexture);
	}
	if (progress != 1) {
		batch.draw(glm::vec4(position.x + _size.x * progress, position.y, _size.x * (1 - progress), _size.y), glm::vec4(_UV2.x + _UV2.z * progress, _UV2.y, _UV2.z * (1 - progress), _UV1.w), _emptyTexture);
	}
}