package photostudio.image;

public class ImageData implements IImageData
{
	private final long _handle;
	
	public ImageData(long handle)
	{
		this._handle = handle;
	}
	
	@Override
	public boolean create(String imageName, ImageInfo imageInfo)
	{
		return this.createJni(imageName, imageInfo.width, imageInfo.height, imageInfo.channelCount, imageInfo.bitsPerChannel);
	}
	
	@Override
	public boolean create(String imageName, int width, int height, int channelCount, int bitsPerChannel)
	{
		return this.createJni(imageName, width, height, channelCount, bitsPerChannel);
	}
	private native boolean createJni(String imageName, int width, int height, int channelCount, int bitsPerChannel);
	
	@Override
	public boolean destroy()
	{
		return this.destroyJni();
	}
	private native boolean destroyJni();
	
	@Override
	public boolean isCreated()
	{
		return this.isCreatedJni();
	}
	private native boolean isCreatedJni();
	
	@Override
	public void setImageName(String imageName)
	{
		this.setImageNameJni(imageName);
	}
	private native void setImageNameJni(String imageName);
	
	@Override
	public String getImageName()
	{
		return this.getImageNameJni();
	}
	private native String getImageNameJni();
	
	@Override
	public ImageInfo getImageInfo()
	{
		return this.getImageInfoJni();
	}
	private native ImageInfo getImageInfoJni();
	
	@Override
	public int[] getPixel(int x, int y)
	{
		return this.getPixelJni(x, y);
	}
	private native int[] getPixelJni(int x, int y);
	
	@Override
	public boolean setPixel(int x, int y, int[] pixel)
	{
		return this.setPixelJni(x, y, pixel);
	}
	private native boolean setPixelJni(int x, int y, int[] pixel);
}
