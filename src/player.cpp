#include <GLFW/glfw3.h>

#include "player.hpp"

#include "camera.hpp"
#include "rendertarget.hpp"
#include "window.hpp"

namespace
{
static const float PlayerSpeed = 200.f;
static const FloatRect ScrollArea({150.f, 100.f}, {500.f, 400.f});
}

Player::Player(
	Camera &camera,
	const Texture &texture,
	const FloatRect &baseRect, unsigned baseCount,
	const FloatRect &turretRect, unsigned turretCount,
	glm::vec2 worldLocation)
	: mCamera(camera)
	, mBaseSprite(texture, baseRect, worldLocation, glm::vec2(0.f))
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
	handleInput(dt, window);
	mBaseSprite.update(dt);
	clampToWorld();
	mTurretSprite.setLocation(mBaseSprite.getLocation());
}

void
Player::draw(RenderTarget &target)
{
	target.draw(mBaseSprite);
	target.draw(mTurretSprite);
}

void
Player::handleInput(float dt, Window &window)
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

	repositionCamera(dt, moveAngle);
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

void
Player::clampToWorld()
{
	auto rect = mCamera.getWorldRectangle();
	mBaseSprite.setLocation(
		glm::clamp(mBaseSprite.getLocation(),
			   rect.pos,
			   rect.pos + rect.size - mBaseSprite.getSize()));
}

void
Player::repositionCamera(float dt, glm::vec2 angle)
{
	angle *= PlayerSpeed * dt;

	auto baseScreenStart = mBaseSprite.getLocation() - mCamera.getPosition();
	auto baseScreenEnd = baseScreenStart + mBaseSprite.getSize();

	if ((baseScreenStart.x < ScrollArea.pos.x)
	    && angle.x < 0.f)
	{
		mCamera.move(glm::vec2(angle.x, 0.f));
	}

	if ((baseScreenEnd.x > ScrollArea.pos.x + ScrollArea.size.x)
	    && angle.x > 0.f)
	{
		mCamera.move(glm::vec2(angle.x, 0.f));
	}

	if ((baseScreenStart.y < ScrollArea.pos.y)
	    && angle.y < 0.f)
	{
		mCamera.move(glm::vec2(0.f, angle.y));
	}

	if ((baseScreenEnd.y > ScrollArea.pos.y + ScrollArea.size.y)
	    && angle.y > 0.f)
	{
		mCamera.move(glm::vec2(0.f, angle.y));
	}
}
