#pragma once

#include <Jauntlet/Rendering/SpriteBatch.h>
#include <Jauntlet/Rendering/SpriteFont.h>
#include <vector>

class Navigation {
public:
	Navigation();

	std::vector<std::vector<int>> genNav(); //new nav positions

	void drawNav(std::vector<std::vector<int>>& navPoints, Jauntlet::SpriteFont& font, Jauntlet::SpriteBatch& spriteBatch);

	void toggleNav();
private:
	bool _navOpen = false;
};