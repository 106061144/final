import time
import serial
import sys,tty,termios

class _Getch:
    def __call__(self):
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

def get():
    inkey = _Getch()
    while(1):
        k=inkey()
        if k!='':break
    if k=='\x1b':
        d1 = input()
        if d1=="1":
            print("Choose a road ID:")
            d2 = input()
            line = "/PART/run " + d1 + " " + d2 + " \n"
        else:
            line = "/PART/run " + d1 + " \n"
        
        print(line)
        s.write(line.encode())
        #time.sleep(1)
        #s.write("/stop/run \n".encode())
    elif k=='q':
        print ("quit")
        return 0
    else:
        print ("not an arrow key!")
    return 1

if len(sys.argv) < 1:
    print ("No port input")
s = serial.Serial(sys.argv[1])
while get():
    i = 0
