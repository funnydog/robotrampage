#pragma once

#include <fstream>

#include "soundfile.hpp"

class SoundFileWav: public SoundFile
{
public:
	virtual bool open(const std::string &filename, Info &info) override;
	virtual void seek(std::uint64_t sampleOffset) override;
	virtual std::size_t read(std::int16_t *samples, std::size_t size) override;

private:
	std::ifstream mStream;
	unsigned      mBytesPerSample = 0;
	std::uint64_t mDataStart      = 0;
	std::uint64_t mDataEnd        = 0;
};
