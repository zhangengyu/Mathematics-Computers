package chat.frame;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;

import cn.hutool.core.lang.Console;
import cn.hutool.json.JSONArray;
import cn.hutool.json.JSONObject;
import com.google.gson.Gson;

public class ALi {

    static class Message {
        String role;
        String content;

        public Message(String role, String content) {
            this.role = role;
            this.content = content;
        }
    }

    static class RequestBody {
        String model;
        Message[] messages;

        public RequestBody(String model, Message[] messages) {
            this.model = model;
            this.messages = messages;
        }
    }

    public static String query(String answer){
        try {
            // ����������
            RequestBody requestBody = new RequestBody(
                    "qwen-plus",
                    new Message[] {
                            new Message("user", answer)
                    }
            );

            // ��������ת��Ϊ JSON
            Gson gson = new Gson();
            String jsonInputString = gson.toJson(requestBody);
            Console.log("jsonInputString : {}", jsonInputString );

            // ���� URL ����
            URL url = new URL("https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions");
            HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();

            // �������󷽷�Ϊ POST
            httpURLConnection.setRequestMethod("POST");
            httpURLConnection.setRequestProperty("Content-Type", "application/json; utf-8");
            httpURLConnection.setRequestProperty("Accept", "application/json");

            // ��û�����û������������ð���API Key�������滻Ϊ��String apiKey = "sk-xxx";
            String apiKey = "sk-d335350e318d4b5ea0989e26602f8c8b";
            String auth = "Bearer " + apiKey;
            httpURLConnection.setRequestProperty("Authorization", auth);

            // �������������
            httpURLConnection.setDoOutput(true);

            // д��������
            try (OutputStream os = httpURLConnection.getOutputStream()) {
                byte[] input = jsonInputString.getBytes(StandardCharsets.UTF_8);
                os.write(input, 0, input.length);
            }

            // ��ȡ��Ӧ��
            int responseCode = httpURLConnection.getResponseCode();
            System.out.println("Response Code: " + responseCode);

            // ��ȡ��Ӧ��
            try (BufferedReader br = new BufferedReader(new InputStreamReader(httpURLConnection.getInputStream(), StandardCharsets.UTF_8))) {
                StringBuilder response = new StringBuilder();
                String responseLine;
                while ((responseLine = br.readLine()) != null) {
                    response.append(responseLine.trim());
                }
                System.out.println("Response Body: " + response);
                JSONObject jo = new JSONObject(response);
                JSONArray ja = jo.getJSONArray("choices");
                JSONObject oo = ja.getJSONObject(0);
                JSONObject mm = oo.getJSONObject("message");
                String cc = mm.getStr("content");
                Console.log("cc : {}", cc);
                return cc;
            }

        } catch (Exception e) {
            e.printStackTrace();
        } finally {
//            System.exit(0);
        }
        return "";
    }

    public static void main(String[] args) {
        String answer = ALi.query("北京市天气如何");
        Console.log("answer : {}", answer);
    }
}