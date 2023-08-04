import sys
import keyboard

count = 0

def callback(key):
    print(key.name)

print("\n现在是地键映射生成时间，请按以下顺序输入对应按键的值，大小写不限。")
print("+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n\
| 31 | 29 | 27 | 25 | 23 | 21 | 19 | 17 | 15 | 13 | 11 |  9 |  7 |  5 |  3 |  1 |\n\
+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n\
| 32 | 30 | 28 | 26 | 24 | 22 | 20 | 18 | 16 | 14 | 12 | 10 |  8 |  6 |  4 |  2 |\n\
+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n")
with open("./kb.ini", "w+") as f:
    f.write("[slider]\n")
    
    keyboard.on_press(callback)
    keyboard.wait()
    # for i in range(32):
    # temp_input = input("请输入" + str(i + 1) + "号键位：")
    # while temp_input == "" or len(temp_input) > 1:
    #     temp_input = input("请输入" + str(i + 1) + "号键位，不能为空且只有一位哦：")
    # f.write("0x" + str(i + 1) + "=" + str(hex(ord(str(temp_input)))) + "\n")
