#include "sprite.hpp"

#include "rendertarget.hpp"

Sprite::Sprite(
	const Texture &texture,
	FloatRect initialFrame,
	glm::vec2 location,
	glm::vec2 velocity)
	: mTexture(texture)
	, mTextureSize(texture.getSize())
	, mFrameSize(initialFrame.size)
	, mTintColor(Color::White)
	, mFrameIndex(0)
	, mFrameElapsed(0.f)
	, mFrameDelay(0.1f)
	, mAnimated(true)
	, mLocation(location)
	, mVelocity(velocity)
	, mRotation(0.f)
	, mCollisionRadius(0.f)
	, mBoundingPadding(0.f)
{
	addFrame(initialFrame);
}

glm::vec2
Sprite::getLocation() const
{
	return mLocation;
}

void
Sprite::setLocation(glm::vec2 location)
{
	mLocation = location;
}

glm::vec2
Sprite::getVelocity() const
{
	return mVelocity;
}

void
Sprite::setVelocity(glm::vec2 velocity)
{
	mVelocity = velocity;
}

Color
Sprite::getTintColor() const
{
	return mTintColor;
}

void
Sprite::setTintColor(Color color)
{
	mTintColor = color;
}

float
Sprite::getRotation() const
{
	return mRotation;
}

void
Sprite::setRotation(float rotation)
{
	mRotation = rotation;
}

unsigned
Sprite::getFrameIndex() const
{
	return mFrameIndex;
}

void
Sprite::setFrameIndex(unsigned index)
{
	mFrameIndex = index % mFrames.size();
}

float
Sprite::getFrameDelay() const
{
	return mFrameDelay;
}

void
Sprite::setFrameDelay(float delay)
{
	mFrameDelay = delay;
}

glm::vec2
Sprite::getCenter() const
{
	return mFrameSize * 0.5f + mLocation;
}

const FloatRect&
Sprite::getSource() const
{
	return mFrames[mFrameIndex];
}

FloatRect
Sprite::getDestination() const
{
	return { mLocation, mFrameSize };
}

FloatRect
Sprite::getBoundingBox() const
{
	return {
		mLocation + mBoundingPadding,
		mFrameSize - mBoundingPadding * 2.f
	};
}

bool
Sprite::isBoxColliding(const FloatRect &other) const
{
	return getBoundingBox().intersect(other);
}

bool
Sprite::isCircleColliding(glm::vec2 otherCenter, float otherRadius)
{
	auto radiusSum = mCollisionRadius+otherRadius;
	otherCenter -= getCenter();
	return otherCenter.x * otherCenter.x + otherCenter.y * otherCenter.y <
		radiusSum * radiusSum;
}

float
Sprite::getCollisionRadius() const
{
	return mCollisionRadius;
}

void
Sprite::setCollisionRadius(float radius)
{
	mCollisionRadius = radius;
}

bool
Sprite::isAnimated() const
{
	return mAnimated;
}

void
Sprite::setAnimated(bool animated)
{
	mAnimated = animated;
}

void
Sprite::addFrame(const FloatRect &rect)
{
	mFrames.emplace_back(rect.pos / mTextureSize, rect.size / mTextureSize);
}

void
Sprite::update(float dt)
{
	if (mAnimated && (mFrameElapsed += dt) > mFrameDelay)
	{
		mFrameElapsed -= mFrameDelay;
		mFrameIndex++;
		if (mFrameIndex >= mFrames.size())
		{
			mFrameIndex = 0;
		}
	}
	mLocation += mVelocity * dt;
}

void
Sprite::draw(RenderTarget &target)
{
	auto srcRect = getSource();
	auto dstRect = getDestination();
	if (mRotation != 0.f)
	{
		target.blitQuad(
			mTexture,
			srcRect,
			dstRect,
			mTintColor,
			mRotation,
			mFrameSize * 0.5f,
			1.f);
	}
	else
	{
		target.blitQuad(mTexture, srcRect, dstRect, mTintColor);
	}
}
