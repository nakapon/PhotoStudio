package photostudio.image;

public final class ImageReader
{
	private ImageReader(){}
	
	public native static boolean read(String filePath, IImageData imageData);
	
}
