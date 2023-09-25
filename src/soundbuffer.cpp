#include "alcheck.hpp"
#include "soundbuffer.hpp"
#include "soundfileogg.hpp"
#include "soundfilewav.hpp"

SoundBuffer::SoundBuffer()
	: mBuffer(0)
	, mSampleCount(0)
	, mSampleRate(0)
	, mChannelCount(0)
{
	alCheck(alGenBuffers(1, &mBuffer));
}

SoundBuffer::~SoundBuffer()
{
	alCheck(alDeleteBuffers(1, &mBuffer));
}

bool
SoundBuffer::loadFromFile(const std::string &filename)
{
	SoundFile::Info info;
	auto filePtr = SoundFile::createFromFilename(filename, info);

	if (!filePtr
	    || info.channelCount == 0 || info.sampleRate == 0 || info.sampleCount == 0)
	{
		return false;
	}

	std::vector<std::int16_t> samples(info.sampleCount, 0);
	if (filePtr->read(samples.data(), info.sampleCount) != info.sampleCount)
	{
		return false;
	}

	mFormat = 0;
	switch (info.channelCount)
	{
	case 1: mFormat = AL_FORMAT_MONO16; break;
	case 2: mFormat = AL_FORMAT_STEREO16; break;
	case 4: mFormat = alGetEnumValue("AL_FORMAT_QUAD16"); break;
	case 6: mFormat = alGetEnumValue("AL_FORMAT_51CHN16"); break;
	case 7: mFormat = alGetEnumValue("AL_FORMAT_61CHN16"); break;
	case 8: mFormat = alGetEnumValue("AL_FORMAT_71CHN16"); break;
	default: break;
	}
	if (mFormat == -1U)
	{
		mFormat = 0;
	}

	mSampleCount = info.sampleCount;
	mSampleRate = info.sampleRate;
	mChannelCount = info.channelCount;
	alCheck(alBufferData(
			mBuffer,
			mFormat,
			samples.data(),
			samples.size() * sizeof(std::int16_t),
			mSampleRate));
	return true;
}

unsigned
SoundBuffer::getFormat() const
{
	return mFormat;
}

unsigned
SoundBuffer::getSampleCount() const
{
	return mSampleCount;
}

unsigned
SoundBuffer::getSampleRate() const
{
	return mSampleRate;
}

unsigned
SoundBuffer::getChannelCount() const
{
	return mChannelCount;
}
