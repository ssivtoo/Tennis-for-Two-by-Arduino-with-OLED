import matplotlib.pyplot as plt

def calculate_dac_output(decimal_input):
    # Convert the decimal input to an 8-bit binary representation
    binary_input = format(decimal_input, '08b')
    n = len(binary_input)
    print(binary_input)
    
    output_voltage = 0
    # Iterate through each bit in the binary input
    for i in range(n):
        bit = int(binary_input[i])
        weight = 2 ** -(i+1)
        # Calculate the output voltage based on the bit value and weight
        output_voltage += bit * 5 * weight
    
    return output_voltage

# Test the calculate_dac_output function with an input of 255
print(calculate_dac_output(255))


#                   (2^0)*pin0 + (2^1)*pin1 + (2^2)*pin2 + (2^3)*pin3 ...... + (2^n)*pinn
# Output voltage = ----------------------------------------------------------------------- * Vref
#                                                   2^n