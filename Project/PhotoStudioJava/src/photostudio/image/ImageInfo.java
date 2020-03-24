package photostudio.image;

public class ImageInfo
{
	public ImageInfo()
	{
	}
	
	public ImageInfo(int width, int height, int channelCount, int bitsPerChannel)
	{
		this.width = width;
		this.height = height;
		this.channelCount = channelCount;
		this.bitsPerChannel = bitsPerChannel;
		this.bytesPerLine = 0;
	}
	
	public int width;
	
	public int height;
	
	public int channelCount;
	
	public int bitsPerChannel;
	
	public int bytesPerLine;
}
