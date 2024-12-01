from PySide6.QtWidgets import QSlider, QToolTip
from PySide6.QtCore import Qt


class QSliderWithTooltip(QSlider):
    def __init__(self, orientation=Qt.Horizontal, parent=None):
        super().__init__(orientation, parent)

    def mouseMoveEvent(self, event):
        super().mouseMoveEvent(event)
        QToolTip.showText(event.globalPosition().toPoint(), f"Value: {(self.value()/10)}", self)
