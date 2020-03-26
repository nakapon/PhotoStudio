#pragma once

/*
 *  RIFF Chunk
 *  +-------------------+
 *  |  ID (4)           |
 *  +-------------------+
 *  |  Size (4)         |
 *  +-------------------+
 *  |  FormType (4)     |
 *  +-------------------+
 *  |  RIFF Chunk Data  |
 *  |                   |
 *  |    +-------------------+ <-- Subchunk
 *  |    |  ID (4)           |
 *  |    +-------------------+
 *  |    |  Size (4)         |
 *  |    +-------------------+
 *  |    |  Data (n)         |
 *  |    |                   |
 *  |    +-------------------+
 *  |    +-------------------+ <-- Subchunk
 *  |    |  ID (4)           |
 *  |    +-------------------+
 *  |    |  Size (4)         |
 *  |    +-------------------+
 *  |    |  Data (n)         |
 *  |    |                   |
 *  |    +-------------------+
 *  |                   |
 *  +-------------------+
 */

namespace Riff
{
	static const UInt32 CHUNKHEADER_SIZE = 8;

	PRAGMA_PACK_PUSH(1)
	typedef struct tagChunkHeader
	{
		UInt32 Id;      // FOURCC (four-character code) specifying the chunk identifier
		UInt32 Size;    // Size of the data member in the chunk
	} CHUNKHEADER;
	PRAGMA_PACK_POP()

	PRAGMA_PACK_PUSH(1)
	typedef struct tagRiffChunk
	{
		UInt32 Id;       // 'RIFF'
		UInt32 Size;
		UInt32 FormType;
	} RIFFCHUNK;
	PRAGMA_PACK_POP()

	PRAGMA_PACK_PUSH(1)
	typedef struct tagListChunk
	{
		UInt32 Id;       // 'LIST'
		UInt32 Size;
		UInt32 ListType;
	} LISTCHUNK;
	PRAGMA_PACK_POP()

	enum ChunkIds
	{
		ChunkId_Unknown = 0,

		ChunkId_RIFF = 'RIFF',
		ChunkId_LIST = 'LIST',
	};

} // namespace Riff
