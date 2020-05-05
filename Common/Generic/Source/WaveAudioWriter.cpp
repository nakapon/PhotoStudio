#include <Platform.h>

#include <Endian.h>

#include <WaveFileTypes.h>

#include <WaveAudioWriter.h>

bool WaveAudioWriter::WriteAudio(LPCTSTR pszFilePath, const IAudioData* pAudioData)
{
	bool bReturn = false;

	FILE* fp;

	IAudioData::SAudioInfo AudioInfo;

	if(pszFilePath == nullptr || pAudioData == nullptr || !pAudioData->IsCreated())
		return false;

	AudioInfo = pAudioData->GetAudioInfo();

	fp = _tfopen(pszFilePath, TEXT("wb"));
	if(fp == nullptr)
		return false;

	{
		Riff::RIFFCHUNK Chunk = { };

		Chunk.Id = Endian::SwapDword(Riff::ChunkId_RIFF);
		Chunk.Size = 0; // update later
		Chunk.FormType = Endian::SwapDword(Wave::ChunkId_WAVE);

		if(0 == fwrite(&Chunk, sizeof(Chunk), 1, fp))
			goto LABEL_RETURN;
	}

	{
		Wave::FORMATCHUNK FormatChunk = { };

		FormatChunk.Id = Endian::SwapDword(Wave::ChunkId_fmt);
		FormatChunk.Size = sizeof(FormatChunk) - Riff::CHUNKHEADER_SIZE;
		FormatChunk.FormatTag = 1; // WAVE_FORMAT_PCM
		FormatChunk.Channels = AudioInfo.ChannelCount;
		FormatChunk.SamplesPerSec = AudioInfo.SamplesPerSec;
		FormatChunk.AvgBytesPerSec = AudioInfo.BytesPerSec;
		FormatChunk.BlockAlign = AudioInfo.BytesPerSample;
		FormatChunk.BitsPerSample = AudioData::GetBitsPerChannel(AudioInfo.DataType);

		if(0 == fwrite(&FormatChunk, sizeof(FormatChunk), 1, fp))
			goto LABEL_RETURN;
	}

	{
		Riff::CHUNKHEADER DataChunk = { };

		DataChunk.Id = Endian::SwapDword(Wave::ChunkId_data);
		DataChunk.Size = AudioInfo.SampleCount * AudioInfo.BytesPerSample;

		if(0 == fwrite(&DataChunk, sizeof(DataChunk), 1, fp))
			goto LABEL_RETURN;

		if(0 == fwrite(pAudioData->GetDataPtr(), DataChunk.Size, 1, fp))
			goto LABEL_RETURN;
	}

	{
		UInt32 FileSize = (UInt32)ftell(fp);
		UInt32 ChunkSize = FileSize - Riff::CHUNKHEADER_SIZE;

		// back to size element of chunk header
		if(0 != fseek(fp, 4, SEEK_SET))
			goto LABEL_RETURN;

		// overwrite chunk size
		if(0 == fwrite(&ChunkSize, sizeof(ChunkSize), 1, fp))
			goto LABEL_RETURN;
	}

	bReturn = true;

LABEL_RETURN:

	fclose(fp);
	fp = nullptr;

	return bReturn;
}
