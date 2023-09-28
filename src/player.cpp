#include "player.hpp"

Player::Player(
	const Texture &texture,
	const FloatRect &baseRect, unsigned baseCount,
	const FloatRect &turretRect, unsigned turretCount,
	glm::vec2 worldLocation)
	: mBaseSprite(texture, baseRect, worldLocation, glm::vec2(0.f))
	, mTurretSprite(texture, turretRect, worldLocation, glm::vec2(0.f))
{
	FloatRect rect = baseRect;
	for (; baseCount > 1; baseCount--)
	{
		rect.pos.x += rect.size.x;
		mBaseSprite.addFrame(rect);
	}

	rect = turretRect;
	for (; turretCount > 1; turretCount--)
	{
		rect.pos.x += rect.size.x;
		mTurretSprite.addFrame(rect);
	}

	mBaseSprite.setAnimated(false);
}

void
Player::update(float dt)
{
	mBaseSprite.update(dt);
	mTurretSprite.setLocation(mBaseSprite.getLocation());
}

void
Player::draw(RenderTarget &target)
{
	mBaseSprite.draw(target);
	mTurretSprite.draw(target);
}
