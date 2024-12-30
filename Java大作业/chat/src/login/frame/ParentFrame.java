package login.frame;

import javax.swing.*;
import java.awt.*;

public class ParentFrame extends JFrame{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public ParentFrame() {
		super();
		init();
	}
	public ParentFrame(String name) {
		super(name);
		init();
	}
	protected void init() {
//        this.setSize(400, 300);
        Toolkit kit = Toolkit.getDefaultToolkit();
        Dimension screenSize = kit.getScreenSize();
        int screenWidth = screenSize.width;
        int screenHeight = screenSize.height;
        setSize(acWidth(), acHeight());
        // ����������λ��
        this.setLocation(screenWidth/2-acWidth()/2, screenHeight/2-acHeight()/2);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	protected int acWidth(){
		return 400;
	}
	protected int acHeight(){
		return 300;
	}
	public void xianShi(){
		this.setVisible(true);
	}
	public void yinCang(){
		this.setVisible(false);
	}
}
