package photostudio;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.EventObject;
import java.util.List;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.TransferHandler;
import javax.swing.filechooser.FileNameExtensionFilter;

import photostudio.image.IImageData;
import photostudio.image.ImageFactory;
import photostudio.image.ImageReader;
import photostudio.image.ImageWriter;

public class MainWindow
{
	private JFrame _mainFrame;
	
	private JMenuBar _menuBar;
	
	private ImageView _imageView;
	
	private IImageData _imageData;
	private IImageData _procImage;

	public MainWindow()
	{
		// main frame
		this._mainFrame = new JFrame();
		
		this._mainFrame.setTitle("Photo Studio");
		this._mainFrame.setBounds(100, 100, 1200, 800);
		this._mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
//		this._mainFrame.setExtendedState(JFrame.MAXIMIZED_BOTH);
		
		// menu
		this._menuBar = new JMenuBar();
		this._mainFrame.setJMenuBar(this._menuBar);
		
		this.createFileMenu();
		this.createImageMenu();
		
		this._imageData = ImageFactory.createInstance();
		this._procImage = ImageFactory.createInstance();
		
		// icon
		{
			ImageIcon icon = null;
			
			URL url = this.getClass().getClassLoader().getResource("PhotoStudioJava.png");
			if(url == null)
			{
				icon = new ImageIcon("./res/PhotoStudioJava.png");
			}
			else
			{
				icon = new ImageIcon(url);
			}
			
			this._mainFrame.setIconImage(icon.getImage());
		}
		
		// image view
		this._imageView = new ImageView();
		this._mainFrame.getContentPane().add(this._imageView);
		
		this._mainFrame.addWindowListener(new WindowAdapter()
		{
			@Override
			public void windowOpened(WindowEvent e)
			{
			}
			
			@Override
			public void windowClosing(WindowEvent e)
			{
				ImageFactory.destroyInstance(_imageData);
				ImageFactory.destroyInstance(_procImage);
			}
			
			@Override
			public void windowClosed(WindowEvent e)
			{
				// this event is not called.
			}
		});
		
		this._mainFrame.addComponentListener(new ComponentListener()
		{
			@Override
			public void componentShown(ComponentEvent e)
			{
			}
			
			@Override
			public void componentResized(ComponentEvent e)
			{
			}
			
			@Override
			public void componentMoved(ComponentEvent e)
			{
			}
			
			@Override
			public void componentHidden(ComponentEvent e)
			{
			}
		});
		
		this._mainFrame.setTransferHandler(new DropFileHandler());
		
		this._mainFrame.setVisible(true);
	}
	
	private class DropFileHandler extends TransferHandler
	{
		private static final long serialVersionUID = -3210868112520978664L;

		@Override
		public boolean canImport(TransferSupport support)
		{
			if(!support.isDrop()){
				// in the case of clipboard
				return false;
			}
			
			if(!support.isDataFlavorSupported(DataFlavor.javaFileListFlavor))
				return false;
			
			return true;
		}
		
		@Override
		public boolean importData(TransferSupport support)
		{
			if(!canImport(support))
				return false;

			Transferable transferable = support.getTransferable();
			try
			{
				@SuppressWarnings("unchecked")
				List<File> files = (List<File>)transferable.getTransferData(DataFlavor.javaFileListFlavor);
				
				if(ImageReader.read(files.get(0).getAbsolutePath(), _imageData))
				{
					_imageView.setImage(_imageData);
					_imageView.repaint();
				}
			}
			catch(UnsupportedFlavorException | IOException e)
			{
				e.printStackTrace();
			}
			return true;
		}
	}
	
	private JMenuItem addMenuItem(JMenu parent, boolean isCheckable, boolean isModal, String name, char mnemonic, Runnable runner)
	{
		JMenuItem item;
		
		Action act;
		String actName;
		
		if(isModal)
		{
			actName = name + "...";
		}
		else
		{
			actName = name;
		}
		
		act = new AbstractAction(actName)
		{
			private static final long serialVersionUID = 5620410453256189860L;

			@Override
			public void actionPerformed(ActionEvent e)
			{
				runner.run();
			}
		};
		
		if(isCheckable)
		{
			item = new JCheckBoxMenuItem(act);
		}
		else
		{
			item = new JMenuItem(act);
		}
		
		if(mnemonic != ' ')
		{
			item.setMnemonic(mnemonic);
		}
		
		parent.add(item);
		
		return item;
	}
	
	private void createFileMenu()
	{
		JMenu Menu = new JMenu("File");
		Menu.setMnemonic('F');
		
		addMenuItem(Menu, false, true, "New", 'N', ()->
		{
			JOptionPane.showMessageDialog(this._mainFrame, "New");
		});
		
		addMenuItem(Menu, false, true, "Open", 'O', ()->
		{
			JFileChooser fileChooser = new JFileChooser();
			
			int result = fileChooser.showOpenDialog(null);
			if(result == JFileChooser.APPROVE_OPTION)
			{
				File file = fileChooser.getSelectedFile();
				
				if(ImageReader.read(file.getAbsolutePath(), this._imageData))
				{
					_imageView.setImage(this._imageData);
					_imageView.repaint();
				}
			}
		});
		
		Menu.addSeparator();
		
		addMenuItem(Menu, false, false, "Close", 'C', ()->
		{
			JOptionPane.showMessageDialog(this._mainFrame, "Close");
		});
		
		Menu.addSeparator();
		
		addMenuItem(Menu, false, true, "Save", 'S', ()->
		{
			JFileChooser fileChooser = new JFileChooser();
			
			int result = fileChooser.showSaveDialog(null);
			if(result == JFileChooser.APPROVE_OPTION)
			{
				File file = fileChooser.getSelectedFile();
				
				ImageWriter.write(file.getAbsolutePath(), this._imageData);
			}
		});
		
		this._menuBar.add(Menu);
	}

	private void createImageMenu()
	{
		JMenu Menu = new JMenu("Image");
		Menu.setMnemonic('I');
		
		addMenuItem(Menu, false, true, "Monochrome", 'M', ()->
		{
			JOptionPane.showMessageDialog(this._mainFrame, "Monochrome");
		});
		
		this._menuBar.add(Menu);
	}
	
}
