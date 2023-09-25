#pragma once

#include "soundsource.hpp"

class SoundBuffer;

class Sound: public SoundSource
{
public:
	explicit Sound(const SoundBuffer &soundBuffer);
	virtual ~Sound() override;

	Sound(const Sound &other) = default;
	Sound& operator=(const Sound &other) = default;
	Sound(Sound &&other) noexcept = default;
	Sound& operator=(Sound &&other) noexcept = default;

	const SoundBuffer *getBuffer() const;
	void setBuffer(const SoundBuffer &buffer);

	// derived from SoundSource
	virtual void play() override;
	virtual void pause() override;
	virtual void stop() override;

private:
	Sound() = default;

	/** SoundBuffer bound to the source */
	const SoundBuffer *mBuffer = nullptr;
};
