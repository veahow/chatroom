package com.veahow.helloa;

import android.os.Vibrator;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class MainActivity extends AppCompatActivity {

    private Button button;    // 发送按钮
    private EditText editText;    // 聊天内容框
    private TextView textView;

    private Socket socket;    // 套接字
    private Vibrator vibrator;    // 振动

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        button = (Button) findViewById(R.id.button);    // 获取按钮实例
        editText = (EditText) findViewById(R.id.edit_text);    // 获取聊天内容实例
        textView = (TextView) findViewById(R.id.content_text_view);    // 服务器测试内容
        vibrator = (Vibrator) getSystemService(VIBRATOR_SERVICE);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 按钮点击后发送聊天内容到服务器server

                final String inputText = editText.getText().toString();    // 获取聊天内容

                vibrator.vibrate(30);

                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try{
                            socket = new Socket("192.168.43.188", 8080);

                            OutputStream os = socket.getOutputStream();
                            os.write(inputText.getBytes());
                            os.flush();

                            InputStream is = socket.getInputStream();

                            textView.setText(is.read());

                            os.close();
                            socket.close();

                        }catch(UnknownHostException e){
                            e.printStackTrace();
                        }catch(IOException e){
                            e.printStackTrace();
                        }

                    }
                }).start();



            }
        });
    }
}
