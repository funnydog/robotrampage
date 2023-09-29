#include <GLFW/glfw3.h>

#include "player.hpp"

#include "window.hpp"

namespace
{
static const float PlayerSpeed = 90.f;
}

Player::Player(
	const Texture &texture,
	const FloatRect &baseRect, unsigned baseCount,
	const FloatRect &turretRect, unsigned turretCount,
	glm::vec2 worldLocation)
	: mBaseSprite(texture, baseRect, worldLocation, glm::vec2(0.f))
	, mTurretSprite(texture, turretRect, worldLocation, glm::vec2(0.f))
	, mBaseAngle(0.f)
	, mTurretAngle(0.f)
{
	mBaseSprite.setBoundingPadding({4.f, 4.f});
	mBaseSprite.setAnimatedWhenStopped(false);
	FloatRect rect = baseRect;
	for (; baseCount > 1; baseCount--)
	{
		rect.pos.x += rect.size.x;
		mBaseSprite.addFrame(rect);
	}

	mTurretSprite.setAnimated(false);
	rect = turretRect;
	for (; turretCount > 1; turretCount--)
	{
		rect.pos.x += rect.size.x;
		mTurretSprite.addFrame(rect);
	}
}

void
Player::update(Window &window, float dt)
{
	handleInput(window);
	mBaseSprite.update(dt);
	mTurretSprite.setLocation(mBaseSprite.getLocation());
}

void
Player::draw(RenderTarget &target)
{
	mBaseSprite.draw(target);
	mTurretSprite.draw(target);
}

void
Player::handleInput(Window &window)
{
	glm::vec2 moveAngle(0.f);
	moveAngle += handleKeyboardMovements(window);
	// TODO:
	// moveAngle += handleGamePadMovements(window);

	glm::vec2 fireAngle(0.f);
	fireAngle += handleKeyboardShots(window);
	// TODO:
	// fireAngle += handleGamePadShots(window);

	if (moveAngle != glm::vec2(0.f))
	{
		mBaseAngle = glm::normalize(moveAngle);
		mBaseSprite.rotateTo(mBaseAngle);
	}

	if (fireAngle != glm::vec2(0.f))
	{
		mTurretAngle = glm::normalize(fireAngle);
		mTurretSprite.rotateTo(mTurretAngle);
	}

	mBaseSprite.setVelocity(moveAngle * PlayerSpeed);
}

glm::vec2
Player::handleKeyboardMovements(Window &window)
{
	const glm::vec2 movements[] = {
		{-1.f, -1.f}, {0.f, -1.f}, {1.f, -1.f},
		{-1.f,  0.f}, {0.f,  0.f}, {1.f,  0.f},
		{-1.f,  1.f}, {0.f,  1.f}, {1.f,  1.f},
	};

	int index = (window.isKeyPressed(GLFW_KEY_S) - window.isKeyPressed(GLFW_KEY_W) + 1) * 3
		+ (window.isKeyPressed(GLFW_KEY_D) - window.isKeyPressed(GLFW_KEY_A) + 1);

	return movements[index];
}

glm::vec2
Player::handleKeyboardShots(Window &window)
{
	if (window.isKeyPressed(GLFW_KEY_KP_1))
	{
		return {-1.f, 1.f};
	}
	if (window.isKeyPressed(GLFW_KEY_KP_2))
	{
		return {0.f, 1.f};
	}
	if (window.isKeyPressed(GLFW_KEY_KP_3))
	{
		return {1.f, 1.f};
	}
	if (window.isKeyPressed(GLFW_KEY_KP_4))
	{
		return {-1.f, 0.f};
	}
	if (window.isKeyPressed(GLFW_KEY_KP_6))
	{
		return {1.f, 0.f};
	}
	if (window.isKeyPressed(GLFW_KEY_KP_7))
	{
		return {-1.f, -1.f};
	}
	if (window.isKeyPressed(GLFW_KEY_KP_8))
	{
		return {0.f, -1.f};
	}
	if (window.isKeyPressed(GLFW_KEY_KP_9))
	{
		return {1.f, -1.f};
	}
	return {0.f, 0.f};
}
