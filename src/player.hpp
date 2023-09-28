#pragma once

#include <glm/glm.hpp>

#include "sprite.hpp"

class Player
{
public:
	Player(const Texture &texture,
	       const FloatRect &baseRect, unsigned baseCount,
	       const FloatRect &turretRect, unsigned turretCount,
	       glm::vec2 worldLocation);

	void update(float dt);
	void draw(RenderTarget &target);

private:
	Sprite mBaseSprite;
	Sprite mTurretSprite;
};
