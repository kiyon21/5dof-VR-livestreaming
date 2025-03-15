import subprocess
import re
import time

# extracts the ip address from ifconfig output
def extract_ipv4_address(network_output):
    # Regex pattern to find 'inet addr'
    match = re.search(r'inet addr:(\d+\.\d+\.\d+\.\d+)', network_output)
    
    if match:
        return match.group(1)
    else:
        return "No IPv4 address found.\n"

# checks inputted device to see if in IP_ADDR:PORT format
def is_ip_device(device):
    """Check if the device string is in an IP address format with a port"""
    return re.match(r'^\d+\.\d+\.\d+\.\d+:\d+$', device) is not None

# extracts adb device ip 
def get_device_ip():
    attempts = 0
    try:
        while(attempts < 5):
            # run adb devices to check for meta quest
            devices = adb_devices()

            if devices:
                print("Found device! Extracting IP...\n")
                result = subprocess.run(["adb", "shell", "ifconfig", "wlan0"], capture_output=True, text=True) # gets connected device ip
                ip_addr = extract_ipv4_address(result.stdout) # extract ip from output
                if ip_addr == "No IPv4 Address found":
                    print(ip_addr+ " Trying again...")
                    time.sleep(1)
                    continue
                else:
                    return ip_addr
            else:
                print("Devices not found... Make sure Quest is connected.. Trying again in 5...\n")
                time.sleep(5)
                attempts += 1
        
    except FileNotFoundError:
        print("ADB not installed")
        return None

# runs adb devices command and returns devices
def adb_devices():
    result = subprocess.run(["adb", "devices"], capture_output=True, text=True)
    lines = result.stdout.split("\n")
    devices = [line.split("\t")[0] for line in lines[1:] if "\tdevice" in line]
    return devices if devices else []
        
    

def set_up_adb_wireless():
    for device in filter(is_ip_device, adb_devices()): # check to see if already wirelessly connected
        print("You should already be connected... Exiting.")
        return
    
    quest_ip = get_device_ip()
    if not quest_ip:
        return

    connected = False

    result = subprocess.run(["adb", "tcpip", "5555"])
    ip_string = quest_ip+":5555"
    input("Please Disconnect Headset... Press Enter to continue\n")
    attempts = 0
    while(attempts <= 5):
        result = subprocess.run(["adb", "devices"], capture_output=True, text=True)
        lines = result.stdout.split("\n")
        devices = [line.split("\t")[0] for line in lines[1:] if "\tdevice" in line]
        if ip_string in devices: # device is already connected, no need to run adb connect
            print("Device already wirelessly connected")
            connected = True
            break
        elif devices:
            print("Waiting for you to Disconnect. Sleeping for 5 seconds\n")
            time.sleep(5)
        else:
            break
    
    attempts = 0
    while(not connected and attempts <= 5):
        
        result = subprocess.run(["adb","connect",ip_string], capture_output=True, text=True) 
        output = result.stdout.strip()

        if "connected" in output:
            break
        else:
            print(output)

    print("Connected adb wirelessly!\n")

if __name__ == "__main__":
    set_up_adb_wireless()