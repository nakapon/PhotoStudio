#include <Platform.h>

#include <GL/gl.h>

#include <imgui/imgui.h>

#include "ImageView.h"

#define DEBUG_SHOW_AREAS 0

template <typename T>
static void ConvertToTexture(IImageData* pDstImage, const IImageData* pSrcImage);

CImageView::CImageView()
	: m_TextureId(0)
	, m_TextureWidth(0)
	, m_TextureHeight(0)
{
	PFMemory::Zero(&this->m_ImageInfo, sizeof(this->m_ImageInfo));
}

CImageView::~CImageView()
{
	this->Clear();
}

void CImageView::Clear()
{
	if(this->m_TextureId != 0)
	{
		glDeleteTextures(1, &this->m_TextureId);
	}

	this->m_TextureId = 0;
	PFMemory::Zero(&this->m_ImageInfo, sizeof(this->m_ImageInfo));
	this->m_TextureWidth = 0;
	this->m_TextureHeight = 0;
}

void CImageView::SetImage(IImageData* pImageData)
{
	IImageData::IMAGEINFO ImageInfo = { 0 };

	UINT TextureWidth, TextureHeight;

	bool bResizeTexture = false;

	if(pImageData == nullptr || !pImageData->IsCreated())
	{
		this->Clear();
		return;
	}

	ImageInfo = pImageData->GetImageInfo();

	TextureWidth  = PFMath::RoundUpPowerOfTowU32(ImageInfo.Width);
	TextureHeight = PFMath::RoundUpPowerOfTowU32(ImageInfo.Height);

	bResizeTexture = this->m_TextureWidth != TextureWidth || this->m_TextureHeight != TextureHeight;

	this->m_ImageInfo = ImageInfo;

	this->m_TextureWidth  = TextureWidth;
	this->m_TextureHeight = TextureHeight;

	// Create texture object
	if(bResizeTexture)
	{
		this->m_TextureObject.Create(TEXT(""), this->m_TextureWidth, this->m_TextureHeight, 4, 8);
	}

	// Convert image data to texture format
	{
		UInt32 BytePerChannel = (ImageInfo.BitsPerChannel + 7) >> 3;

		switch(BytePerChannel)
		{
		case 1: ConvertToTexture<BYTE>(&this->m_TextureObject, pImageData); break;
		case 2: ConvertToTexture<WORD>(&this->m_TextureObject, pImageData); break;
		default: return;
		}
	}

	// Create a OpenGL texture identifier
	if(this->m_TextureId == 0)
	{
		glGenTextures(1, &this->m_TextureId);
		glBindTexture(GL_TEXTURE_2D, this->m_TextureId);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, this->m_TextureId);
	}

	// Upload pixels into texture
	if(bResizeTexture)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->m_TextureWidth, this->m_TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->m_TextureObject.GetDataPtr());
	}
	else
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->m_TextureWidth, this->m_TextureHeight, GL_RGBA, GL_UNSIGNED_BYTE, this->m_TextureObject.GetDataPtr());
	}
}

