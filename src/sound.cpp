#include "alcheck.hpp"
#include "sound.hpp"
#include "soundbuffer.hpp"

Sound::Sound(const SoundBuffer &soundBuffer)
	: SoundSource()
{
	setBuffer(soundBuffer);
}

Sound::~Sound()
{
	stop();
}

const SoundBuffer *
Sound::getBuffer() const
{
	return mBuffer;
}

void
Sound::setBuffer(const SoundBuffer &buffer)
{
	if (mBuffer)
	{
		stop();
	}
	mBuffer = &buffer;
	alCheck(alSourcei(mSource, AL_BUFFER, static_cast<ALint>(mBuffer->mBuffer)));
}

void
Sound::play()
{
	assert(mSource && "Sound moved out");
	alCheck(alSourcePlay(mSource));
}

void
Sound::pause()
{
	assert(mSource && "Sound moved out");
	alCheck(alSourcePause(mSource));
}

void
Sound::stop()
{
	assert(mSource && "Sound moved out");
	alCheck(alSourceStop(mSource));
}
