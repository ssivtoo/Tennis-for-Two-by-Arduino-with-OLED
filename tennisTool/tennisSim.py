import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import serial

# 開啟串行端口
ser = serial.Serial('/dev/tty.usbmodem11301', 115200)

# 設定球的初始位置和速度
x = 64
y = 128
vx = 0
vy = 0

# 設定重力加速度
g = -0.05

# 設定速度衰減因子
damping = 0.9

# 設定反彈次數
bounce_count = 11

# 建立畫布和球的物件
fig, ax = plt.subplots()
ball, = ax.plot(x, y, 'o', color='b', markersize=20)

# 設定畫布的邊界
ax.set_xlim(0, 256)
ax.set_ylim(0, 256)

# 更新函數
def update(frame):
    global x, y, vx, vy, bounce_count

    # 從串行端口讀取球員的擊球角度和是否按下擊球按鈕
    if ser.in_waiting:
        data = ser.readline().strip().split()
        if len(data) == 4:
            angle1 = int(data[0]) * np.pi / 32  # 將角度轉換為弧度
            angle2 = int(data[1]) * np.pi / 32
            hit1 = int(data[2])
            hit2 = int(data[3])

            # 如果球員按下擊球按鈕，根據擊球角度更新球的速度
            if hit1 == 0:
                vx = np.cos(angle1)
                vy = np.sin(angle1)
            if hit2 == 0:
                vx = np.cos(angle2)
                vy = -np.sin(angle2)  # 注意 y 軸的方向是向下的

    x += vx
    y += vy

    # 加入重力效應
    vy += g

    # 如果球碰到邊界，反轉速度
    if x < 0:
        x = 0
        vx = -vx
        bounce_count += 1
    elif x > 256:
        x = 256
        vx = -vx
        bounce_count += 1
    if y < 0:
        y = 0
        vy = -vy * damping  # 在反彈時應用速度衰減
        bounce_count += 1
    elif y > 256:
        y = 256
        vy = -vy  # 在撞到天花板時不應用速度衰減
        bounce_count += 1

    # 如果反彈次數達到10次，重置球的位置和速度
    if bounce_count >= 10:
        x = 64
        y = 128
        vx = 0
        vy = 0
        bounce_count = 0

    ball.set_data(x, y)
    return ball,

# 建立動畫
ani = animation.FuncAnimation(fig, update, frames=range(1000), interval=10, blit=True)

plt.show()