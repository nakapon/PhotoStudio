package photostudio.image;

import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;

public final class ImageLibrary
{
	private ImageLibrary(){}
	
	private static int getImageType(int channelCount)
	{
		int type;
		
		switch(channelCount)
		{
		case 1:
			type = BufferedImage.TYPE_BYTE_GRAY;
			break;
			
		default:
		case 3:
			type = BufferedImage.TYPE_3BYTE_BGR;
			break;
			
		case 4:
			type = BufferedImage.TYPE_4BYTE_ABGR;
			break;
		}
		
		return type;
	}
	
	public static BufferedImage getBufferedImage(IImageData imageData, BufferedImage bufferedImage)
	{
		ImageInfo imageInfo;
		int type;
		
		imageInfo = imageData.getImageInfo();
		
		type = ImageLibrary.getImageType(imageInfo.channelCount);
		
		if(bufferedImage != null)
		{
			if(bufferedImage.getWidth() != imageInfo.width
			|| bufferedImage.getHeight() != imageInfo.height
			|| bufferedImage.getType() != type)
			{
				bufferedImage = new BufferedImage(imageInfo.width, imageInfo.height, type);
			}
		}
		else
		{
			bufferedImage = new BufferedImage(imageInfo.width, imageInfo.height, type);
		}
		
		{
			DataBufferByte buffer;
			byte[] pixels;
			
			buffer = (DataBufferByte)bufferedImage.getRaster().getDataBuffer();
			pixels = buffer.getData();
			
			ImageLibrary.getDataBufferJni(pixels, imageData);
		}
		
		return bufferedImage;
	}
	
	private native static boolean getDataBufferJni(byte[] pixels, IImageData imageData);
}