void CImageView::Render(HWND hWindow)
{
	UInt32 WindowFlags = 0;

	if(this->m_TextureId == 0)
		return;

	if(ImGui::BeginChild("ImageViewe", ImVec2(0.0f, 0.0f), false, WindowFlags))
	{
		ImGuiIO& io = ImGui::GetIO();

		ImVec2 Cursor;

		ImVec2 ContentPos; // Window coordinates
		ImVec2 ContentSize;

		Cursor = ImGui::GetCursorPos();

		// Calculate content area
		{
			ImVec2 WindowPos = ImGui::GetWindowPos(); // Screen coordinates

			ImVec2 ContentMin = ImGui::GetWindowContentRegionMin(); // Window coordinates
			ImVec2 ContentMax = ImGui::GetWindowContentRegionMax();

			ContentPos.x = ContentMin.x + WindowPos.x; // window -> screen coordinates
			ContentPos.y = ContentMin.y + WindowPos.y;

			ContentSize.x = ContentMax.x - ContentMin.x;
			ContentSize.y = ContentMax.y - ContentMin.y;
		}

		ImVec2 ViewImageSize;

		{
			float ScaleH, ScaleV, Scale;

			ScaleH = ContentSize.x / this->m_ImageInfo.Width;
			ScaleV = ContentSize.y / this->m_ImageInfo.Height;
			Scale = PFMath::Min(ScaleH, ScaleV);

			ViewImageSize.x = Scale * this->m_ImageInfo.Width;
			ViewImageSize.y = Scale * this->m_ImageInfo.Height;
		}

		{
			ImVec2 ViewImageCursor;

			ImVec2 uv0(0.0f, 0.0f);
			ImVec2 uv1;

			ViewImageCursor.x = Cursor.x + (ContentSize.x - ViewImageSize.x) / 2.0f;
			ViewImageCursor.y = Cursor.y + (ContentSize.y - ViewImageSize.y) / 2.0f;

			uv1.x = (float)this->m_ImageInfo.Width  / this->m_TextureWidth;
			uv1.y = (float)this->m_ImageInfo.Height / this->m_TextureHeight;

			ImGui::SetCursorPos(ViewImageCursor);

			ImGui::Image((ImTextureID)this->m_TextureId, ViewImageSize, uv0, uv1);
		}

		// Debug Frame
#if DEBUG_SHOW_AREAS
		// Content Area (Yellow)
		ImGui::GetForegroundDrawList()->AddRect(ImVec2(ContentPos.x, ContentPos.y), ImVec2(ContentPos.x + ContentSize.x, ContentPos.y + ContentSize.y), IM_COL32(255, 255, 0, 255));
#endif
	}

	ImGui::EndChild();
}

template <typename T>
static void ConvertToTexture(IImageData* pDstImage, const IImageData* pSrcImage)
{
	IImageData::IMAGEINFO SrcInfo, DstInfo;

	const BYTE* pbySrcBits;
	const T* pSrcLine, *pSrcPixel;

	BYTE* pbyDstBits, *pbyDstLine, *pbyDstPixel;

	Int32 BitShift;

	SrcInfo = pSrcImage->GetImageInfo();
	DstInfo = pDstImage->GetImageInfo();

	pbySrcBits = pSrcImage->GetDataPtr();
	pbyDstBits = pDstImage->GetDataPtr();

	BitShift = (8 < SrcInfo.BitsPerChannel) ? (SrcInfo.BitsPerChannel - 8) : 0;

	if(SrcInfo.ChannelCount == 1)
	{
		for(UInt32 y = 0; y < SrcInfo.Height; y++)
		{
			pSrcLine = (T *)&pbySrcBits[y * SrcInfo.BytesPerLine];
			pbyDstLine = &pbyDstBits[y * DstInfo.BytesPerLine];

			for(UInt32 x = 0; x < SrcInfo.Width; x++)
			{
				T SrcValue = pSrcLine[x];
				pbyDstPixel = &pbyDstLine[x * DstInfo.ChannelCount];

				SrcValue >>= BitShift;

				pbyDstPixel[0] = (BYTE)SrcValue;
				pbyDstPixel[1] = (BYTE)SrcValue;
				pbyDstPixel[2] = (BYTE)SrcValue;
				pbyDstPixel[3] = 255;
			}
		}
	}
	else
	{
		for(UInt32 y = 0; y < SrcInfo.Height; y++)
		{
			pSrcLine = (T *)&pbySrcBits[y * SrcInfo.BytesPerLine];
			pbyDstLine = &pbyDstBits[y * DstInfo.BytesPerLine];

			for(UInt32 x = 0; x < SrcInfo.Width; x++)
			{
				pSrcPixel = &pSrcLine[x * SrcInfo.ChannelCount];
				pbyDstPixel = &pbyDstLine[x * DstInfo.ChannelCount];

				pbyDstPixel[0] = (BYTE)(pSrcPixel[0] >> BitShift);
				pbyDstPixel[1] = (BYTE)(pSrcPixel[1] >> BitShift);
				pbyDstPixel[2] = (BYTE)(pSrcPixel[2] >> BitShift);
				pbyDstPixel[3] = 255;
			}
		}
	}
}
