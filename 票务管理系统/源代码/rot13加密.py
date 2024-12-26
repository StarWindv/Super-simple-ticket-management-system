def encrypt(text):
    encrypted_text = ""
    for char in text:
        if char.isalpha():
            ascii_offset = ord('a') if char.islower() else ord('A')
            encrypted_char = chr((ord(char) - ascii_offset + 13) % 26 + ascii_offset)
            encrypted_text += encrypted_char
        else:
            encrypted_text += char
    return encrypted_text

def decrypt(text):
    decrypted_text = ""
    for char in text:
        if char.isalpha():
            ascii_offset = ord('a') if char.islower() else ord('A')
            decrypted_char = chr((ord(char) - ascii_offset - 13 + 26) % 26 + ascii_offset)
            decrypted_text += decrypted_char
        else:
            decrypted_text += char
    return decrypted_text

# 测试
while True:
    user_input=input("请输入字符串\n\t");
    if user_input.lower() == "exit":
        break
    
    encrypted = encrypt(user_input)
    print("Encrypted:", encrypted)
    
    decrypted = decrypt(user_input)
    print("Decrypted:", decrypted)
    print()
print("退出程序\n\t")
