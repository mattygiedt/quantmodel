using System;
using System.ComponentModel;
using System.Security.Cryptography;
using System.Text;
using System.Windows.Forms;

namespace com.quantmodel.common.controls
{
	public class LoginControl : System.Windows.Forms.UserControl
	{
		public event System.EventHandler Successful;
		public event System.EventHandler Failed;
		public event System.EventHandler Ready;

		private bool bCheck = true;
		private bool bHash= true;
		private string theUsername;
		private string thePassword;
		private string tmpU;
		private string tmpP;
		private string ErrMsg = "Please enter the correct Username / Password combination!";
		private System.Windows.Forms.Button bLogin;
		private System.Windows.Forms.Label lPassword;
		private System.Windows.Forms.Label lUsername;
		private System.Windows.Forms.TextBox tbPassword;
		private System.Windows.Forms.Label lHeader;
		private System.Windows.Forms.Label lText;
		private System.Windows.Forms.PictureBox pbImage;
		private System.Windows.Forms.TextBox tbUsername;
		private System.Windows.Forms.Panel pHeader;
		private System.Windows.Forms.ErrorProvider errorProvider;

		private System.ComponentModel.Container components = null;

		public LoginControl()
		{
			InitializeComponent();
		}

		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		private void InitializeComponent()
		{
			System.Resources.ResourceManager resources =
			    new System.Resources.ResourceManager(typeof(LoginControl));
			this.bLogin = new System.Windows.Forms.Button();
			this.lPassword = new System.Windows.Forms.Label();
			this.lUsername = new System.Windows.Forms.Label();
			this.tbPassword = new System.Windows.Forms.TextBox();
			this.pHeader = new System.Windows.Forms.Panel();
			this.lHeader = new System.Windows.Forms.Label();
			this.lText = new System.Windows.Forms.Label();
			this.pbImage = new System.Windows.Forms.PictureBox();
			this.tbUsername = new System.Windows.Forms.TextBox();
			this.errorProvider = new System.Windows.Forms.ErrorProvider();
			this.pHeader.SuspendLayout();
			this.SuspendLayout();
			//
			// bLogin
			//
			this.bLogin.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
			this.bLogin.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.bLogin.Location = new System.Drawing.Point(330, 245);
			this.bLogin.Name = "bLogin";
			this.bLogin.Size = new System.Drawing.Size(70, 20);
			this.bLogin.TabIndex = 9;
			this.bLogin.Text = "Login";
			this.bLogin.Click += new System.EventHandler(this.OnClickLogin);
			//
			// lPassword
			//
			this.lPassword.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.lPassword.Location = new System.Drawing.Point(30, 205);
			this.lPassword.Name = "lPassword";
			this.lPassword.Size = new System.Drawing.Size(80, 20);
			this.lPassword.TabIndex = 11;
			this.lPassword.Text = "Password:";
			this.lPassword.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			//
			// lUsername
			//
			this.lUsername.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.lUsername.Location = new System.Drawing.Point(30, 165);
			this.lUsername.Name = "lUsername";
			this.lUsername.Size = new System.Drawing.Size(80, 20);
			this.lUsername.TabIndex = 10;
			this.lUsername.Text = "Username:";
			this.lUsername.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			//
			// tbPassword
			//
			this.tbPassword.MaxLength = 16;
			this.tbPassword.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tbPassword.Location = new System.Drawing.Point(130, 205);
			this.tbPassword.Name = "tbPassword";
			this.tbPassword.Size = new System.Drawing.Size(270, 20);
			this.tbPassword.TabIndex = 7;
			this.tbPassword.Text = "";
			//
			// pHeader
			//
			//this.pHeader.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.pHeader.BackColor = System.Drawing.SystemColors.Window;
			this.pHeader.Controls.AddRange(new System.Windows.Forms.Control[] {
																				  this.lHeader,
																				  this.lText,
																				  this.pbImage});
			this.pHeader.Dock = System.Windows.Forms.DockStyle.Top;
			this.pHeader.Name = "pHeader";
			this.pHeader.Size = new System.Drawing.Size(460, 60);
			this.pHeader.TabIndex = 8;
			//
			// lHeader
			//
			this.lHeader.Font = new System.Drawing.Font("Tahoma", 10.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.lHeader.Location = new System.Drawing.Point(10, 10);
			this.lHeader.Name = "lHeader";
			this.lHeader.Size = new System.Drawing.Size(360, 20);
			this.lHeader.TabIndex = 3;
			this.lHeader.Text = "Login";
			this.lHeader.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			//
			// lText
			//
			this.lText.Location = new System.Drawing.Point(20, 30);
			this.lText.Name = "lText";
			this.lText.Size = new System.Drawing.Size(360, 20);
			this.lText.TabIndex = 4;
			this.lText.Text = "Please enter your Username and Password.";
			this.lText.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			//
			// pbImage
			//
			this.pbImage.Image = ((System.Drawing.Bitmap)(resources.GetObject("pbImage.Image")));
			this.pbImage.Location = new System.Drawing.Point(388, 5);
			this.pbImage.Name = "pbImage";
			this.pbImage.Size = new System.Drawing.Size(48, 48);
			this.pbImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.pbImage.TabIndex = 5;
			this.pbImage.TabStop = false;
			//
			// tbUsername
			//
			this.tbUsername.MaxLength = 16;
			this.tbUsername.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tbUsername.Location = new System.Drawing.Point(130, 165);
			this.tbUsername.Name = "tbUsername";
			this.tbUsername.Size = new System.Drawing.Size(270, 20);
			this.tbUsername.TabIndex = 6;
			this.tbUsername.Text = "";
			//
			// LoginControl
			//
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.bLogin,
																		  this.lPassword,
																		  this.lUsername,
																		  this.tbPassword,
																		  this.pHeader,
																		  this.tbUsername});
			this.Name = "LoginControl";
			this.Size = new System.Drawing.Size(460, 300);
			this.Load += new System.EventHandler(this.OnLoad);
			this.pHeader.ResumeLayout(false);
			this.ResumeLayout(false);
	    }

