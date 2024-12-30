package application;

import login.frame.LoginFrame;
import util.ThemeUtil;

public class Application {

	public static void main(String[] args) {
		
		//ThemeUtil.setTheme();
		
		LoginFrame.instance.setVisible(true);
		
	}
	
}
