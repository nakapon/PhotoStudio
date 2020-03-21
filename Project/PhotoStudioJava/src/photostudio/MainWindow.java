package photostudio;

import java.awt.event.ActionEvent;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.net.URL;
import java.util.EventObject;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;

public class MainWindow
{
	private JFrame _mainFrame;
	
	private JMenuBar _menuBar;

	public MainWindow()
	{
		this._mainFrame = new JFrame();
		
		this._mainFrame.setTitle("Photo Studio");
		this._mainFrame.setBounds(100, 100, 1200, 800);
		this._mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
//		this._mainFrame.setExtendedState(JFrame.MAXIMIZED_BOTH);
		
		this._menuBar = new JMenuBar();
		this._mainFrame.setJMenuBar(this._menuBar);
		
		this.createFileMenu();
		this.createImageMenu();
		
		this._mainFrame.setVisible(true);
		
		// icon
/*
		{
			ImageIcon icon = null;
			
			URL url = this.getClass().getClassLoader().getResource("Symbol.png");
			if(url == null)
			{
				icon = new ImageIcon("./res/Symbol.png");
			}
			else
			{
				icon = new ImageIcon(url);
			}
			
			this._mainFrame.setIconImage(icon.getImage());
		}
*/
		this._mainFrame.addWindowListener(new WindowAdapter()
		{
			@Override
			public void windowOpened(WindowEvent e)
			{
			}
			
			@Override
			public void windowClosing(WindowEvent e)
			{
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
			JOptionPane.showMessageDialog(this._mainFrame, "Open");
		});
		
		Menu.addSeparator();
		
		addMenuItem(Menu, false, false, "Close", 'C', ()->
		{
			JOptionPane.showMessageDialog(this._mainFrame, "Close");
		});
		
		Menu.addSeparator();
		
		addMenuItem(Menu, false, true, "Save", 'S', ()->
		{
			JOptionPane.showMessageDialog(this._mainFrame, "Save");
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
