import argparse

def bin_to_header(input_file, output_file, array_name):
    with open(input_file, 'rb') as f:
        binary_data = f.read()

    hex_array = ', '.join(f'0x{byte:02X}' for byte in binary_data)
    header_content = f"""\
#ifndef {array_name.upper()}_H
#define {array_name.upper()}_H

#include <stdint.h>

const uint8_t {array_name}[] = {{
    {hex_array}
}};

const uint32_t {array_name}_size = {len(binary_data)};

#endif // {array_name.upper()}_H
"""

    with open(output_file, 'w') as f:
        f.write(header_content)

    print(f"Файл '{output_file}' успешно создан!")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Преобразование бинарного файла в C-заголовочный файл.')
    parser.add_argument('input', help='Входной бинарный файл')
    parser.add_argument('output', help='Выходной заголовочный файл (.h)')
    parser.add_argument('--name', default='binary_data', help='Имя массива в C-коде (по умолчанию: binary_data)')

    args = parser.parse_args()
    
    bin_to_header(args.input, args.output, args.name)