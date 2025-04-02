
def calculate_crc_from_datagram(datagram, datagram_size):
    crc = 0
    for i in range(datagram_size - 1):
        byte = datagram[i]
        for _ in range(8):
            if (crc >> 7) ^ (byte & 0x01):
                crc = (crc << 1) ^ 0x07
            else:
                crc = crc << 1
            byte >>= 1
        crc &= 0xFF  # Ensure CRC remains 8-bit
    return crc


if __name__ == "__main__":
    datagram = [0x05, 0x00, 0x00, 0x48]
    crc = calculate_crc_from_datagram(datagram, 4)
    print(f"CRC for datagram {datagram}: 0x{crc:02X}")

    datagram = [0x05, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x01, 0xBB]
    crc = calculate_crc_from_datagram(datagram, 8)
    print(f"CRC for datagram {datagram}: 0x{crc:02X}")


