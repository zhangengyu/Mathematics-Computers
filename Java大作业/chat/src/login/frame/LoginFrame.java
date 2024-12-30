package login.frame;

import login.panel.LoginPanel;
//import parent.frame.ParentFrame;

public class LoginFrame extends ParentFrame{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
//	public JPanel adminPage = new JPanel();
	
	public static LoginFrame instance = new LoginFrame();
	
	public LoginFrame() {
		super("问答机器人");
		LoginPanel.instance.setOpaque(false);
		this.add(LoginPanel.instance);
		
//		ImageIcon bg=new ImageIcon("C:/Users/55025/Desktop/login2.jpg");
//		JLabel label=new JLabel(bg);
//		JPanel jPanel = new JPanel();
//		label.setBounds(0, 0, acWidth(), acHeight());
//		jPanel.add(label);
//		getLayeredPane().add(jPanel, JLayeredPane.DEFAULT_LAYER);
	}

	@Override
	protected int acWidth() {
		// TODO Auto-generated method stub
		return 400;
	}

	@Override
	protected int acHeight() {
		// TODO Auto-generated method stub
		return 300;
	}

	@Override
	public void xianShi() {
		LoginPanel.instance.userNameInput.setText("");
		LoginPanel.instance.passwordInput.setText("");
		super.xianShi();
	}

}
