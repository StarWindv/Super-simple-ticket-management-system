import requests
import json
import subprocess
import time

# 设置Ollama的API端点
url = "http://localhost:8080/v1/chat/completions"
headers = {"Content-Type": "application/json"}

# 检查服务器是否已经启动
def is_server_running():
    try:
        response = requests.get(url)  # 发送一个GET请求，检查服务器状态
        return response.status_code == 404  # 如果返回200则说明服务器已启动
    except requests.exceptions.RequestException:
        return False  # 如果请求失败，表示服务器没有启动

# 启动Ollama服务器
def start_server():
    print("服务器未启动，正在启动Ollama模型服务器...\n\n")
    # 调用 ollama run model 启动服务器
    subprocess.Popen(["ollama", "run", "model"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    time.sleep(5)  # 等待几秒钟，确保服务器启动完成

# 初始化系统消息
def start_new_conversation():
    return [
        {"role": "system", "content": "你是CodeGeeX4"}
    ]

# 尝试加载之前的对话历史
def load_conversation_history():
    try:
        with open('conversation_history.json', 'r') as f:
            return json.load(f)
    except FileNotFoundError:
        return start_new_conversation()  # 文件不存在时，启动新对话

# 保存对话历史
def save_conversation_history(messages):
    with open('conversation_history.json', 'w') as f:
        json.dump(messages, f)

def ollama():
    # 加载或初始化对话历史
    messages = load_conversation_history()

    while True:
        # 获取用户输入
        user_input = input("\033[0m\n\033[100m>>>\033[0m")

        if user_input.lower() in ['/bye']:  # 用户输入/bye时退出
            print("退出对话\n")
            messages = start_new_conversation()  # 重置对话历史
            save_conversation_history(messages)
            user_input = None
            break

        # 检查是否需要开始新对话
        if user_input.lower() in ['/clear']:
            print("\033[0m\n记忆已清除\n")
            messages = start_new_conversation()  # 重置对话历史
            save_conversation_history(messages)  # 保存新的对话历史
            continue  # 跳过当前用户输入，开始新对话

        # 检查服务器是否运行，如果没有运行，则启动它
        if not is_server_running():
            start_server()

        # 将用户输入添加到对话历史
        messages.append({"role": "user", "content": user_input})

        # 构建请求体
        payload = {
            "model": "codegeex4:latest",  # 你可以更换成其他模型
            "messages": messages
        }

        # 发送请求
        response = requests.post(url, headers=headers, data=json.dumps(payload))

        if response.status_code == 200:
            response_data = response.json()
            
            # 获取模型的回复
            for choice in response_data['choices']:
                model_message = choice['message']['content']
                print("\n助手:\n\033[33m", model_message, end='\n\n')
            
            # 将模型的回复添加到对话历史
            messages.append({"role": "assistant", "content": model_message})

            # 保存更新的对话历史
            save_conversation_history(messages)
        
        else:
            print("Error:", response.status_code, response.text)

if __name__ == "__main__":
    ollama()
