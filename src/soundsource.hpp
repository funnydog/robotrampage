#pragma once

#include <glm/glm.hpp>

class SoundSource
{
public:
	enum Status
	{
		Stopped,
		Paused,
		Playing,
	};
public:
	SoundSource();
	SoundSource(const SoundSource &other);
	SoundSource& operator=(const SoundSource &other);
	SoundSource(SoundSource&& other) noexcept;
	SoundSource& operator=(SoundSource &&other) noexcept;

	virtual ~SoundSource();
	virtual Status getStatus() const;

	virtual void play() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;

	float getPitch() const;
	void setPitch(float pitch);

	float getVolume() const;
	void setVolume(float volume);

	glm::vec3 getPosition() const;
	void setPosition(glm::vec3 position);

	bool getRelative() const;
	void setRelative(bool relative);

	float getMinDistance() const;
	void setMinDistance(float distance);

	float getAttenuation() const;
	void setAttenuation(float attenuation);

protected:
	/** OpenAL sound identifier */
	unsigned mSource;
};
