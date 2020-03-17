#pragma once

class IImageData
{
public:
	typedef struct
	{
		UINT Width;			/* 幅 pix */
		UINT Height;		/* 高さ pix */
		UINT ChannelCount;	/* チャンネル数 */
		UINT BitsPerChannel;/* 1チャンネル当たりのビット数 */
		UINT BytesPerLine;	/* メモリ上の横1ラインのバイト数 */
	} IMAGEINFO;

public:
	virtual ~IImageData(){}

	// 画像データ作成
	// BytesPerLine は指定しても無視される（内部で自動計算する）
	virtual bool Create(LPCTSTR pszImageName, const IMAGEINFO& ImageInfo) = 0;

	// 画像データ作成
	virtual bool Create(LPCTSTR pszImageName, UINT Width, UINT Height, UINT ChannelCount, UINT BitsPerChannel) = 0;

	// 画像データ破棄
	virtual void Destroy() = 0;

	// 画像データ作成済み？
	virtual bool IsCreated() const = 0;

	// 画像名取得
	virtual LPCTSTR GetImageName() const = 0;

	// 画像名変更
	virtual void SetImageName(LPCTSTR pszImageName) = 0;

	// 画像情報取得
	virtual const IMAGEINFO& GetImageInfo() const = 0;

	// 画像データのポインタ取得
	virtual const BYTE* GetDataPtr() const = 0;
	virtual BYTE* GetDataPtr() = 0;

	/* 例） 例えば 横3pix 縦2pix チャンネル数 3 ビット数 8 (RGB 24bit) の場合，以下のようにメモリ上にデータが並ぶ

			Width = 3;
			Height = 2;
			ChannelCount = 3;
			BitsPerChannel = 8;
			BytesPerLine = Width * ChannelCount * ((BitsPerChannel + 7) / 8) + padding;  ※ padding はアライメント調整の詰め物

			[R(0,0)][G(0,0)][B(0,0)][R(1,0)][G(1,0)][B(1,0)][R(2,0)][G(2,0)][B(2,0)][ padding ]
			[R(0,1)][G(0,1)][B(0,1)][R(1,1)][G(1,1)][B(1,1)][R(2,1)][G(2,1)][B(2,1)][ padding ]
			|<------------------------- BytesPerLine ---------------------------------------->|

			※ [R(x,y)] は 座標(x,y) のR画素値(BitsPerChannel が8なら1バイトなので 0 - 255)

	*/
};
