#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "camera.hpp"

Camera::Camera(glm::vec2 position, glm::vec2 size)
	: mWorldRectangle(position, size)
	, mPositionMin(position)
	, mPositionMax(position)
	, mSize(size)
	, mRotation(0.f)
	, mViewport(position, size)
	, mTransform(1.f)
	, mTransformNeedsUpdate(true)
	, mInverseNeedsUpdate(true)
{
}

glm::vec2
Camera::getPosition() const
{
	return mViewport.pos;
}

void
Camera::setPosition(glm::vec2 position)
{
	mViewport.pos = glm::clamp(position, mPositionMin, mPositionMax);
	mTransformNeedsUpdate = true;
	mInverseNeedsUpdate = true;
}

const FloatRect&
Camera::getWorldRectangle() const
{
	return mWorldRectangle;
}

void
Camera::setWorldRectangle(const FloatRect &worldRect)
{
	mWorldRectangle = worldRect;
	mPositionMin = mWorldRectangle.pos;
	mPositionMax = mWorldRectangle.pos + mWorldRectangle.size - mViewport.size;
	setPosition(getPosition());
}

glm::vec2
Camera::getSize() const
{
	return mSize;
}

void
Camera::setSize(glm::vec2 size)
{
	mSize = size;
	mTransformNeedsUpdate = true;
	mInverseNeedsUpdate = true;
}

float
Camera::getRotation() const
{
	return mRotation;
}

void
Camera::setRotation(float rotation)
{
	mRotation = rotation;
	mTransformNeedsUpdate = true;
	mInverseNeedsUpdate = true;
}

const FloatRect&
Camera::getViewport() const
{
	return mViewport;
}

void
Camera::setViewport(const FloatRect &viewport)
{
	mViewport = viewport;
	mPositionMin = mWorldRectangle.pos;
	mPositionMax = mWorldRectangle.pos + mWorldRectangle.size - mViewport.size;
}

bool
Camera::isVisible(const FloatRect &bounds) const
{
	return mViewport.intersect(bounds);
}

void
Camera::move(glm::vec2 offset)
{
	setPosition(getPosition() + offset);
}

void
Camera::rotate(float angle)
{
	setRotation(mRotation + angle);
}

void
Camera::scale(float factor)
{
	setSize(mSize * factor);
}

const glm::mat4&
Camera::getTransform() const
{
	if (mTransformNeedsUpdate)
	{
		mTransformNeedsUpdate = false;

		const glm::vec2 center = mSize * 0.5f + getPosition();
		const float angle = glm::radians(mRotation);
		const float cos = std::cos(angle);
		const float sin = std::sin(angle);
		const float tx = -center.x * cos - center.y * sin + center.x;
		const float ty = center.x * sin - center.y * cos + center.y;

		const float a = 2.f / mSize.x;
		const float b = -2.f / mSize.y;
		const float c = -a * center.x;
		const float d = -b * center.y;

		mTransform[0][0] = a * cos;
		mTransform[0][1] = -b * sin;
		mTransform[1][0] = a * sin;
		mTransform[1][1] = b * cos;
		mTransform[3][0] = a * tx + c;
		mTransform[3][1] = b * ty + d;
	}
	return mTransform;
}

const glm::mat4&
Camera::getInverse() const
{
	if (mInverseNeedsUpdate)
	{
		mInverseNeedsUpdate = false;
		mInverse = glm::affineInverse(getTransform());
	}
	return mInverse;
}
