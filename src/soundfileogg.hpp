#pragma once

#include <fstream>

#include <vorbis/vorbisfile.h>

#include "soundfile.hpp"

class SoundFileOgg: public SoundFile
{
public:
	virtual ~SoundFileOgg() override;

	virtual bool open(const std::string &filename, Info &info) override;
	virtual void seek(std::uint64_t sampleOffset) override;
	virtual std::size_t read(std::int16_t *samples, std::size_t size) override;

private:
	std::ifstream  mStream;
	OggVorbis_File mVorbis{};
	unsigned       mChannelCount{};
};
