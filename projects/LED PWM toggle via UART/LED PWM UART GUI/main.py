import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, \
    QSlider, QPushButton, QHBoxLayout, QLabel, QComboBox
from PyQt5.QtCore import Qt
import serial

class SerialSlider(QWidget):
    def __init__(self):
        super().__init__()

        self.serial_port = None
        self.connected = False

        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()
        layout.setContentsMargins(20, 20, 20, 20)
        self.setStyleSheet("background-color: #333333; color: \
            white; border: 2px solid lightblue; border-radius: 10px;")

        # COM Port Selection
        com_layout = QHBoxLayout()
        com_label = QLabel("COM Port:")
        com_label.setStyleSheet("font-size: 16px; font-weight: \
            bold; border-radius: 5px;")
        self.com_combo = QComboBox()
        self.refresh_com_ports()
        self.com_combo.setStyleSheet("font-size: 16px; font-weight: \
            bold; padding: 8px; background-color: darkblue; color: \
            white; border: 2px solid lightblue; border-radius: 5px;")
        com_layout.addWidget(com_label)
        com_layout.addWidget(self.com_combo)
        layout.addLayout(com_layout)

        # Baud Rate Selection
        baud_layout = QHBoxLayout()
        baud_label = QLabel("Baud Rate:")
        baud_label.setStyleSheet("font-size: 16px; font-weight: \
            bold; border-radius: 5px;")
        self.baud_combo = QComboBox()
        self.baud_combo.addItems(["9600", "19200", "38400", "57600", "115200"])
        self.baud_combo.setStyleSheet("font-size: 16px; font-weight: bold; \
            padding: 8px; background-color: darkblue; color: white; border: \
            2px solid lightblue; border-radius: 5px;")
        baud_layout.addWidget(baud_label)
        baud_layout.addWidget(self.baud_combo)
        layout.addLayout(baud_layout)

        # Connect/Disconnect Button
        self.connect_btn = QPushButton("Connect")
        self.connect_btn.setStyleSheet("font-size: 16px; font-weight: bold; \
            padding: 10px; background-color: darkblue; color: white; border: \
            2px solid lightblue; border-radius: 5px;")
        self.connect_btn.clicked.connect(self.toggle_connection)
        layout.addWidget(self.connect_btn)

        # Space
        layout.addSpacing(20)

        # Title
        title_label = QLabel("LED Brightness (PWM duty cycle)")
        title_label.setStyleSheet("font-size: 16px; font-weight: bold; border: none;")
        layout.addWidget(title_label)

        # Slider
        self.slider = QSlider(Qt.Horizontal)
        self.slider.setMinimum(0)
        self.slider.setMaximum(100)
        self.slider.setStyleSheet("""
            QSlider::handle:horizontal {
                background-color: white;
                width: 20px;
                height: 20px;
                border-radius: 10px;
            }
            QSlider::groove:horizontal {
                background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                                  stop:0 lightblue, stop:1 darkblue);
                height: 20px;
                border-radius: 10px;
            }
        """)
        self.slider.valueChanged.connect(self.send_serial)
        layout.addWidget(self.slider)

        self.setLayout(layout)

    def refresh_com_ports(self):
        self.com_combo.clear()
        ports = [f"COM{p}" for p in range(1, 10)]
        self.com_combo.addItems(ports)

    def toggle_connection(self):
        if not self.connected:
            port = self.com_combo.currentText()
            baud_rate = int(self.baud_combo.currentText())
            try:
                self.serial_port = serial.Serial(port, baud_rate)
                self.connect_btn.setText("Disconnect")
                self.connected = True
                self.connect_btn.setStyleSheet("font-size: 16px; font-weight: \
                    bold; padding: 10px; background-color: red; color: white; \
                    border: 2px solid #99ccff; border-radius: 5px;")
            except serial.SerialException as e:
                print(f"Error: {e}")
        else:
            if self.serial_port:
                self.serial_port.close()
            self.connect_btn.setText("Connect")
            self.connected = False
            self.connect_btn.setStyleSheet("font-size: 16px; font-weight: bold; \
                padding: 10px; background-color: darkblue; color: white; border: \
                2px solid lightblue; border-radius: 5px;")

    def send_serial(self, value):
        if self.serial_port and self.serial_port.is_open:
            hex_value = format(value, '02X')  # Convert the integer value to hexadecimal string
            self.serial_port.write(bytes.fromhex(hex_value))  # Write the hexadecimal value to the serial port

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = SerialSlider()
    window.setWindowTitle('Serial Slider')
    window.show()
    sys.exit(app.exec_())
