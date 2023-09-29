#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "color.hpp"
#include "rect.hpp"

class RenderTarget;
class Texture;

class Sprite
{
public:
	Sprite(const Texture &texture, FloatRect initialFrame, glm::vec2 location, glm::vec2 velocity);

	glm::vec2 getLocation() const;
	void setLocation(glm::vec2 location);

	glm::vec2 getVelocity() const;
	void setVelocity(glm::vec2 velocity);

	Color getTintColor() const;
	void setTintColor(Color color);

	float getRotation() const;
	void setRotation(float rotation);

	unsigned getFrameIndex() const;
	void setFrameIndex(unsigned frame);

	float getFrameDelay() const;
	void setFrameDelay(float delay);

	glm::vec2 getCenter() const;
	const FloatRect& getSource() const;
	FloatRect getDestination() const;

	FloatRect getBoundingBox() const;
	bool isBoxColliding(const FloatRect &other) const;
	bool isCircleColliding(glm::vec2 otherCenter, float otherRadius);

	float getCollisionRadius() const;
	void setCollisionRadius(float radius);

	glm::vec2 getBoundingPadding() const;
	void setBoundingPadding(glm::vec2 padding);

	bool isExpired() const;
	void setExpired(bool expired);

	bool isAnimated() const;
	void setAnimated(bool animated);

	bool isAnimatedWhenStopped() const;
	void setAnimatedWhenStopped(bool animatedWhenStopped);

	void addFrame(const FloatRect &rect);

	virtual void update(float dt);
	virtual void draw(RenderTarget &target);

protected:
	// drawable
	const Texture &mTexture;
	glm::vec2 mTextureSize;
	std::vector<FloatRect> mFrames;
	glm::vec2 mFrameSize;
	Color mTintColor;

	// animation
	unsigned mFrameIndex;
	float mFrameElapsed;
	float mFrameDelay;
	bool mExpired;
	bool mAnimated;
	bool mAnimatedWhenStopped;

	// position
	glm::vec2 mLocation;
	glm::vec2 mVelocity;
	float mRotation;

	// collision
	float mCollisionRadius;
	glm::vec2 mBoundingPadding;
};
