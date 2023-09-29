#pragma once

#include <glm/glm.hpp>

#include "sprite.hpp"

class Window;

class Player
{
public:
	Player(const Texture &texture,
	       const FloatRect &baseRect, unsigned baseCount,
	       const FloatRect &turretRect, unsigned turretCount,
	       glm::vec2 worldLocation);

	void update(Window &window, float dt);
	void draw(RenderTarget &target);

private:
	void handleInput(Window &window);
	static glm::vec2 handleKeyboardMovements(Window &window);
	static glm::vec2 handleKeyboardShots(Window &window);

private:
	Sprite mBaseSprite;
	Sprite mTurretSprite;
	glm::vec2 mBaseAngle;
	glm::vec2 mTurretAngle;
};
