package chat.frame;

import cn.hutool.core.lang.Console;
import cn.hutool.core.util.ObjectUtil;
import cn.hutool.http.HttpUtil;
import cn.hutool.json.JSONObject;
//import com.alibaba.fastjson.JSONObject;

public class BaiDuAI {

    public static final String url = "https://luckycola.com.cn/ai/openwxyy";

    public static final String uid = "MOcNXW1718087588744FPC4KDAnWx";

    public static final String appKey = "6667f0686e15113935ab0203";

    public static final int isLongChat = 1;

//    {
//        "ques": "数据结构相关书籍和资料推荐",
//            "appKey": "6667f0686e15113935ab0203",
//            "uid": "MOcNXW1718087588744FPC4KDAnWx",
//            "isLongChat": 1
//    }


    public static String query(String ques){

        JSONObject jsonObject = new JSONObject();
        jsonObject.put("ques", ques);
        jsonObject.put("appKey", appKey);
        jsonObject.put("uid", uid);
        jsonObject.put("isLongChat", 1);

        String body = HttpUtil.post(url, jsonObject.toString());
        Console.log("body : {}", body);
        JSONObject jo = new JSONObject(body);
        Integer i = jo.getInt("code");
        if(ObjectUtil.equals(i, 0)){
            return jo.getJSONObject("data").getStr("result");
        }else{
            return "暂时无法解答，你可以问些其他问题！";
        }
    }

    public static void main (String args[]){
        query("你是谁呀");
    }

}
