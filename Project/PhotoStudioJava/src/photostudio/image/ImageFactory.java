package photostudio.image;

public final class ImageFactory
{
	private ImageFactory(){}
	
	public native static IImageData createInstance();
	
	public native static IImageData duplicateInstance(IImageData imageData);
	
	public native static void destroyInstance(IImageData imageData);
}
