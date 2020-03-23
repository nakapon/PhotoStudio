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

public class ImageView extends JPanel implements MouseInputListener, MouseWheelListener
{
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
			
			int viewWidth = (int)(1.0 * imageWidth);
			int viewHeight = (int)(1.0 * imageHeight);
			
			g.drawImage(this._image, 0, 0, viewWidth, viewHeight, 0, 0, imageWidth, imageHeight, this);
		}
	}
	
	@Override
	public void mouseClicked(MouseEvent arg0)
	{
	}

	@Override
	public void mouseEntered(MouseEvent arg0)
	{
	}

	@Override
	public void mouseExited(MouseEvent arg0)
	{
	}

	@Override
	public void mousePressed(MouseEvent arg0)
	{
	}

	@Override
	public void mouseReleased(MouseEvent arg0)
	{
	}

	@Override
	public void mouseDragged(MouseEvent arg0)
	{
	}

	@Override
	public void mouseMoved(MouseEvent arg0)
	{
	}

	@Override
	public void mouseWheelMoved(MouseWheelEvent arg0)
	{
	}
}
