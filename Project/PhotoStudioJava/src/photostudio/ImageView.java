package photostudio;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.awt.image.BufferedImage;

import javax.swing.JPanel;
import javax.swing.event.MouseInputListener;

import photostudio.image.IImageData;
import photostudio.image.ImageLibrary;

public class ImageView extends JPanel implements MouseInputListener, MouseWheelListener
{
	private static final long serialVersionUID = 1287664191053266421L;
	
	private Color _backColor = null;
	
	private BufferedImage _image = null;
	
	public ImageView()
	{
		super();
		
		this._backColor = new Color(0x00808080);
		
		this._image = null;
		
		this.addMouseListener(this);
		this.addMouseMotionListener(this);
		this.addMouseWheelListener(this);
	}
	
	public void setImage(IImageData imageData)
	{
		this._image = ImageLibrary.getBufferedImage(imageData, this._image);
	}
	
	@Override
	protected void paintComponent(Graphics g)
	{
		Graphics2D g2d = (Graphics2D)g;
		
		int canvasWidth = getWidth();
		int canvasHeight = getHeight();
		
		// Background
		g.setColor(_backColor);
		g.fillRect(0, 0, canvasWidth, canvasHeight);
		
		// Image
		if(this._image != null)
		{
			int imageWidth = this._image.getWidth();
			int imageHeight = this._image.getHeight();
			
			float scaleH, scaleV, scale;
			
			scaleH = (float)canvasWidth / imageWidth;
			scaleV = (float)canvasHeight / imageHeight;
			scale = Math.min(scaleH, scaleV);
			
			int viewWidth  = (int)(scale * imageWidth + 0.5f);
			int viewHeight = (int)(scale * imageHeight + 0.5f);
			
			g.drawImage(this._image,
						(canvasWidth - viewWidth) / 2, (canvasHeight - viewHeight) / 2,
						(canvasWidth + viewWidth) / 2, (canvasHeight + viewHeight) / 2,
						0, 0, imageWidth, imageHeight, this);
		}
	}
	
	@Override
	public void mouseClicked(MouseEvent e)
	{
	}

	@Override
	public void mouseEntered(MouseEvent e)
	{
	}

	@Override
	public void mouseExited(MouseEvent e)
	{
	}

	@Override
	public void mousePressed(MouseEvent e)
	{
	}

	@Override
	public void mouseReleased(MouseEvent e)
	{
	}

	@Override
	public void mouseDragged(MouseEvent e)
	{
	}

	@Override
	public void mouseMoved(MouseEvent e)
	{
	}

	@Override
	public void mouseWheelMoved(MouseWheelEvent e)
	{
	}
}
