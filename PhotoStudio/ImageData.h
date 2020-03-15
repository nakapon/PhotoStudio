#pragma once

#include <vector>

typedef struct
{
	UINT Width;			/* �� pix */
	UINT Height;		/* ���� pix */
	UINT ChannelCount;	/* �`�����l���� */
	UINT BitsPerChannel;/* 1�`�����l��������̃r�b�g�� */
	UINT BytesPerLine;	/* ��������̉�1���C���̃o�C�g�� */
} IMAGEINFO;

class CImageData
{
public:
	CImageData();
	~CImageData();
	CImageData(const CImageData& ImageData);
	CImageData& operator=(const CImageData& ImageData);

	// �摜�f�[�^�쐬�i��̍��f�[�^�j
	// BytesPerLine �͎w�肵�Ă����������i�����Ŏ����v�Z����j
	bool Create(const IMAGEINFO& ImageInfo);

	// �摜�f�[�^�j��
	void Destroy();

	// �摜�f�[�^�쐬�ς݁H
	bool IsCreated() const;

	// �摜���擾
	const IMAGEINFO& GetImageInfo() const;

	// �摜�f�[�^�̃|�C���^�擾
	const BYTE* GetDataPtr() const;
	BYTE* GetDataPtr();

	/* ��j �Ⴆ�� ��3pix �c2pix �`�����l���� 3 �r�b�g�� 8 (RGB 24bit) �̏ꍇ�C�ȉ��̂悤�Ƀ�������Ƀf�[�^������

			Width = 3;
			Height = 2;
			ChannelCount = 3;
			BitsPerChannel = 8;
			BytesPerLine = Width * ChannelCount * ((BitsPerChannel + 7) / 8) + padding;  �� padding �̓A���C�����g�����̋l�ߕ�

			[R(0,0)][G(0,0)][B(0,0)][R(1,0)][G(1,0)][B(1,0)][R(2,0)][G(2,0)][B(2,0)][ padding ]
			[R(0,1)][G(0,1)][B(0,1)][R(1,1)][G(1,1)][B(1,1)][R(2,1)][G(2,1)][B(2,1)][ padding ]
			|<------------------------- BytesPerLine ---------------------------------------->|

			�� [R(x,y)] �� ���W(x,y) ��R��f�l(BitsPerChannel ��8�Ȃ�1�o�C�g�Ȃ̂� 0 - 255)

	*/

private:

	IMAGEINFO m_ImageInfo;

	std::vector<BYTE> m_ImageData;
};
