import os
import sys

def binary_hook(binf, outf, boot_file='boot.bin', target_size=0xC000, padding_byte=b'\xFF'):
    try:
        # Read the application binary
        with open(binf, 'rb') as f:
            appbin = f.read()
        
        # Read the bootloader binary
        with open(boot_file, 'rb') as f:
            bootbin = f.read()
        
        # Calculate required padding for bootloader
        padding_size = target_size - len(bootbin)
        if padding_size < 0:
            raise ValueError("Boot binary is larger than the target size.")

        # Write bootloader + padding + app binary to output
        with open(outf, 'wb') as f:
            f.write(bootbin + (padding_byte * padding_size) + appbin)
        print(f"Successfully created {outf}")
    
    except FileNotFoundError as e:
        print(f"Error: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: python script.py <input_app_binary> <output_binary>")
    else:
        binary_hook(sys.argv[1], sys.argv[2])
