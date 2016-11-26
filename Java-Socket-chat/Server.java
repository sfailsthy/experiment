package hahaha;  
  
import java.awt.FlowLayout;  
import java.awt.event.ActionEvent;  
import java.awt.event.ActionListener;  
import java.io.BufferedReader;  
import java.io.BufferedWriter;  
import java.io.IOException;  
import java.io.InputStreamReader;  
import java.io.OutputStreamWriter;  
import java.io.PrintWriter;  
import java.net.ServerSocket;  
import java.net.Socket;  
  
import javax.swing.JButton;  
import javax.swing.JFrame;  
import javax.swing.JLabel;  
import javax.swing.JScrollPane;  
import javax.swing.JTextArea;  
import javax.swing.JTextField;  
  
public class Server {  
  
    public static void main(String[] args) {  
        // TODO Auto-generated method stub  
        WindowServer win = new WindowServer();  
        win.setTitle("服务器");  
    }  
  
}  
  
class WindowServer extends JFrame implements ActionListener {  
    JButton start, send;  
    JTextField port;  
    JTextField inputText;  
    JTextArea showResult;  
    Socket socket = null;  
    PrintWriter out = null;  
    BufferedReader in = null;  
    ServerThread thread;  
  
    WindowServer() {  
        setLayout(new FlowLayout());  
        add(new JLabel("Port:"));  
        port = new JTextField("8888", 25);  
        add(port);  
        start = new JButton("start");  
        add(start);  
        showResult = new JTextArea(15, 35);  
        JScrollPane scroll = new JScrollPane(showResult);  
        add(scroll);  
        add(new JLabel("Say:"));  
        inputText = new JTextField(26);  
        add(inputText);  
        send = new JButton("发送");  
        send.setEnabled(false);  
        add(send);  
        start.addActionListener(this);  
        send.addActionListener(this);  
        showResult.append("Server starting……\n");  
        setBounds(400, 100, 400, 400);  
        setVisible(true);  
        setResizable(false);  
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
    }  
  
    public void actionPerformed(ActionEvent e) {  
        if (e.getSource() == start) {  
            try {  
                ServerSocket s = new ServerSocket(Integer.parseInt(port.getText()));  
                socket = s.accept();  
                showResult.append("Client connected……\n\n");  
                // PrintWriter  
                out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())));  
                // BufferedReader  
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));  
                thread = new ServerThread();  
                thread.start();  
                send.setEnabled(true);  
            } catch (IOException e1) {  
            }  
        }  
  
        if (e.getSource() == send) {  
            if (socket.isConnected()) {  
                String s = inputText.getText();  
                if (!s.isEmpty()) {  
                    out.println("服务器:" + s);  
                    showResult.append("服务器:" + s + "\n");  
                    inputText.setText(null);  
                    out.flush();  
                }  
            }  
        }  
  
    }  
  
    class ServerThread extends Thread {  
        String s = null;  
  
        public void run() {  
            while (true) {  
                if (socket.isConnected()) {  
                    try {  
                        s = in.readLine();  
                        showResult.append(s + "\n");  
                    } catch (IOException e) {  
                        showResult.append("与服务器已断开\n");  
                        break;  
                    }  
                } else {  
                    showResult.append("与服务器已断开\n");  
                }  
            }  
        }  
    }  
}
