from collections import deque
from time import sleep
import matplotlib.pyplot as plt
import serial

ser = serial.Serial("/dev/tty.usbmodem11301",9600)

num_points = 100  # 螢幕上同時顯示的座標點數量

x_values = deque(maxlen=num_points)  # 使用一個固定大小的deque來存儲x座標
y_values = deque(maxlen=num_points)  # 使用一個固定大小的deque來存儲y座標

plt.ion()  # 開啟互動模式

x = None
y = None

while True:
    getval = ser.readline()
    getval_str = getval.decode()  # 將bytes對象轉換為字符串
    getval_clean = getval_str.strip()  # 去除字符串兩端的換行符和空格


    # 檢查數據是否帶有 'x' 或 'y'
    if getval_clean and 'x' == getval_clean[0]:
        x = float(getval_clean[1:])  # 更新 x 座標
    elif 'y' == getval_clean[0]:
        y = float(getval_clean[1:])  # 更新 y 座標
    else:
        print(getval_clean)

    # 如果 x 和 y 都有值，則添加到列表中並繪製圖表
    if x is not None and y is not None:
        x_values.append(x)  # 將新的x座標添加到x_values列表中
        y_values.append(y)  # 將新的y座標添加到y_values列表中

        print(x, y)

        # 繪製圖表
        plt.clf()  # 清除舊的圖表
        plt.plot(x_values, y_values)
        plt.xlim(0, 255)  # 設定x軸的範圍
        plt.ylim(0, 255)  # 設定y軸的範圍
        plt.pause(0.00000000000000000000001)  # 暫停一下以更新圖表