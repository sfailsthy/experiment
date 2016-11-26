package hahaha;  
  
import java.awt.FlowLayout;  
import java.awt.event.ActionEvent;  
import java.awt.event.ActionListener;  
import java.io.BufferedReader;  
import java.io.IOException;  
import java.io.InputStreamReader;  
import java.io.PrintWriter;  
import java.net.InetAddress;  
import java.net.InetSocketAddress;  
import java.net.Socket;  
  
import javax.swing.JButton;  
import javax.swing.JFrame;  
import javax.swing.JLabel;  
import javax.swing.JScrollPane;  
import javax.swing.JTextArea;  
import javax.swing.JTextField;  
  
public class Client {  
  
    public static void main(String[] args) {  
        // TODO Auto-generated method stub  
        WindowClient win = new WindowClient();  
        win.setTitle("客户端");  
    }  
  
}  
  
class WindowClient extends JFrame implements Runnable, ActionListener {  
    JTextField ip, port;  
    JButton connection, send;  
    JTextField inputText;  
    JTextArea showResult;  
    Socket socket = null;  
    PrintWriter out = null;  
    BufferedReader in = null;  
    Thread thread;  
  
    WindowClient() {  
        socket = new Socket();  
        setLayout(new FlowLayout());  
        add(new JLabel("Serve IP:"));  
        ip = new JTextField("127.0.0.1", 8);  
        add(ip);  
        add(new JLabel("Serve Port:"));  
        port = new JTextField("8888", 8);  
        add(port);  
        connection = new JButton("Connect");  
        add(connection);  
        showResult = new JTextArea(15, 35);  
        add(showResult);  
        JScrollPane scroll = new JScrollPane(showResult);  
        add(scroll);  
        add(new JLabel("Say:"));  
        inputText = new JTextField(25);  
        add(inputText);  
        send = new JButton("发送");  
        send.setEnabled(false);  
        add(send);  
  
        connection.addActionListener(this);  
        send.addActionListener(this);  
        thread = new Thread(this);  
        setBounds(800, 100, 410, 400);  
        setVisible(true);  
        setResizable(false);  
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
    }  
  
    public void actionPerformed(ActionEvent e) {  
        if (e.getSource() == connection) {  
            try {  
                // 请求和服务器建立连接  
                if (socket.isConnected()) {  
                } else {  
                    InetAddress address = InetAddress.getByName("127.0.0.1");  
                    InetSocketAddress socketAddress = new InetSocketAddress(address, Integer.parseInt(port.getText()));  
                    socket.connect(socketAddress);  
                    showResult.append("Connect to server……\n\n");  
                    in = new BufferedReader(new InputStreamReader(socket.getInputStream()));  
                    out = new PrintWriter(socket.getOutputStream());  
                    send.setEnabled(true);  
                    if (!(thread.isAlive())) {  
                        thread = new Thread(this);  
                    }  
                    thread.start();  
                }  
            } catch (IOException e1) {  
                System.out.println(e1);  
                socket = new Socket();  
            }  
        }  
  
        if (e.getSource() == send) {  
            if (socket.isConnected()) {  
  
                String s = inputText.getText();  
                if (!s.isEmpty()) {  
                    out.println("客户端:" + s);  
                    showResult.append("客户端:" + s + "\n");  
                    inputText.setText(null);  
                    out.flush();  
                }  
            }  
        }  
  
    }  
  
    public void run() {  
        String s = null;  
        while (true) {  
            try {  
                s = in.readLine();  
                showResult.append(s + "\n");  
            } catch (Exception e) {  
                showResult.append("服务器已断开\n");  
                break;  
            }  
        }  
    }  
} 
