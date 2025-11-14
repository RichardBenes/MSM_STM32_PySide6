from PySide6.QtWidgets import (
    QMainWindow, QPushButton, QLabel, QWidget, QVBoxLayout, QApplication)

class MainWindow(QMainWindow):

    lbl: QLabel
    btn: QPushButton
    counter: int

    def __init__(self):
        super().__init__()

        self.counter = 0

        self.btn = QPushButton("Hit Me")
        self.btn.clicked.connect(self.update_counter)

        self.lbl = QLabel(str(self.counter))

        vlayout = QVBoxLayout()

        vlayout.addWidget(self.btn)
        vlayout.addWidget(self.lbl)

        holder = QWidget()
        holder.setLayout(vlayout)

        self.setCentralWidget(holder)
    
    def update_counter(self):

        self.counter += 1
        self.lbl.setText(str(self.counter))


if __name__ == "__main__":

    app = QApplication()
    main_window = MainWindow()
    main_window.show()

    app.exec()