		static void Main() { }

		private void OnLoad(object sender, System.EventArgs e)
		{
			tbPassword.PasswordChar = '\u25CF';
			if (bCheck)
			{
				tmpU = theUsername;
				tmpP = thePassword;
			}
		}

		private void OnClickLogin(object sender, System.EventArgs e)
		{
			if (!bCheck)
			{
				if (!bHash)
				{
					theUsername = tbUsername.Text;
					thePassword = tbPassword.Text;
					if (Ready != null)
					{
						Ready(this, new System.EventArgs());
					}
				}
				else
				{
					theUsername = HashString(tbUsername.Text);
					thePassword = HashString(tbPassword.Text);
					if (Ready != null)
					{
						Ready(this, new System.EventArgs());
					}
				}
			}
			else
			{
				bool b = CheckCredentials(tmpU, tmpP);
				if (b)
				{
		 		    if (Ready != null)
    				{
    				    Successful(this, new System.EventArgs());
    				}
				}
				else
				{
					if (Ready != null)
					{
						Failed(this, new System.EventArgs());
					}
				}
			}
		}

		public string labelHeadline
		{
			get
			{
				return lHeader.Text;
			}
			set
			{
				lHeader.Text = value;
			}
		}

		public string labelHeader
		{
			get
			{
				return lText.Text;
			}
			set
			{
				lText.Text = value;
			}
		}

		public string labelUsername
		{
			get
			{
				return lUsername.Text;
			}
			set
			{
				lUsername.Text = value;
			}
		}

		public string labelPassword
		{
			get
			{
				return lPassword.Text;
			}
			set
			{
				lPassword.Text = value;
			}
		}

		public string labelButton
		{
			get
			{
				return bLogin.Text;
			}
			set
			{
				bLogin.Text = value;
			}
		}

		public string Username
		{
			get
			{
				return theUsername;
			}
			set
			{
				theUsername = value;
			}
		}

		public string Password
		{
			get
			{
				return thePassword;
			}
			set
			{
				thePassword = value;
			}
		}

		public bool Check
		{
			get
			{
				return bCheck;
			}
			set
			{
				bCheck = value;
			}
		}

		public bool Hash
		{
			get
			{
				return bHash;
			}
			set
			{
				bHash= value;
			}
		}

		/// <summary>
		/// If NoCheck is set to false this method checks Username and Password.
		/// </summary>
		private bool CheckCredentials(string usr, string pwd)
		{
			if (!bHash)
			{
				if (tbUsername.Text == usr)
				{
					if (tbPassword.Text == pwd)
					{
						return true;
					}
					else
					{
						errorProvider.SetError(bLogin, ErrMsg);
						return false;
					}
				}
				else
				{
					errorProvider.SetError(bLogin, ErrMsg);
					return false;
				}
			}
			else
			{
				if (HashString(tbUsername.Text) == HashString(usr))
				{
					if (HashString(tbPassword.Text) == HashString(pwd))
					{
						return true;
					}
					else
					{
						errorProvider.SetError(bLogin, ErrMsg);
						return false;
					}
				}
				else
				{
					errorProvider.SetError(bLogin, ErrMsg);
					return false;
				}
			}
		}

		private string HashString( string s )
		{
			UTF8Encoding UTF8 = new UTF8Encoding();
			byte[] data = UTF8.GetBytes(s);
			MD5 md5 = new MD5CryptoServiceProvider();
			byte[] result = md5.ComputeHash(data);
			return System.Text.Encoding.UTF8.GetString(result);
		}

		public void Reset()
		{
		    tbPassword.Text = "";
		    tbUsername.Text = "";
		}

		public string ErrorMessage
		{
			get
			{
				return ErrMsg;
			}
			set
			{
				ErrMsg = value;
			}
		}
	}
}
