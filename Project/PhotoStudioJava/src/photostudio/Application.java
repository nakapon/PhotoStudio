package photostudio;

import java.awt.EventQueue;

import javax.swing.UIManager;

public class Application
{
	@SuppressWarnings("unused")
	private MainWindow _mainWindow;
	
	public static void main(String[] args)
	{
		try
		{
			Application app = new Application();
			app.run(args);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public void run(String[] args)
	{
		EventQueue.invokeLater(new Runnable()
		{
			public void run()
			{
				try
				{
//					UIManager.setLookAndFeel("javax.swing.plaf.nimbus.NimbusLookAndFeel");
					UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
					
					_mainWindow = new MainWindow();
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}
			}
		});
	}
}
