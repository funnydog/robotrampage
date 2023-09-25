#include <cassert>
#include "soundfileogg.hpp"

namespace
{

std::size_t read(void *buffer, std::size_t elementSize, std::size_t elementCount, void *data)
{
	assert(elementSize == 1);
	std::ifstream &stream = *static_cast<std::ifstream*>(data);
	stream.read(static_cast<char *>(buffer), elementCount);
	std::size_t count = stream.gcount();
	stream.clear();
	return count;
}

int seek(void *data, ogg_int64_t offset, int origin)
{
	static const std::ios_base::seekdir direction[] = {
		std::ios_base::beg,
		std::ios_base::cur,
		std::ios_base::end,
	};
	std::ifstream &stream = *static_cast<std::ifstream*>(data);
	stream.seekg(offset, direction[origin]);
	stream.clear();
	return 0;
}

long tell(void *data)
{
	std::ifstream &stream = *static_cast<std::ifstream*>(data);
	return static_cast<long>(stream.tellg());
}

const ov_callbacks callbacks{ read, seek, nullptr, tell };
}

SoundFileOgg::~SoundFileOgg()
{
	ov_clear(&mVorbis);
}

bool
SoundFileOgg::open(const std::string &filename, Info &info)
{
	mStream.open(filename, std::ios::binary);
	if (!mStream)
	{
		return false;
	}

	ov_clear(&mVorbis);
	const int status = ov_open_callbacks(&mStream, &mVorbis, nullptr, 0, callbacks);
	if (status < 0)
	{
		mStream.close();
		return false;
	}

	vorbis_info *vorbisInfo = ov_info(&mVorbis, -1);
	info.channelCount = static_cast<unsigned>(vorbisInfo->channels);
	info.sampleRate   = static_cast<unsigned>(vorbisInfo->rate);
	info.sampleCount  = static_cast<std::size_t>(
		ov_pcm_total(&mVorbis, -1) * vorbisInfo->channels);

	mChannelCount = info.channelCount;
	return true;
}

void
SoundFileOgg::seek(std::uint64_t sampleOffset)
{
	assert(mVorbis.datasource && "Vorbis datasource is missing. Call Open to initialize it.");
	ov_pcm_seek(&mVorbis, sampleOffset / mChannelCount);
}

std::size_t
SoundFileOgg::read(std::int16_t *samples, std::size_t size)
{
	assert(mVorbis.datasource && "Vorbis datasource is missing. Call Open to initialize it.");
	std::size_t count = 0;
	while (count < size)
	{
		const auto bytesRead = ov_read(
			&mVorbis,
			reinterpret_cast<char *>(samples),
			static_cast<int>((size - count) * sizeof(std::int16_t)),
			0, 2, 1, nullptr);
		if (bytesRead <= 0)
		{
			break;
		}
		auto samplesRead = bytesRead / sizeof(std::int16_t);
		count += samplesRead;
		samples += samplesRead;
	}
	return count;
}
