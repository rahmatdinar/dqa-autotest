import serial.tools.list_ports

def checkArduinoPorts():
    serial.tools.list_ports.comports()
    arduino_ports = [p.device for p in serial.tools.list_ports.comports()]
    arduino_ports = [x for x in arduino_ports if x not in ['COM8', 'COM9']]
    return arduino_ports[0]