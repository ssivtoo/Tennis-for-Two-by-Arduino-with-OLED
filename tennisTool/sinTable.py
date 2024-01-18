import math
import matplotlib.pyplot as plt

sinlist = []
coslist = []
amplitude = 128
amplitude2 = 128
for i in range(0, 360):
    sinlist.append(int(math.sin(math.radians(i)) * amplitude + amplitude))
print(*sinlist, sep=", ")

for i in range(0, 360):
    coslist.append(int(math.cos(math.radians(i)) * 128 + amplitude2))
# print(*coslist, sep=", ")
# print(len(coslist))
# print(len(sinlist))

def get_voltage(num):
    binary = bin(num)[2:]
    binary = binary.zfill(8)
    # print(binary)
    voltage = 0
    for i in range(len(binary)):
        # voltage += int(binary[i])*5 * (2 ** i)
        voltage += int(binary[len(binary)-i-1])*5 * (2 ** i)
    voltage /= 2 ** len(binary) - 1
    return voltage

sinlistvoltage = []  # 創建一個空列表
for i in range(len(sinlist)):
    sinlistvoltage.append(get_voltage(sinlist[i]))  # 添加元素
for i in range(len(sinlist)):
    sinlistvoltage[i] = get_voltage(sinlist[i])
print(*sinlistvoltage, sep=", ")
        
fig, ax1 = plt.subplots()

# 繪製第一條線，並設定它的顏色為藍色
ax1.plot(sinlist, 'b-')
ax1.set_ylabel('sinlist', color='b')
ax1.tick_params('y', colors='b')

# 創建一個共享x軸的新座標軸
ax2 = ax1.twinx()

# 繪製第二條線，並設定它的顏色為紅色
ax2.plot(sinlistvoltage, 'r-')
ax2.set_ylabel('sinlistvoltage', color='r')
ax2.tick_params('y', colors='r')

plt.show()

