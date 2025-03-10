import subprocess
import re
import serial
import math

SERIAL_PORT = "/dev/tty.usbmodem145401"
import time

BAUD_RATE = 9600
coordinate_regex = re.compile(r'Head position: \((-?\d+\.\d+), (-?\d+\.\d+), (-?\d+\.\d+)\)')

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Wait for Arduino to initialize
    print(f"Connected to Arduino on {SERIAL_PORT}")
except serial.SerialException as e:
    print(f"Error: Could not open serial port {SERIAL_PORT}\n{e}")
    exit(1)

def get_audio_flinger_output():
    try:
        result = subprocess.run(
            ["adb", "shell", "dumpsys", "media.audio_flinger"],
            capture_output=True,
            text=True,
            check=True  # Raises CalledProcessError if the command fails
        )
        match = coordinate_regex.search(result.stdout)
        if match:
            x_match, z_match, y_match = match.groups()
        return float(x_match), float(y_match)

    except subprocess.CalledProcessError as e:
        print("Error running adb command:", e)
        print(result.stderr)
        print(result.stdout)
        return None, None
    except FileNotFoundError:
        print("ADB is not installed or not in PATH.")
        return None, None

def calculate_relative_coords(first_x, first_y, fwd_x, fwd_y, new_x, new_y):
    # Step 1: Calculate the forward vector
    fwd_vec = (fwd_x - first_x, fwd_y - first_y)
    # Step 2: Normalize the forward vector
    magnitude = math.sqrt(fwd_vec[0]**2 + fwd_vec[1]**2)
    norm_fwd_vec = (fwd_vec[0] / magnitude, fwd_vec[1] / magnitude)
   
    # Step 3: Calculate the right vector (perpendicular to forward vector)
    right_vec = (-norm_fwd_vec[1], norm_fwd_vec[0])
             
    # Step 4: Calculate relative coordinates
    rel_x = (new_x - first_x) * right_vec[0] + (new_y - first_y) * right_vec[1]
    rel_y = (new_x - first_x) * norm_fwd_vec[0] + (new_y - first_y) * norm_fwd_vec[1]
                             
    return rel_x, rel_y


def run_adb_logcat():
    # Regular expression to match lines containing the "Unity" tag
    input("get first coordinates")

    first_x, first_y = get_audio_flinger_output()
    input("move forward")

    fwd_x, fwd_y = get_audio_flinger_output()
    
    input("back to start")

    # Start the subprocess
    process = subprocess.Popen(
        ['pos_data.sh'],
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True  # Ensure the output is text, not bytes
    )

    try:
        # Continuously read the output line by line
        while True:
            # Read a line from stdout
            line = process.stdout.readline()
            if "Head position" in line:
                match = coordinate_regex.search(line)
                if match:
                    x_match, z_match, y_match = match.groups()
                    x = float(x_match)
                    y = float(y_match)
                    x_dist, y_dist = calculate_relative_coords(first_x, first_y, fwd_x, fwd_y, x, y)
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
                    ser.write(coord_str.encode())  # Send as bytes



    except KeyboardInterrupt:
        # Handle the user pressing Ctrl+C to stop the process
        print("Stopping logcat...")
        process.terminate()
        ser.close()


run_adb_logcat()
