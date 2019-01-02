package com.veahow.helloa;

import android.app.ProgressDialog;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class MainActivity extends AppCompatActivity {

    public static final int CONNECTING = 0;
    public static final int UPDATE_TEXT = 1;

    private Button send_button = null;    // 发送按钮
    private EditText editText = null;    // 聊天内容框
    private TextView textView = null;
    private Button connect_button = null;    // 连接服务器按钮

    private static Socket socket = null;    // 套接字
    private static OutputStream os = null;
    private static InputStream is = null;
    private Vibrator vibrator = null;    // 振动

    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case CONNECTING:
                    if(socket != null && !socket.isClosed()){

                        Toast.makeText(MainActivity.this, "服务器连接成功 Successful",
                                Toast.LENGTH_SHORT).show();
                        connect_button.setEnabled(false);
                        textView.append("成功连接服务器！\n");
                    } else{
                        Toast.makeText(MainActivity.this, "服务器连接失败 Failed",
                                Toast.LENGTH_SHORT).show();
                    }
                    break;
                case UPDATE_TEXT:
                    textView.append(msg.obj.toString() + "\r\n");
                    break;
                default:
                    break;
            }
        }
    };

    /**
     * 建立服务端连接
     */
    public synchronized void connectServer(View v) {
        new Thread() {

            @Override
            public void run() {

                try {
                    socket = new Socket("192.168.43.188", 8080);

                    Message connMessage = handler.obtainMessage();
                    connMessage.what = CONNECTING;
                    handler.sendMessage(connMessage);

                    OutputStream os = socket.getOutputStream();
                    os.write("android".getBytes("GB2312"));    // 中文显示需要GB2312编码
                    os.flush();

                    is = socket.getInputStream();
                    //Message message = new Message();
                    for(;;){
                        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(is, "GB2312"));
                        String str = bufferedReader.readLine();
                        Log.d("recv-info", str);

                        Message recvMessage = handler.obtainMessage();
                        recvMessage.what = UPDATE_TEXT;
                        recvMessage.obj = str;
                        handler.sendMessage(recvMessage);

                    }

                } catch (UnknownHostException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }.start();
    }

    /**
     * 发送消息到服务器
     * */
    public void send(View v){
        new Thread(){
            @Override
            public void run(){
                try {

                    final String inputText = editText.getText().toString();    // 获取聊天内容

                    os = socket.getOutputStream();
                    os.write(inputText.getBytes("GB2312"));
                    os.flush();

                } catch (UnknownHostException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        }.start();
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        send_button = (Button) findViewById(R.id.send_button);    // 获取按钮实例
        connect_button = (Button) findViewById(R.id.connect_button);
        editText = (EditText) findViewById(R.id.edit_text);    // 获取聊天内容实例
        textView = (TextView) findViewById(R.id.content_text_view);    // 服务器测试内容
        vibrator = (Vibrator) getSystemService(VIBRATOR_SERVICE);    // 振动

        connect_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                vibrator.vibrate(30);
                connectServer(v);
            }
        });

        send_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 按钮点击后发送聊天内容到服务器server
                vibrator.vibrate(30);

                if(socket != null && !socket.isClosed()){
                    send(v);
                    editText.setText("");
                }

            }

        });
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        try {
            is.close();    // 关闭输入流
            os.close();    // 关闭输出流
            socket.close();    // 关闭套接字
        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

}
