#pragma once

#include <glm/glm.hpp>

#include "rect.hpp"

class Camera
{
public:
	explicit Camera(glm::vec2 position = {0.f, 0.f}, glm::vec2 size = {1.f, 1.f});

	const FloatRect& getWorldRectangle() const;
	void setWorldRectangle(const FloatRect &rectangle);

	glm::vec2 getPosition() const;
	void setPosition(glm::vec2 position);

	glm::vec2 getSize() const;
	void setSize(glm::vec2 size);

	float getRotation() const;
	void setRotation(float rotation);

	const FloatRect& getViewport() const;
	void setViewport(const FloatRect &viewport);

	bool isVisible(const FloatRect &bounds) const;

	void move(glm::vec2 offset);
	void rotate(float angle);
	void scale(float factor);

	const glm::mat4& getTransform() const;
	const glm::mat4& getInverse() const;

private:
	FloatRect mWorldRectangle;
	glm::vec2 mPositionMin;
	glm::vec2 mPositionMax;
	glm::vec2 mSize;
	float     mRotation;
	FloatRect mViewport;

	// cached elements
	mutable glm::mat4 mTransform;
	mutable glm::mat4 mInverse;
	mutable bool      mTransformNeedsUpdate;
	mutable bool      mInverseNeedsUpdate;
};
