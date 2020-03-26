#pragma once

#include <RiffTypes.h>

namespace Wave
{
	enum ChunkIds
	{
		ChunkId_Unknown = 0,

		ChunkId_WAVE = 'WAVE',

		ChunkId_fmt  = 'fmt ',
		ChunkId_data = 'data',
		ChunkId_fact = 'fact',
		ChunkId_cue  = 'cue ',
		ChunkId_plst = 'plst',
		ChunkId_list = 'list',
		ChunkId_labl = 'labl',
		ChunkId_note = 'note',
		ChunkId_ltxt = 'ltxt',
		ChunkId_smpl = 'smpl',
		ChunkId_inst = 'inst',
	};

	PRAGMA_PACK_PUSH(1)
	typedef struct tagFormatChunk
	{
		UInt32 Id;          // 'fmt '
		UInt32 Size;
		UInt16 FormatTag;
		UInt16 Channels;
		UInt32 SamplesPerSec;
		UInt32 AvgBytesPerSec;
		UInt16 BlockAlign;
		UInt16 BitsPerSample;
	} FORMATCHUNK;
	PRAGMA_PACK_POP()

} // namespace Wave
