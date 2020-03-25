#include <Platform.h>

#include <Endian.h>

#include <WaveFileTypes.h>

#include <WaveAudioReader.h>

bool WaveAudioReader::ReadAudio(LPCTSTR pszFilePath, IAudioData* pAudioData)
{
	bool bReturn = false;

	IAudioData::AUDIOINFO AudioInfo = { };

	FILE* fp;

	if(pszFilePath == nullptr || pAudioData == nullptr)
		return false;

	fp = _tfopen(pszFilePath, TEXT("rb"));
	if(fp == nullptr)
		return false;

	{
		Riff::RIFFCHUNK Chunk = { };

		if(0 == fread(&Chunk, sizeof(Chunk), 1, fp))
			goto LABEL_RETURN;

		Chunk.Id = Endian::SwapDword(Chunk.Id);
		Chunk.FormType = Endian::SwapDword(Chunk.FormType);

		if(Chunk.Id != Riff::ChunkId_RIFF || Chunk.FormType != Wave::ChunkId_WAVE)
			goto LABEL_RETURN;
	}

	for(;;)
	{
		Riff::CHUNKHEADER Header = { };

		if(0 == fread(&Header, sizeof(Header), 1, fp))
			break;

		Header.Id = Endian::SwapDword(Header.Id);

		switch(Header.Id)
		{
		case Wave::ChunkId_fmt:
			{
				Wave::FORMATCHUNK FormatChunk = { };

				if(0 == fread(&FormatChunk.FormatTag, sizeof(FormatChunk) - Riff::CHUNKHEADER_SIZE, 1, fp))
					goto LABEL_RETURN;

				AudioInfo.ChannelCount   = FormatChunk.Channels;
				AudioInfo.BitsPerChannel = FormatChunk.BitsPerSample;
				AudioInfo.SamplesPerSec  = FormatChunk.SamplesPerSec;
				AudioInfo.BytesPerSample = FormatChunk.BlockAlign;
				AudioInfo.BytesPerSec    = FormatChunk.AvgBytesPerSec;
				AudioInfo.SampleCount    = 0;
			}
			break;

		case Wave::ChunkId_data:
			if(AudioInfo.BytesPerSample != 0)
			{
				UInt32 SampleCount = Header.Size / AudioInfo.BytesPerSample;

				if(!pAudioData->Create(pszFilePath
									 , AudioInfo.ChannelCount
									 , AudioInfo.BitsPerChannel
									 , AudioInfo.SamplesPerSec
									 , SampleCount))
					goto LABEL_RETURN;

				if(0 == fread(pAudioData->GetDataPtr(), Header.Size, 1, fp))
					goto LABEL_RETURN;
			}
			break;

		case Riff::ChunkId_LIST:
		default:
			if(0 != fseek(fp, Header.Size, SEEK_CUR))
				goto LABEL_RETURN;
			break;
		}
	}

	bReturn = pAudioData->IsCreated();

LABEL_RETURN:

	fclose(fp);
	fp = nullptr;

	return bReturn;
}
