using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

using System.IO;
using System.ComponentModel;

namespace tpg {
	static class Program {
		[STAThread]
		static void Main() {
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new TPG());
		}
	}

	public partial class TPG {
		internal TPG() {
			InitializeComponent();
		}

		private string Filestream;
		private List<List<Color>> ColorTables = new List<List<Color>>();


		private void Button1_Click(object sender, EventArgs e) {
			OpenFileDialog ofd = new OpenFileDialog();
			if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
				ColorTables = new List<List<Color>>();
				ColorTableView.Controls.Clear();

				FilePathField.Text = ofd.FileName;
				Filestream = PullDataFromFile(ofd.FileName);
				ReadColorTable(Filestream);
				ReadTpage(Filestream);
			}
		}

		private object ReadColorTable(string Stream) {
			var tempVar = 8;
			int CTLength = Read(ref Stream, ref tempVar, 8);

			int IndexPos = 12;

			int TableCount = CTLength / 3;
			int TextureCount = TableCount / 16;
			for (var i = 0; i < TextureCount; i++) {
				List<Color> CTable = new List<Color>();
				for (var h = 0; h <= TextureCount * 3 - 3; h += 3) {
					int j = (i) * 48;
					Color CColor = new Color();
					var tempVar2 = IndexPos + j + h;
					var tempVar3 = IndexPos + j + h + 1;
					var tempVar4 = IndexPos + j + h + 2;
					CColor = Color.FromArgb(Read(ref Stream, ref tempVar2, 1), Read(ref Stream, ref tempVar3, 1), Read(ref Stream, ref tempVar4, 1));
					CTable.Add(CColor);

					ColorTableView.Controls.Add(new Panel {
						BackColor = CColor,
						BorderStyle = BorderStyle.FixedSingle,
						Location = new Point(16 * (h / 3), 16 * i),
						Size = new Size(14, 14)
					});
					Application.DoEvents();
				}
				ColorTables.Add(CTable);
			}
			return null;
		}

		private object ReadTpage(string Stream) {
			var tempVar = 8;
			int offset = Read(ref Stream, ref tempVar, 8) + 12;
			//each byte stores 2 pixels, thus one line of 64px is actually 128px so 64px=128px = half the vertical line
			//so 2 64px chunks Is one entire vertical line
			//getting the upper and lowe parts of the bytes
			//FF = 11111111 == (F/1111) & (F/1111)

			//so we can do a dumb calc to get a rough idea
			//64px is one line, but we worry about all that in a secondary loop
			//we need the amount of textures

			//first and foremost, were only gonna assume its always a 4x4 texture grid
			//meaning one texture is 32x32 = 64x64px

			Bitmap bigassbitmap = new Bitmap(2000, 2000);
			for (var row = 0; row <= 3; row++) { //the current texture row we are on 0-3
				//each texture row is 64px down
				//but first we need to know what colum of texture we are in
				for (var column = 0; column <= 3; column++) {
					//ok, so row0 col0 is the top left square etc...
					//now we know what sqaure we are in, we need to convert that to a pointer in the raw file

					//find out what VERTICAL square we are in, hence the colum, then multiply by 32 (thus 4*32 = a full row)
					//as this is how the data is stored in the file, vertical lines of 128px total length
					//then we multiply that by the row number, except not entirely
					//we actually need to do 128*row as each row is worth 128
					for (var pixelrow = 0; pixelrow <= 63; pixelrow++) { //for each row of 32px in this square
						for (var pixel = 0; pixel <= 62; pixel += 2) { //for each pixel on this line
							//had to make it 63 in here so that the points can be set properly
							//Do pixel /2 you'll get over it
							int X = pixel + (column * 64);
							int y = pixelrow + (row * 64);

							int c1 = column * 64;
							int c2 = c1 + pixel / 2;

							int r1 = row * 64;
							int r2 = r1 + (pixelrow * 128);

							int f0 = (c1 + c2 + r1 + r2 + (column * 32) + (row * (128 * 64))) - ((column + row) * 128);

							string PixelData = Stream.Substring((offset + (f0)) * 2, 2);
							Int16[] UncompressedPoints = (Int16[])ExpandPointData(PixelData);

							bigassbitmap.SetPixel(X, y, ColorTables[(column) + (row * 4)][UncompressedPoints[1]]);
							bigassbitmap.SetPixel(X + 1, y, ColorTables[(column) + (row * 4)][UncompressedPoints[0]]);
							TPageView.Image = bigassbitmap;
						}
					}
					TPageView.Image = bigassbitmap;
				}
			}
			TPageView.Image = bigassbitmap;
			Application.DoEvents();
			return null;
		}

		public object ExpandPointData(string Point) {
			byte b1 = Convert.ToByte(("0x" + Point), 16);
			byte highNibble = (byte)(b1 >> 4 & 0xF);
			byte lowNibble = (byte)(b1 & 0xF);

			short[] TPGD = new short[3];
			TPGD[0] = highNibble;
			TPGD[1] = lowNibble;
			return TPGD;
		}

		public static string PullDataFromFile(string Stream) {
			return string.Join("", Array.ConvertAll(File.ReadAllBytes(Stream), (b) => b.ToString("X2")));
		}

		public static byte[] fread(ref string Stream, ref int StartIndexPtr, int Length) {
			if (StartIndexPtr + Length > Stream.Length) {
				return HexToBytes("00");
			}
			byte[] A = HexToBytes(Stream.Substring(StartIndexPtr * 2, Length * 2));
			Array.Resize(ref A, ((Length < 4) ? 4 : Length));
			StartIndexPtr += Length;
			return A;
		}

		public static int Read(ref string Stream, ref int StartIndexPtr, int Length) {
			Int32 a = BitConverter.ToInt32(fread(ref Stream, ref StartIndexPtr, Length), 0);
			return a;
		}

		public static byte[] HexToBytes(string HexString) {
			int length = HexString.Length;
			int upperBound = length / 2;
			if (length % 2 == 0) {
				upperBound -= 1;
			} else {
				HexString = "0" + HexString;
			}
			byte[] bytes = new byte[upperBound + 1];
			for (var i = 0; i <= upperBound; i++) {
				bytes[i] = Convert.ToByte(HexString.Substring(i * 2, 2), 16);
			}
			return bytes;
		}

		private void MenuItem1_Click(object sender, EventArgs e) {
			Clipboard.SetImage(TPageView.Image);
		}

		private void contextMenuStrip1_Opening(object sender, CancelEventArgs e) {
			if(ColorTables.Count < 1) {
				e.Cancel = true;
			}
		}


	}




	public partial class TPG : System.Windows.Forms.Form {
		//Form overrides dispose to clean up the component list.
		[System.Diagnostics.DebuggerNonUserCode()]
		protected override void Dispose(bool disposing) {
			try {
				if (disposing && components != null) {
					components.Dispose();
				}
			} finally {
				base.Dispose(disposing);
			}
		}

		//Required by the Windows Form Designer
		private System.ComponentModel.IContainer components;

		//NOTE: The following procedure is required by the Windows Form Designer
		//It can be modified using the Windows Form Designer.  
		//Do not modify it using the code editor.
		[System.Diagnostics.DebuggerStepThrough()]
		private void InitializeComponent() {
			this.components = new System.ComponentModel.Container();
			this.FilePathField = new System.Windows.Forms.TextBox();
			this.BuBrowse = new System.Windows.Forms.Button();
			this.ColorTableView = new System.Windows.Forms.Panel();
			this.TPageView = new System.Windows.Forms.PictureBox();
			this.Label1 = new System.Windows.Forms.Label();
			this.Label2 = new System.Windows.Forms.Label();
			this.ContextMenu1 = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.MenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			((System.ComponentModel.ISupportInitialize)(this.TPageView)).BeginInit();
			this.SuspendLayout();
			// 
			// FilePathField
			// 
			this.FilePathField.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			this.FilePathField.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.FilePathField.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(240)))), ((int)(((byte)(240)))));
			this.FilePathField.Location = new System.Drawing.Point(12, 12);
			this.FilePathField.Name = "FilePathField";
			this.FilePathField.Size = new System.Drawing.Size(338, 20);
			this.FilePathField.TabIndex = 0;
			// 
			// BuBrowse
			// 
			this.BuBrowse.Location = new System.Drawing.Point(356, 10);
			this.BuBrowse.Name = "BuBrowse";
			this.BuBrowse.Size = new System.Drawing.Size(75, 23);
			this.BuBrowse.TabIndex = 1;
			this.BuBrowse.Text = "Browse...";
			this.BuBrowse.UseVisualStyleBackColor = true;
			this.BuBrowse.Click += new System.EventHandler(this.Button1_Click);
			// 
			// ColorTableView
			// 
			this.ColorTableView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
			| System.Windows.Forms.AnchorStyles.Left)));
			this.ColorTableView.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.ColorTableView.Location = new System.Drawing.Point(12, 54);
			this.ColorTableView.Name = "ColorTableView";
			this.ColorTableView.Size = new System.Drawing.Size(419, 304);
			this.ColorTableView.TabIndex = 2;
			// 
			// TPageView
			// 
			this.TPageView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
			| System.Windows.Forms.AnchorStyles.Left)
			| System.Windows.Forms.AnchorStyles.Right)));
			this.TPageView.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.TPageView.Location = new System.Drawing.Point(438, 54);
			this.TPageView.Name = "TPageView";
			this.TPageView.Size = new System.Drawing.Size(378, 304);
			this.TPageView.TabIndex = 3;
			this.TPageView.TabStop = false;
			this.ContextMenuStrip = ContextMenu1;
			// 
			// Label1
			// 
			this.Label1.AutoSize = true;
			this.Label1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(240)))), ((int)(((byte)(240)))));
			this.Label1.Location = new System.Drawing.Point(12, 38);
			this.Label1.Name = "Label1";
			this.Label1.Size = new System.Drawing.Size(61, 13);
			this.Label1.TabIndex = 4;
			this.Label1.Text = "Color Table";
			// 
			// Label2
			// 
			this.Label2.AutoSize = true;
			this.Label2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(240)))), ((int)(((byte)(240)))));
			this.Label2.Location = new System.Drawing.Point(435, 38);
			this.Label2.Name = "Label2";
			this.Label2.Size = new System.Drawing.Size(48, 13);
			this.Label2.TabIndex = 5;
			this.Label2.Text = "Textures";
			// 
			// ContextMenu1
			// 
			this.ContextMenu1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
			this.MenuItem1});
			this.ContextMenu1.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStrip1_Opening);
			// 
			// MenuItem1
			// 
			this.MenuItem1.Name = "copyToolStripMenuItem";
			this.MenuItem1.Size = new System.Drawing.Size(180, 22);
			this.MenuItem1.Text = "Copy";
			this.MenuItem1.Click += new System.EventHandler(this.MenuItem1_Click);
			// 
			// TPG
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
			this.ClientSize = new System.Drawing.Size(823, 367);
			this.Controls.Add(this.Label2);
			this.Controls.Add(this.Label1);
			this.Controls.Add(this.TPageView);
			this.Controls.Add(this.ColorTableView);
			this.Controls.Add(this.BuBrowse);
			this.Controls.Add(this.FilePathField);
			this.MinimumSize = new System.Drawing.Size(839, 406);
			this.Name = "TPG";
			this.ShowIcon = false;
			this.Text = "Raw Bin Extractor";
			((System.ComponentModel.ISupportInitialize)(this.TPageView)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		internal TextBox FilePathField;
		internal Button BuBrowse;
		internal Panel ColorTableView;
		internal PictureBox TPageView;
		internal Label Label1;
		internal Label Label2;
		internal ContextMenuStrip ContextMenu1;
		internal ToolStripMenuItem MenuItem1;
	}




}