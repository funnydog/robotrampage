#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

class Sound;

class SoundBuffer
{
public:
	using Ptr = std::unique_ptr<SoundBuffer>;

public:
	SoundBuffer();
	~SoundBuffer();

	SoundBuffer(const SoundBuffer &other) = delete;
	SoundBuffer& operator=(const SoundBuffer &other) = delete;
	SoundBuffer(SoundBuffer &&other) noexcept = delete;
	SoundBuffer& operator=(SoundBuffer &&other) noexcept = delete;

	bool loadFromFile(const std::string &filename);

	unsigned getFormat() const;
	unsigned getSampleCount() const;
	unsigned getSampleRate() const;
	unsigned getChannelCount() const;

private:
	unsigned mBuffer;
	unsigned mFormat;
	unsigned mSampleCount;
	unsigned mSampleRate;
	unsigned mChannelCount;

	friend class Sound;
};
