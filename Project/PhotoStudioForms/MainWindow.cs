using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PhotoStudioForms
{
	public partial class MainWindow : Form
	{
		private PsImage.IImageData _ImageData;
		private PsImage.IImageData _ProcImage;

		private System.Drawing.Bitmap _ViewImage;

		public MainWindow()
		{
			InitializeComponent();

			this._ImageData = new PsImage.ImageData();
			this._ProcImage = new PsImage.ImageData();
		}

		private void OnLoad(object sender, EventArgs e)
		{
		}

		private void OnPaint(object sender, PaintEventArgs e)
		{
			this.UpdateView(e.Graphics);
		}

		private void OnResize(object sender, EventArgs e)
		{
			this.UpdateView();
		}

		private void UpdateView()
		{
			System.Drawing.Graphics Graphics = this.CreateGraphics();

			this.UpdateView(Graphics);

			Graphics.Dispose();
		}

		private void UpdateView(System.Drawing.Graphics Graphics)
		{
			System.Drawing.Size ClientSize = this.ClientSize;

			// Background
			Graphics.FillRectangle(System.Drawing.Brushes.Black, 0, 0, ClientSize.Width, ClientSize.Height);

			if(this._ViewImage != null)
			{
				System.Drawing.Size ImageSize = this._ViewImage.Size;

				float ScaleH, ScaleV, Scale;

				ScaleH = (float)ClientSize.Width / ImageSize.Width;
				ScaleV = (float)ClientSize.Height / ImageSize.Height;
				Scale = System.Math.Min(ScaleH, ScaleV);

				System.Drawing.Size ViewImageSize = new System.Drawing.Size();

				ViewImageSize.Width = (Int32)(Scale * ImageSize.Width + 0.5f);
				ViewImageSize.Height = (Int32)(Scale * ImageSize.Height + 0.5f);

				System.Drawing.Rectangle SrcRect = new System.Drawing.Rectangle(0, 0, ImageSize.Width, ImageSize.Height);

				System.Drawing.Rectangle DstRect = new System.Drawing.Rectangle(
					(ClientSize.Width - ViewImageSize.Width) / 2,
					(ClientSize.Height - ViewImageSize.Height) / 2,
					ViewImageSize.Width, ViewImageSize.Height);

				Graphics.DrawImage(this._ViewImage, DstRect, SrcRect, GraphicsUnit.Pixel);
			}
		}

		private void UpdateViewImage(PsImage.IImageData ImageData)
		{
			if(this._ViewImage != null)
			{
				this._ViewImage.Dispose();
				this._ViewImage = null;
			}

			if(ImageData != null)
			{
				PsImage.ImageLibrary.Convert(ref this._ViewImage, ImageData);
			}
		}

		private void OnFileMenuOpening(object sender, EventArgs e)
		{
			this.MenuFileClose.Enabled = this._ImageData.IsCreated;
			this.MenuFileSave.Enabled = this._ImageData.IsCreated;
		}

		private void OnFileNew(object sender, EventArgs e)
		{
			// TODO
		}

		private void OnFileOpen(object sender, EventArgs e)
		{
			System.Windows.Forms.OpenFileDialog dlg = new System.Windows.Forms.OpenFileDialog();

			dlg.Filter = "All files (*.*)|*.*";

			if(dlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				this._ProcImage.Destroy();

				PsImage.ImageReader.ReadImage(dlg.FileName, this._ImageData);

				this.UpdateViewImage(this._ImageData);

				this.UpdateView();
			}

			dlg.Dispose();
		}

		private void OnFileClose(object sender, EventArgs e)
		{
			this._ImageData.Destroy();
			this._ProcImage.Destroy();

			this.UpdateViewImage(null);

			this.UpdateView();
		}

		private void OnFileSave(object sender, EventArgs e)
		{
			System.Windows.Forms.SaveFileDialog dlg = new System.Windows.Forms.SaveFileDialog();

			dlg.Filter = "All files (*.*)|*.*";

			if(dlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				PsImage.IImageData ImageData = this._ProcImage.IsCreated ? this._ProcImage : this._ImageData;

				PsImage.ImageWriter.WriteImage(dlg.FileName, ImageData);
			}

			dlg.Dispose();
		}

		private void OnImageMenuOpening(object sender, EventArgs e)
		{
			this.MenuImageGray.Enabled = this._ImageData.IsCreated;

		}

		private void OnImageGray(object sender, EventArgs e)
		{
			// TODO
		}

	}
}
