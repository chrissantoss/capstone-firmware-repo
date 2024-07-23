import subprocess
import re


def run_adb_logcat():
    # Start the subprocess
    process = subprocess.Popen(
        ['adb', 'logcat', '-s', 'Unity'],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True  # Ensure the output is text, not bytes
    )
    
    # Regular expression to match lines containing the "Unity" tag
    coordinate_regex = re.compile(r'Headset Position: \((-?\d+\.\d+), (-?\d+\.\d+), (-?\d+\.\d+)\)')

    try:
        # Continuously read the output line by line
        while True:
            # Read a line from stdout
            line = process.stdout.readline()
            #breakpoint()
            if "Headset Position" in line:
                #breakpoint()
                match = coordinate_regex.search(line)
                if match:
                    x, y, z = match.groups()
                    print(f"({x}, {y}, {z})")
    except KeyboardInterrupt:
        # Handle the user pressing Ctrl+C to stop the process
        print("Stopping logcat...")
        process.terminate()


run_adb_logcat()