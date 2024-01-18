import matplotlib.pyplot as plt

def calculate_dac_output(decimal_input):
    binary_input = format(decimal_input, '08b')
    n = len(binary_input)
    print(binary_input)
    output_voltage = 0
    for i in range(n):
        bit = int(binary_input[i])
        weight = 2 ** -(i+1)
        output_voltage += bit*5 * weight
    return output_voltage

print(calculate_dac_output(255))

# outputs = [calculate_dac_output(i) for i in range(256)]

# plt.plot(outputs)
# plt.title('DAC Output')
# plt.xlabel('Input')
# plt.ylabel('Output')
# plt.show()
