import subprocess
import re
import serial
import setup_adb_wireless as setup
import threading

SERIAL_PORT = "/dev/tty.usbmodem143101"  # Change this to match your Arduino's port (e.g., "/dev/ttyUSB0" on Linux)
import time

BAUD_RATE = 9600

running = True

# try:
#     ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
#     time.sleep(2)  # Wait for Arduino to initialize
#     print(f"Connected to Arduino on {SERIAL_PORT}")
# except serial.SerialException as e:
#     print(f"Error: Could not open serial port {SERIAL_PORT}\n{e}")
#     exit(1)

def run_adb_logcat():
    # Start the subprocess
    global running

    process = subprocess.Popen(
        ['./pos_data.sh'],
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True  # Ensure the output is text, not bytes
    )
    
    # Regular expression to match lines containing the "Unity" tag
    coordinate_regex = re.compile(r'Head position: \((-?\d+\.\d+), (-?\d+\.\d+), (-?\d+\.\d+)\)')
    first_x = None
    first_y = None
    try:
        # Continuously read the output line by line
        while running:

            # Read a line from stdout
            line = process.stdout.readline()
            #breakpoint()
            if "Head position" in line:

                #breakpoint()
                match = coordinate_regex.search(line)
                if match:
                    x_match, z_match, y_match = match.groups()
                    x = float(x_match)
                    y = float(y_match)
                    x = -x
                    if first_x == None:
                        first_x = x
                        first_y = y
                        continue

                    x_dist = x - first_x 
                    y_dist = y - first_y

                    if x_dist > 0:
                        if x_dist > 1.0:
                            x_dist = 1.0
                    else:
                        x_dist = 0

                    x_coord = int(x_dist/(1/9))

                    if y_dist > 0:
                        if y_dist > 2.0:
                            y_dist = 2.0
                    else:
                        y_dist = 0

                    y_coord = int(y_dist/(2/23))
                    print(f"{x_coord}, {y_coord}")
                    coord_str = f"{x_coord},{y_coord}\n"
                    # ser.write(coord_str.encode())  # Send as bytes



    except KeyboardInterrupt:
        # Handle the user pressing Ctrl+C to stop the process
        print("Stopping logcat...\n")
        process.terminate()
        # ser.close()


def main():
    input("Plug in the Quest Headset and Press enter...\n")
    print("Starting Wireless ADB Setup...\n")
    setup.set_up_adb_wireless()
    input("Press enter to start running Logcat ADB\n")
    print("Starting up adb logcat...\n")
    
    while(setup.adb_devices is not None):
        input("Press Enter to start reading coordinates...\n")
        run_adb_logcat()
        print("Stopping adb logcat")


if __name__ ==  "__main__":
    while(1):
        main()
        print("No Devices found...")