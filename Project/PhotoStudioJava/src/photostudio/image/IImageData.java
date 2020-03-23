package photostudio.image;

public interface IImageData
{
	public boolean create(String imageName, ImageInfo imageInfo);
	
	public boolean create(String imageName, int width, int height, int channelCount, int bitsPerChannel);
	
	public boolean destroy();
	
	public boolean isCreated();
	
	public void setImageName(String imageName);
	
	public String getImageName();
	
	public ImageInfo getImageInfo();
	
	public int[] getPixel(int x, int y);
	
	public boolean setPixel(int x, int y, int pixel[]);
}
