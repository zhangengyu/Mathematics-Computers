package chat.frame;

// 引入必要的工具类
import cn.hutool.core.io.FileUtil; // 文件操作工具类
import cn.hutool.core.util.StrUtil; // 字符串处理工具类
import login.frame.LoginFrame; // 登录框
import login.panel.LoginPanel; // 登录面板
import javax.swing.*; // 用于创建用户界面组件
import java.awt.*; // 提供图形界面布局的工具
import java.awt.event.ActionEvent; // 事件处理
import java.awt.event.ActionListener; // 事件监听器
import java.nio.charset.Charset; // 字符编码工具

public class ChatInterface extends JFrame {
    // 定义静态变量，用于保存文件路径
    public static String path = "C:\\Users\\詹耿羽\\Desktop\\chat(1)\\chat\\src";
    public static String path2 = "C:\\Users\\詹耿羽\\Desktop\\chat(1)\\out\\production\\chat(1)\\chat2.txt";

    // 定义用户界面组件
    private JTextArea chatArea; // 聊天显示区域
    private JTextField inputField; // 用户输入框
    private JButton sendButton; // 发送按钮
    private JButton zhongZhi; // 中止按钮
    private JComboBox<String> comboBox; // 模型选择框
    private static volatile boolean zz = false; // 用于中止线程的标志位

    // 构造函数，初始化聊天界面
    public ChatInterface() {
        createUI(); // 创建用户界面
    }

    // 创建用户界面的方法
    private void createUI() {
        // 设置窗口标题
        setTitle("知识问答机器人");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(600, 700); // 设置窗口大小
        setLocationRelativeTo(null); // 居中显示窗口

        // 创建一个面板，设置布局为边界布局
        JPanel panel = new JPanel(new BorderLayout());

        // 创建聊天区域并设置属性
        chatArea = new JTextArea(12, 30); // 定义文本区域的大小
        chatArea.setLineWrap(true); // 自动换行
        chatArea.setWrapStyleWord(true); // 保持单词完整性换行
        chatArea.setEditable(false); // 设置不可编辑

        // 创建下拉框，用于选择不同的模型
        String[] items = {"百度文心一言", "阿里通义千问"};
        comboBox = new JComboBox<>(items);
        panel.add(comboBox, BorderLayout.NORTH); // 添加到面板的顶部

        // 将聊天区域嵌套到滚动面板中，支持滚动查看
        JScrollPane scrollPane = new JScrollPane(chatArea);
        panel.add(scrollPane, BorderLayout.CENTER);

        // 创建输入框和发送按钮
        inputField = new JTextField(30); // 定义输入框长度
        JPanel p = new JPanel();
        p.setLayout(new BorderLayout());
        p.add(inputField, BorderLayout.CENTER);
        panel.add(p, BorderLayout.SOUTH);

        // 创建一个面板用于放置按钮
        JPanel p2 = new JPanel();
        zhongZhi = new JButton("中止"); // 中止按钮
        zhongZhi.addActionListener(new ActionListener() { // 按钮的事件监听
            @Override
            public void actionPerformed(ActionEvent e) {
                ChatInterface.zz = true; // 设置中止标志位
            }
        });

        // 创建发送按钮并绑定事件
        sendButton = new JButton("发送");
        sendButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                sendMessage(); // 调用发送消息的方法
            }
        });
        p2.add(sendButton); // 添加发送按钮
        p2.add(zhongZhi); // 添加中止按钮
        p.add(p2, BorderLayout.EAST); // 将按钮面板放置在右侧

        // 将整个面板添加到主窗口
        add(panel);

        // 创建菜单栏
        JMenuBar mb = new JMenuBar();
        JMenu heSuan = new JMenu("菜单"); // 创建菜单
        JMenuItem jMenuItem = new JMenuItem("退出"); // 创建退出选项
        heSuan.add(jMenuItem); // 将退出选项添加到菜单
        mb.add(heSuan); // 将菜单添加到菜单栏
        setJMenuBar(mb); // 将菜单栏设置到窗口上

        // 给退出选项绑定事件监听器
        ChatInterface c = this;
        jMenuItem.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                c.setVisible(false); // 隐藏当前窗口
                LoginFrame.instance.setVisible(true); // 显示登录窗口
            }
        });

        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    // 发送消息的方法
    private void sendMessage() {
        // 确定聊天记录的保存路径
        String p = path;
        String fileName = "chat{}.txt";
        fileName = StrUtil.format(fileName, LoginPanel.userId);
        p += fileName;
        final String pp = p;

        // 获取用户选择的模型
        String s = (String) comboBox.getSelectedItem();
        // 获取用户输入的消息
        String message = inputField.getText();
        String wen = "";

        // 如果消息不为空，将消息显示到聊天区域并保存到文件
        if (!message.trim().isEmpty()) {
            wen = "Me: " + message + "\n";
            chatArea.append(wen); // 显示到聊天区域
            inputField.setText(""); // 清空输入框
        }
        FileUtil.appendString(wen, pp, Charset.defaultCharset()); // 追加消息到文件

        // 创建新线程处理机器人回复
        new Thread(new Runnable() {
            @Override
            public void run() {
                String huiDa = "";
                if (StrUtil.equals(s, "阿里通义千问")) {
                    huiDa = ALi.query(message); // 调用阿里模型
                } else {
                    huiDa = BaiDuAI.query(message); // 调用百度模型
                }
                String da = "";

                // 如果有回复，将回复逐字显示
                if (!huiDa.trim().isEmpty()) {
                    da = "机器人: " + huiDa + "\n";
                    inputField.setText("");
                    for (int i = 0; i < da.length(); i++) {
                        try {
                            Thread.sleep(20); // 模拟逐字输出的延迟
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                        if (ChatInterface.zz) { // 检查中止标志位
                            ChatInterface.zz = false;
                            chatArea.append("\n");
                            return;
                        }
                        chatArea.append(da.charAt(i) + ""); // 逐字追加到聊天区域
                        inputField.repaint();
                    }
                }
                FileUtil.appendString(da, pp, Charset.defaultCharset()); // 保存回复到文件
            }
        }).start();
    }

    // 程序的入口
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() { // 保证线程安全
            public void run() {
                new ChatInterface(); // 创建并显示聊天窗口
            }
        });
    }
}
