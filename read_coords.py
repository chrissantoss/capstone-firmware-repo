import subprocess
import re
import threading
import time
import serial

ser = serial.Serial("/dev/tty.usbmodem141101", 9600)


lock = threading.Lock()

# Initialize the global variables
x, y, z = None, None, None

def read_data():
    # Regular expression to match lines containing the "Unity" tag
    coordinate_regex = re.compile(r'Headset Position: \((-?\d+\.\d+), (-?\d+\.\d+), (-?\d+\.\d+)\)')

    process = subprocess.Popen(
        ['adb', 'logcat', '-s', 'Unity'],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True  # Ensure the output is text, not bytes
    )
    while(1):
        line = process.stdout.readline()
        if "Headset Position" in line:
            match = coordinate_regex.search(line)
            if match:
                with lock:
                    global x, y, z
                    x, y, z = match.groups()
                #outstr = f"{x}, {y}, {z}"
                #print(outstr)
            # time.sleep(0.5)
            # process.stdout.flush()
        





def run_adb_logcat():
    global x
    global y
    global z

    reader = threading.Thread(target=read_data)
    reader.start()
    # Start the subprocess

    try:
        # Continuously read the output line by line
        while True:
            # Read a line from stdout
            print(ser.readline().decode('utf-8').strip())
            with lock:
                global x, y, z
                outstr = f"{x}, {y}, {z}"
            print(outstr)
            ser.write(outstr.encode('utf-8'))
            ser.write(b'\n')
            #breakpoint()
          
    except KeyboardInterrupt:
        # Handle the user pressing Ctrl+C to stop the process
        print("Stopping logcat...")
        ser.close()
        #process.terminate()


run_adb_logcat()