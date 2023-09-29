#include "player.hpp"

Player::Player(
	const Texture &texture,
	const FloatRect &baseRect, unsigned baseCount,
	const FloatRect &turretRect, unsigned turretCount,
	glm::vec2 worldLocation)
	: mBaseSprite(texture, baseRect, worldLocation, glm::vec2(0.f))
	, mTurretSprite(texture, turretRect, worldLocation, glm::vec2(0.f))
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
