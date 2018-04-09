package sfailsthy;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.Socket;
import javax.swing.*;

public class ClientSocketFrame extends JFrame {
    private JTextArea ta_info;
    private File file = null;//声明所选择的File对象
    private JTextField tf_path;
    private DataInputStream in = null;//创建流对象
    private DataOutputStream out = null;//创建流对象

    private Socket socket;//声明Socket对象
    private long lengths = -1;//文件的大小
    private String fileName = null;

    private void connect() {
        //连接套接字方法
        try {
            //捕捉异常
            socket = new Socket("127.0.0.1", 2512);//实例化Socket对象
            ta_info.append("已连接到服务器。\n");//文本域中提示信息

            while (true) {
                if (socket != null && !socket.isClosed()) {
                    out = new DataOutputStream(socket.getOutputStream());//获得输出流对象
                    in = new DataInputStream(socket.getInputStream());//获得输入流对象
                    getServerInfo();
                } else {
                    socket = new Socket("127.0.0.1", 2512);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        ClientSocketFrame client = new ClientSocketFrame();
        client.setVisible(true);
        client.connect();
    }

    private void getServerInfo() {
        try {
            String name = in.readUTF();//读取文件名
            long lengths = in.readLong();//读取文件的长度
            byte[] bt = new byte[(int) lengths];//创建字节数组
            for (int i = 0; i < bt.length; i++) {
                bt[i] = in.readByte();//读取字节信息并存储到字节数组
            }

            FileDialog dialog = new FileDialog(ClientSocketFrame.this, "保存");
            dialog.setMode(FileDialog.SAVE);//设置对话框为保存对话框
            dialog.setFile(name);
            dialog.setVisible(true);//显示保存对话框
            String path = dialog.getDirectory();//获得文件的保存路径
            String newFileName = dialog.getFile();//获得保存的文件名
            if (path == null || newFileName == null) {
                return;
            }

            String pathAndName = path + "\\" + newFileName;//文件的完整路径
            FileOutputStream fOut = new FileOutputStream(pathAndName);
            fOut.write(bt);
            fOut.flush();
            fOut.close();
            ta_info.append("文件接收完毕。");
        } catch (Exception e) {
        } finally {
            try {
                if (in != null) {
                    in.close();//关闭流
                }
                if (socket != null) {
                    socket.close();//关闭套接字
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public ClientSocketFrame() {
        super();
        setTitle("客户端");
        setBounds(800, 100, 385, 150);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        final JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(4, 1));
        getContentPane().add(panel, BorderLayout.WEST);

        final JLabel label = new JLabel();
        label.setText("        文件路径:");
        panel.add(label);
        panel.setBackground(Color.LIGHT_GRAY);

        tf_path = new JTextField();
        tf_path.setPreferredSize(new Dimension(140, 25));

        panel.add(tf_path);
        final JButton button = new JButton();
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();//创建文件选择器
                int flag = fileChooser.showOpenDialog(null);//显示打开对话框

                if (flag == JFileChooser.APPROVE_OPTION) {
                    file = fileChooser.getSelectedFile();//获取选中文件的File对象
                }

                if (file != null) {
                    tf_path.setText(file.getAbsolutePath());//文件的完整路径
                    fileName = file.getName();//获得文件的名称
                }
            }
        });

        button.setText("选择文件");
        button.setBackground(Color.pink);
        panel.add(button);

        final JButton button1 = new JButton();
        button1.setBackground(Color.pink);
        button1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    DataInputStream inStream = null;//定义数据输入流对象
                    if (file != null) {
                        lengths = file.length();//获得所选择文件的大小
                        inStream = new DataInputStream(new FileInputStream(file));//创建输入流对象
                    } else {
                        JOptionPane.showMessageDialog(null, "还没有选择文件。");
                        return;
                    }
                    out.writeUTF(fileName);//写入文件名
                    out.writeLong(lengths);//将文件的大小写入输出流
                    byte[] bt = new byte[(int) lengths];//创建字节数组
                    int len = -1;
                    while (( len = inStream.read(bt) ) != -1) {
                        //将文件读取到字节数组
                        out.write(bt);//将字节数组写入输出流
                    }
                    out.flush();
                    out.close();
                    ta_info.append("文件发送完毕。");
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
            }
        });
        button1.setText("发送");
        panel.add(button1);

        final JScrollPane scrollPane = new JScrollPane();
        getContentPane().add(scrollPane, BorderLayout.CENTER);

        ta_info = new JTextArea();
        ta_info.setBackground(new Color(204, 232, 207));
        scrollPane.setViewportView(ta_info);
    }
}
