import subprocess
import re
import serial 

ser = serial.Serial("/dev/tty.usbmodem145101", 9600)

def run_adb_logcat():
    # Start the subprocess
    # Regular expression to match lines containing the "Unity" tag
    coordinate_regex = re.compile(r'Headset Position: \((-?\d+\.\d+), (-?\d+\.\d+), (-?\d+\.\d+)\)')

    outstr = "1.0, 0.5, 1.0"

    try:
        # Continuously read the output line by line
        while True:
            print(ser.readline().decode('utf-8').strip())
            ser.write(outstr.encode('utf-8'))
            ser.write(b'\n')
            # Read a line from stdout
    except KeyboardInterrupt:
        ser.close()
        # Handle the user pressing Ctrl+C to stop the process
        print("Stopping logcat...")
        #process.terminate()


run_adb_logcat()
