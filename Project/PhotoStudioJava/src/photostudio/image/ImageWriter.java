package photostudio.image;

public final class ImageWriter
{
	private ImageWriter(){}
	
	public native static boolean write(String filePath, IImageData imageData);
	
}
