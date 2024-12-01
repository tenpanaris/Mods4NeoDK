import struct
import sys
from PySide6.QtCore import QIODeviceBase
from PySide6.QtWidgets import QLabel, QMainWindow, QMessageBox
from PySide6.QtWidgets import QApplication

from PySide6.QtSerialPort import QSerialPort

from ui_burst_creator import Ui_MainWindow
from settingsdialog import SettingsDialog

from PySide6.QtCore import QThread, Signal


class SerialReaderThread(QThread):
    data_received = Signal(str)  # Signal to send data to the UI thread

    def __init__(self, serial_port: QSerialPort):
        super().__init__()
        self.serial_port = serial_port
        self.running = True  # Control flag for the thread loop

    def run(self):
        while self.running:
            if self.serial_port.waitForReadyRead(10):  # Wait for data
                data = self.serial_port.readAll().data().decode('utf-8')  # Read and decode
                self.data_received.emit(data)  # Send data to the main thread

    def stop(self):
        self.running = False
        self.quit()
        self.wait()


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.NeoWindow=Ui_MainWindow()
        self.settingsDialog = SettingsDialog(self)
        self.serialPort = QSerialPort(self)
        self.NeoWindow.setupUi(self)
        self.NeoWindow.buttonSend.clicked.connect(self.start_burst)
        self.NeoWindow.buttonSend.clicked.connect(self.start_listening)
        self.NeoWindow.actionCOM_setting.triggered.connect(self.settingsDialog.show)
        self.buffer = bytearray()
        self.reader_thread = SerialReaderThread(self.serialPort)
        self.reader_thread.data_received.connect(self.update_text_box)

    def show_status_message(self, msg):
        self.NeoWindow.statusbar.showMessage(msg)
        
    def start_burst(self):
        self.pack_data()
        ##debugging: write the data to a file for inspection with a hex editor
        ##with open("output.bin", "wb") as file:
        ##    file.write(self.buffer)
        if not self.serialPort.isOpen():
            if not self.open_serial_port():
                QMessageBox.critical(self, "Critical Error","Com port opening failed")
                return
            self.show_status_message("port opened")
        self.serialPort.write(self.buffer)

    def pack_data(self): #this should be refactored. While compact, it is impossible to debug the conversions - especially converting frequencies to periods with integer decimal etc.
        self.buffer.clear()
        self.buffer.extend(struct.pack('<I', int((self.NeoWindow.sliderDuration.value()*100))))
        self.buffer.extend(struct.pack('B', int((self.NeoWindow.sliderPW.value()/10))))
        self.buffer.extend(struct.pack('<H', int(((1/(self.NeoWindow.sliderFrequency.value()/10))*1000000))))
        self.buffer.extend(struct.pack('B', int((self.NeoWindow.sliderVoltage.value() ))))
        self.buffer.extend(struct.pack('B', int((self.NeoWindow.sliderVoltageWaveform.value()))))
        self.buffer.extend(struct.pack('<H', int(((1 / (self.NeoWindow.sliderVoltageModFreq.value()/10)) * 1000))))
        temp = int((self.NeoWindow.sliderVoltage.value() - (self.NeoWindow.sliderVoltage.value() * (self.NeoWindow.sliderVoltageModAmt.value() / 1000))))
        temp = temp if temp > 10 else 10
        self.buffer.extend(struct.pack('B', temp ))
        self.buffer.extend(struct.pack('B', int((self.NeoWindow.sliderPWModWaveform.value()))))
        self.buffer.extend(struct.pack('<H', int(((1 / (self.NeoWindow.sliderPWModFreq.value() / 10)) * 1000))))
        temp=int((self.NeoWindow.sliderPW.value() / 10 - (self.NeoWindow.sliderPW.value() / 10 * (self.NeoWindow.sliderPWModAmt.value() / 1000))))
        temp = temp if temp > 60 else 60
        self.buffer.extend(struct.pack('B',temp ))
        self.buffer.extend(struct.pack('B', int((self.NeoWindow.sliderFrequencyModWaveform.value()))))
        self.buffer.extend(struct.pack('<H', int(((1 / (self.NeoWindow.sliderFrequencyModFreq.value() / 10)) * 1000))))
        temp=int((1/(self.NeoWindow.sliderFrequency.value()/10 - (self.NeoWindow.sliderFrequency.value()/10 * (self.NeoWindow.sliderFrequencyModAmt.value() / 1000))))*1000000)
        temp = temp if temp<65535 else 65534
        self.buffer.extend(struct.pack('<H', temp ))
        self.buffer.extend(struct.pack('B', int(self.NeoWindow.sliderPolarity.value())))
        self.buffer.extend(struct.pack('<H', int((self.NeoWindow.sliderPause.value() * 100))))
        self.buffer.extend(struct.pack('<H', int(self.NeoWindow.spinRepeats.value())))
        self.buffer.extend(struct.pack('B', int(1)))     # hardcode to immediate run packets; 0 would just add this to the buffer on the neodk, which may be desired in the future.


    def start_listening(self):
        self.reader_thread.start()

    def stop_listening(self):
        self.reader_thread.stop()

    def update_text_box(self, data: str):
        self.NeoWindow.textEdit.append(data)

    #override close event to ensure serial port is closed
    def closeEvent(self, event):
        if self.reader_thread.isRunning():
            self.reader_thread.stop()
        if self.serialPort.isOpen():
            self.serialPort.close()
            print("Serial port closed.")
        event.accept()  # Proceed with closing the window 
        
    def open_serial_port(self):
        s = self.settingsDialog.settings()
        self.serialPort.setPortName(s.name)
        self.serialPort.setBaudRate(s.baud_rate)
        self.serialPort.setDataBits(s.data_bits)
        self.serialPort.setParity(s.parity)
        self.serialPort.setStopBits(s.stop_bits)
        self.serialPort.setFlowControl(s.flow_control)
        if self.serialPort.open(QIODeviceBase.ReadWrite):
            self.show_status_message("Serial Communication started")
            return True
        else:
            QMessageBox.critical(self, "Error", self.serialPort.errorString())
            self.show_status_message("Serial Communication error")
            return False
            
            
if __name__ == "__main__":
    a = QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(a.exec())            
        
        