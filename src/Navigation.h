#pragma once

#include <vector>
#include <Jauntlet/Rendering/SpriteFont.h>
#include <Jauntlet/Rendering/SpriteBatch.h>

class Navigation {
public:
	Navigation();

	std::vector<std::vector<int>> genNav(); //new nav positions

	void drawNav(std::vector<std::vector<int>>& navPoints, Jauntlet::SpriteFont& font, Jauntlet::SpriteBatch& spriteBatch);

	void toggleNav();
private:
	bool _navOpen = false;
};