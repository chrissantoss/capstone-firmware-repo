import serial
import time
import math

# Constants
SERIAL_PORT = "/dev/tty.usbmodem146101"  # Change this to match your Arduino's port (e.g., "/dev/ttyUSB0" on Linux)
BAUD_RATE = 9600
MAX_X = 9
MAX_Y = 25
DELAY_BETWEEN_COORDS = 0.1  # 100ms

# Open serial connection
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Wait for Arduino to initialize
    print(f"Connected to Arduino on {SERIAL_PORT}")
except serial.SerialException as e:
    print(f"Error: Could not open serial port {SERIAL_PORT}\n{e}")
    exit(1)

def send_coordinates(x, y):
    """Send X,Y coordinates to Arduino over Serial."""
    if 0 <= x <= MAX_X and 0 <= y <= MAX_Y:
        coord_str = f"{x},{y}\n"
        ser.write(coord_str.encode())  # Send as bytes
        print(f"Sent: {coord_str.strip()}")
        time.sleep(DELAY_BETWEEN_COORDS)  # Wait before sending the next coordinate
    else:
        print(f"Error: Coordinates ({x}, {y}) out of range")

def move_hourglass():
    """Move the gantry in an hourglass pattern from corner to corner."""
    path = [
        (0, 0), (MAX_X, MAX_Y),  # Bottom-left to Top-right
        (MAX_X, 0), (0, MAX_Y),  # Top-right to Bottom-right
        (0, 0)  # Back to start
    ]
    for (x, y) in path:
        send_coordinates(x, y)

def move_circle(center_x=MAX_X // 2, center_y=MAX_Y // 2, radius=4, steps=36):
    """Move the gantry in a circular pattern."""
    for i in range(steps):
        angle = (i / steps) * 2 * math.pi  # Convert step to angle (radians)
        x = int(center_x + radius * math.cos(angle))
        y = int(center_y + radius * math.sin(angle))

        # Ensure values are within bounds
        x = max(0, min(MAX_X, x))
        y = max(0, min(MAX_Y, y))

        send_coordinates(x, y)

def main():
    while True:
        print("\nChoose a movement pattern:")
        print("1 - Hourglass")
        print("2 - Circle")
        print("3 - Exit")
        choice = input("Enter choice: ")

        if choice == "1":
            move_hourglass()
        elif choice == "2":
            move_circle()
        elif choice == "3":
            print("Exiting...")
            ser.close()
            break
        else:
            print("Invalid choice, please select 1, 2, or 3.")

if __name__ == "__main__":
    main()

