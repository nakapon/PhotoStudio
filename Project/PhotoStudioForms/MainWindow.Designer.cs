namespace PhotoStudioForms
{
	partial class MainWindow
	{
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
		protected override void Dispose(bool disposing)
		{
			if(disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows フォーム デザイナーで生成されたコード

		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWindow));
			this.MainMenu = new System.Windows.Forms.MenuStrip();
			this.MenuFile = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuFileNew = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuFileOpen = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.MenuFileClose = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.MenuFileSave = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuImage = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuImageGray = new System.Windows.Forms.ToolStripMenuItem();
			this.MainMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// MainMenu
			// 
			this.MainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuFile,
            this.MenuImage});
			this.MainMenu.Location = new System.Drawing.Point(0, 0);
			this.MainMenu.Name = "MainMenu";
			this.MainMenu.Size = new System.Drawing.Size(1184, 24);
			this.MainMenu.TabIndex = 0;
			this.MainMenu.Text = "menuStrip1";
			// 
			// MenuFile
			// 
			this.MenuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuFileNew,
            this.MenuFileOpen,
            this.toolStripSeparator1,
            this.MenuFileClose,
            this.toolStripSeparator2,
            this.MenuFileSave});
			this.MenuFile.Name = "MenuFile";
			this.MenuFile.Size = new System.Drawing.Size(37, 20);
			this.MenuFile.Text = "&File";
			this.MenuFile.DropDownOpening += new System.EventHandler(this.OnFileMenuOpening);
			// 
			// MenuFileNew
			// 
			this.MenuFileNew.Name = "MenuFileNew";
			this.MenuFileNew.ShortcutKeyDisplayString = "";
			this.MenuFileNew.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
			this.MenuFileNew.Size = new System.Drawing.Size(154, 22);
			this.MenuFileNew.Text = "&New...";
			this.MenuFileNew.Click += new System.EventHandler(this.OnFileNew);
			// 
			// MenuFileOpen
			// 
			this.MenuFileOpen.Name = "MenuFileOpen";
			this.MenuFileOpen.ShortcutKeyDisplayString = "";
			this.MenuFileOpen.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
			this.MenuFileOpen.Size = new System.Drawing.Size(154, 22);
			this.MenuFileOpen.Text = "&Open...";
			this.MenuFileOpen.Click += new System.EventHandler(this.OnFileOpen);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(151, 6);
			// 
			// MenuFileClose
			// 
			this.MenuFileClose.Name = "MenuFileClose";
			this.MenuFileClose.ShortcutKeyDisplayString = "";
			this.MenuFileClose.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.W)));
			this.MenuFileClose.Size = new System.Drawing.Size(154, 22);
			this.MenuFileClose.Text = "&Close";
			this.MenuFileClose.Click += new System.EventHandler(this.OnFileClose);
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(151, 6);
			// 
			// MenuFileSave
			// 
			this.MenuFileSave.Name = "MenuFileSave";
			this.MenuFileSave.ShortcutKeyDisplayString = "";
			this.MenuFileSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
			this.MenuFileSave.Size = new System.Drawing.Size(154, 22);
			this.MenuFileSave.Text = "&Save...";
			this.MenuFileSave.Click += new System.EventHandler(this.OnFileSave);
			// 
			// MenuImage
			// 
			this.MenuImage.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuImageGray});
			this.MenuImage.Name = "MenuImage";
			this.MenuImage.Size = new System.Drawing.Size(51, 20);
			this.MenuImage.Text = "&Image";
			this.MenuImage.DropDownOpening += new System.EventHandler(this.OnImageMenuOpening);
			// 
			// MenuImageGray
			// 
			this.MenuImageGray.Name = "MenuImageGray";
			this.MenuImageGray.Size = new System.Drawing.Size(146, 22);
			this.MenuImageGray.Text = "&Monochrome";
			this.MenuImageGray.Click += new System.EventHandler(this.OnImageGray);
			// 
			// MainWindow
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1184, 761);
			this.Controls.Add(this.MainMenu);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.MainMenu;
			this.Name = "MainWindow";
			this.Text = "Photo Studio";
			this.Load += new System.EventHandler(this.OnLoad);
			this.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
			this.Resize += new System.EventHandler(this.OnResize);
			this.MainMenu.ResumeLayout(false);
			this.MainMenu.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip MainMenu;
		private System.Windows.Forms.ToolStripMenuItem MenuFile;
		private System.Windows.Forms.ToolStripMenuItem MenuImage;
		private System.Windows.Forms.ToolStripMenuItem MenuFileNew;
		private System.Windows.Forms.ToolStripMenuItem MenuFileOpen;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripMenuItem MenuFileClose;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripMenuItem MenuFileSave;
		private System.Windows.Forms.ToolStripMenuItem MenuImageGray;
	}
}

