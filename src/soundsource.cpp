#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "alcheck.hpp"
#include "audiodevice.hpp"
#include "soundsource.hpp"

SoundSource::SoundSource()
{
	alCheck(alGenSources(1, &mSource));
	alCheck(alSourcei(mSource, AL_BUFFER, 0));
}

SoundSource::SoundSource(const SoundSource &other)
{
	ALint buffer;
	alCheck(alGetSourcei(other.mSource, AL_BUFFER, &buffer));

	alCheck(alGenSources(1, &mSource));
	alCheck(alSourcei(mSource, AL_BUFFER, buffer));

	setPitch(other.getPitch());
	setVolume(other.getVolume());
	setPosition(other.getPosition());
	setRelative(other.getRelative());
	setMinDistance(other.getMinDistance());
	setAttenuation(other.getAttenuation());
}

SoundSource&
SoundSource::operator=(const SoundSource &other)
{
	ALint buffer;
	alCheck(alGetSourcei(other.mSource, AL_BUFFER, &buffer));
	alCheck(alSourcei(mSource, AL_BUFFER, buffer));
	setPitch(other.getPitch());
	setVolume(other.getVolume());
	setPosition(other.getPosition());
	setRelative(other.getRelative());
	setMinDistance(other.getMinDistance());
	setAttenuation(other.getAttenuation());

	return *this;
}

SoundSource::SoundSource(SoundSource &&other) noexcept
	: mSource(0)
{
	std::swap(mSource, other.mSource);
}

SoundSource&
SoundSource::operator=(SoundSource &&other) noexcept
{
	std::swap(mSource, other.mSource);
	return *this;
}

SoundSource::~SoundSource()
{
	if (mSource)
	{
		alCheck(alSourcei(mSource, AL_BUFFER, 0));
		alCheck(alDeleteSources(1, &mSource));
	}
}

float
SoundSource::getPitch() const
{
	assert(mSource && "OpenAL Source moved out");
	float pitch;
	alCheck(alGetSourcef(mSource, AL_PITCH, &pitch));
	return pitch;
}

void
SoundSource::setPitch(float pitch)
{
	assert(mSource && "OpenAL Source moved out");
	alCheck(alSourcef(mSource, AL_PITCH, pitch));
}

float
SoundSource::getVolume() const
{
	assert(mSource && "OpenAL Source moved out");
	float volume;
	alCheck(alGetSourcef(mSource, AL_GAIN, &volume));
	return volume * 100.f;
}

void
SoundSource::setVolume(float volume)
{
	assert(mSource && "OpenAL Source moved out");
	if (0.f <= volume && volume <= 100.f)
	{
		alCheck(alSourcef(mSource, AL_GAIN, volume * 0.01f));
	}
}

glm::vec3
SoundSource::getPosition() const
{
	assert(mSource && "OpenAL Source moved out");
	glm::vec3 position;
	alCheck(alGetSourcefv(mSource, AL_POSITION, glm::value_ptr(position)));
	return position;
}

void
SoundSource::setPosition(glm::vec3 position)
{
	assert(mSource && "OpenAL Source moved out");
	alCheck(alSourcefv(mSource, AL_POSITION, glm::value_ptr(position)));
}

bool
SoundSource::getRelative() const
{
	assert(mSource && "OpenAL Source moved out");
	ALint relative;
	alCheck(alGetSourcei(mSource, AL_SOURCE_RELATIVE, &relative));
	return relative != 0;
}

void
SoundSource::setRelative(bool relative)
{
	assert(mSource && "OpenAL Source moved out");
	alCheck(alSourcei(mSource, AL_SOURCE_RELATIVE, relative));
}

float
SoundSource::getMinDistance() const
{
	assert(mSource && "OpenAL Source moved out");
	float distance;
	alCheck(alGetSourcef(mSource, AL_REFERENCE_DISTANCE, &distance));
	return distance;
}

void
SoundSource::setMinDistance(float distance)
{
	assert(mSource && "OpenAL Source moved out");
	alCheck(alSourcef(mSource, AL_REFERENCE_DISTANCE, distance));
}

float
SoundSource::getAttenuation() const
{
	assert(mSource && "OpenAL Source moved out");
	float attenuation;
	alCheck(alGetSourcef(mSource, AL_ROLLOFF_FACTOR, &attenuation));
	return attenuation;
}

void
SoundSource::setAttenuation(float attenuation)
{
	assert(mSource && "OpenAL Source moved out");
	alCheck(alSourcef(mSource, AL_ROLLOFF_FACTOR, attenuation));
}

SoundSource::Status
SoundSource::getStatus() const
{
	assert(mSource && "OpenAL Source moved out");
	ALint status;
	alCheck(alGetSourcei(mSource, AL_SOURCE_STATE, &status));

	switch (status)
	{
	default:
	case AL_STOPPED: return Stopped;
	case AL_PAUSED:  return Paused;
	case AL_PLAYING: return Playing;
	}
}
