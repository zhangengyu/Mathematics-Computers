package login.panel;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPasswordField;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import chat.frame.ChatInterface;
import cn.hutool.core.util.StrUtil;
import login.frame.LoginFrame;

public class LoginPanel extends ParentPanel{

	private static final long serialVersionUID = 1L;

	public static String userId = "";
	
	public JLabel userName = new JLabel("用户名");
	
	public JTextField userNameInput = new JTextField();
    
	public JLabel password = new JLabel("密码");
	
	public JPasswordField passwordInput = new JPasswordField();
	
    public JButton loginButton = new JButton("登录");
    
    public JButton reset = new JButton("重置");
    
    public static LoginPanel instance = new LoginPanel();
    
    public LoginPanel() {
    	
    	super();
    	
    	this.setLayout(null);
    	this.add(userName);
    	this.add(userNameInput);
    	this.add(password);
    	this.add(passwordInput);
    	this.add(loginButton);
    	this.add(reset);
    	
    	userName.setBounds(50,40, 150, 30);
    	userNameInput.setBounds(100,40, 180, 30);
    	password.setBounds(50,80, 150, 30);
    	passwordInput.setBounds(100,80, 180, 30);
    	loginButton.setBounds(80,120, 70, 30);
    	reset.setBounds(180,120, 70, 30);
    	
    	loginButton.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				String userName = userNameInput.getText();
				String password = passwordInput.getText();
//				User dbUser = new UserDAO().queryByUserNameAndPassword(userName, password);
				if((StrUtil.equals(userName, "1") && StrUtil.equals(password, "1"))
			||(StrUtil.equals(userName, "2") && StrUtil.equals(password, "2"))
						||(StrUtil.equals(userName, "3") && StrUtil.equals(password, "3"))
						||(StrUtil.equals(userName, "4") && StrUtil.equals(password, "4"))
						||(StrUtil.equals(userName, "5") && StrUtil.equals(password, "5"))
						){
//					UserManager.instance.setUser(dbUser);
//					if(UserManager.instance.isAdmin()){
//						LoginFrame.instance.setVisible(false);
//						// AdminFrame.instance.xianShi();
//					}else{
//						LoginFrame.instance.setVisible(false);
//						// SearchFrame.instance.xianShi();
//					}
					userId = userName;
					LoginFrame.instance.setVisible(false);
//					AdminFrame.instance.xianShi();
//					ChatFrame.instance.xianShi();
					new ChatInterface().setVisible(true);
//					JOptionPane.showMessageDialog(LoginFrame.instance, "��¼�ɹ���");
				}else{
					JOptionPane.showMessageDialog(LoginFrame.instance, "用户名或密码错误！");
				}
			}
			
		});
    	
    	reset.addActionListener(new ActionListener() {
			
    			@Override
    			public void actionPerformed(ActionEvent e) {
    				userNameInput.setText("");
    				passwordInput.setText("");
    			}
    			
    		});
    	
	} 
    
//    public void paintComponent(Graphics g) {
//		super.paintComponent(g);
//		
//		ImageIcon icon=new ImageIcon(getClass().getResource("C:/Users/55025/Desktop/login.jpg"));
//		Image img=icon.getImage();
//		//����������Ϊ�˱���ͼƬ���Ը��洰�����е�����С�������Լ����óɹ̶���С
//		g.drawImage(img, 0, 0,this.getWidth(), this.getHeight(), this);
//	}
//    	
}
