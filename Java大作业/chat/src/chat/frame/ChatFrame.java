//package chat.frame;
//
//import login.frame.LoginFrame;
//import login.panel.LoginPanel;
////import parent.frame.ParentFrame;
//
//import javax.swing.*;
//
//public class ChatFrame  extends ParentFrame {
//
//    /**
//     *
//     */
//    private static final long serialVersionUID = 1L;
//
////	public JPanel adminPage = new JPanel();
//
//    public static ChatFrame instance = new ChatFrame();
//
//    public ChatFrame() {
//        super("����");
//
//        String[] items = {"Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "Item 6", "Item 7", "Item 8", "Item 9", "Item 10"};
//        JList<String> jList = new JList<>(items);
//        JScrollPane scrollPane = new JScrollPane(jList);
//        this.add(scrollPane);
//
////        LoginPanel.instance.setOpaque(false);
////        this.add(LoginPanel.instance);
//
////		ImageIcon bg=new ImageIcon("C:/Users/55025/Desktop/login2.jpg");
////		JLabel label=new JLabel(bg);
////		JPanel jPanel = new JPanel();
////		label.setBounds(0, 0, acWidth(), acHeight());
////		jPanel.add(label);
////		getLayeredPane().add(jPanel, JLayeredPane.DEFAULT_LAYER);
//    }
//
//    @Override
//    protected int acWidth() {
//        // TODO Auto-generated method stub
//        return 800;
//    }
//
//    @Override
//    protected int acHeight() {
//        // TODO Auto-generated method stub
//        return 600;
//    }
//
//    @Override
//    public void xianShi() {
//        LoginPanel.instance.userNameInput.setText("");
//        LoginPanel.instance.passwordInput.setText("");
//        super.xianShi();
//    }
//
//}