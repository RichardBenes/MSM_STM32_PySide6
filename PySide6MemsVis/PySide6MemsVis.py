from PySide6.QtWidgets import (
    QMainWindow, QPushButton, QLabel, QWidget, QVBoxLayout, QApplication)
from PySide6.QtSerialPort import QSerialPort, QSerialPortInfo
from PySide6.QtCore import QIODevice, QByteArray

import re

class MainWindow(QMainWindow):

    lbl: QLabel
    btn: QPushButton
    port: QSerialPort

    gx: int
    gy: int
    gz: int

    gyro_pattern: re.Pattern

    def __init__(self):
        super().__init__()

        self.gyro_pattern \
            = re.compile(r'GX(?P<gx>\d+)Y(?P<gy>\d+)Z(?P<gz>\d+)\n')

        self.port = QSerialPort("COM3", baudRate=230400)
        self.port.readyRead.connect(self.handle_com_data)

        if not self.port.open(QIODevice.OpenModeFlag.ReadOnly):
            print("Could not open the serial port")
            self.close()

        self.gx, self.gy, self.gz = 99, 99, 99

        self.gxlbl = QLabel("-")
        self.gylbl = QLabel("-")
        self.gzlbl = QLabel("-")

        self.update_gyrolbl()

        vlayout = QVBoxLayout()

        vlayout.addWidget(self.gxlbl)
        vlayout.addWidget(self.gylbl)
        vlayout.addWidget(self.gzlbl)

        holder = QWidget()
        holder.setLayout(vlayout)

        self.setCentralWidget(holder)
    
    def update_gyrolbl(self):
        self.gxlbl.setText(str(self.gx))
        self.gylbl.setText(str(self.gy))
        self.gzlbl.setText(str(self.gz))
    
    def handle_com_data(self):
        if self.port.bytesAvailable() < (len("GX0Y0Z0\n") * 2):
            return
        data: QByteArray = self.port.readAll()
        f: int = data.indexOf(b"G")
        if f == -1:
            return
        l: int = data.indexOf(b"G", f + 1)
        if l == -1:
            return
        slice: QByteArray = data.slice(f, l - f)

        m: re.Match[str] | None = self.gyro_pattern.match(slice.toStdString())
        
        if m is None:
            return
        
        self.gx = int(m.groupdict()['gx'])
        self.gy = int(m.groupdict()['gy'])
        self.gz = int(m.groupdict()['gz'])

        # TODO: use signal for this
        self.update_gyrolbl()


if __name__ == "__main__":

    app = QApplication()
    main_window = MainWindow()
    main_window.show()

    app.exec()