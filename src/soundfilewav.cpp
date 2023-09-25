#include <cassert>

#include "soundfilewav.hpp"

namespace
{

struct MasterChunk
{
	char          chunkId[4];
	std::uint32_t chunkSize;
	char          waveId[4];
};

struct ChunkHeader
{
	char          chunkId[4];
	std::uint32_t chunkSize;
};

struct FormatChunk
{
	std::uint16_t wFormatTag;
	std::uint16_t nChannels;
	std::uint32_t nSamplesPerSec;
	std::uint32_t nAvgBytesPerSec;
	std::uint16_t bBlockAlign;
	std::uint16_t nBitsPerSample;
	std::uint16_t cbSize;
	std::uint16_t wValidBitsPerSample;
	std::uint32_t dwChannelMask;
	char          SubFormat[16];
};

}

bool
SoundFileWav::open(const std::string &filename, Info &info)
{
	mBytesPerSample = 0;
	mDataStart = mDataEnd = 0;

	mStream.open(filename, std::ios::in|std::ios::binary);
	if (!mStream)
	{
		return false;
	}

	// check the correctness of the signature
	MasterChunk master;
	if (!mStream.read(reinterpret_cast<char*>(&master), sizeof(master))
	    || !std::equal(master.chunkId+0, master.chunkId+4, "RIFF")
	    || !std::equal(master.waveId+0, master.waveId+4, "WAVE"))
	{
		return false;
	}

	// read the relevant chunks
	ChunkHeader header;
	while (mStream.read(reinterpret_cast<char *>(&header), sizeof(header)))
	{
		if (std::equal(header.chunkId+0, header.chunkId+4, "fmt "))
		{
			FormatChunk format;
			if (!mStream.read(reinterpret_cast<char *>(&format), header.chunkSize)
			    || format.wFormatTag != 1)
			{
				return false;
			}
			info.channelCount = format.nChannels;
			info.sampleRate = format.nSamplesPerSec;
			mBytesPerSample = format.nAvgBytesPerSec / format.nSamplesPerSec;
		}
		else if (std::equal(header.chunkId+0, header.chunkId+1, "data"))
		{
			mDataStart = mStream.tellg();
			mDataEnd = mDataStart + header.chunkSize;
			break;
		}
		else
		{
			mStream.seekg(header.chunkSize, std::ios::cur);
		}
	}

	if (!mDataStart || !mBytesPerSample)
	{
		return false;
	}

	info.sampleCount = (mDataEnd - mDataStart) / mBytesPerSample;
	return true;
}

void
SoundFileWav::seek(std::uint64_t sampleOffset)
{
	assert(mStream && "Stream is not open");
	assert(mDataStart + sampleOffset * mBytesPerSample < mDataEnd);
	mStream.seekg(mDataStart + sampleOffset * mBytesPerSample);
}

std::size_t
SoundFileWav::read(std::int16_t *samples, std::size_t maxCount)
{
	assert(0 < mBytesPerSample && mBytesPerSample <= 4 && "Unrecognized format");

	std::size_t count;
	std::uint64_t pos;
	for (count = 0, pos = mStream.tellg();
	     count < maxCount && pos < mDataEnd;
	     count++, pos += mBytesPerSample)
	{
		unsigned char bytes[4];
		if (!mStream.read(reinterpret_cast<char*>(bytes), mBytesPerSample))
		{
			break;
		}
		uint32_t sample = 0;
		for (unsigned i = 0; i < mBytesPerSample; i++)
		{
			sample |= static_cast<uint32_t>(bytes[i]) << (i * 8);
		}
		switch (mBytesPerSample)
		{
		case 1: *samples++ = sample << 8; break;
		case 2: *samples++ = sample; break;
		case 3: *samples++ = sample >> 8; break;
		case 4: *samples++ = sample >> 16; break;
		default:
			assert(false && "Unsupported number of bytes per sample");
			break;
		}
	}
	return count;
}
