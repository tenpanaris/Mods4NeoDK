In future, a PC application and possibly a ESP32 microcontroller will create data packets and stream them to the NeoDK.

These 'burst packets' are binary and fixed size.

For basic testing, right now test packets can be sent using a terminal emulator that has file upload capability. Test packet files are in this directory.

To edit packets, I recommend ImHex. I have included a pattern file for ImHex which lists the variables.

Frequencies are not actually frequencies, they are the inverse, or periods. Note the units for time related items, some are in microseconds(us) and some in milliseconds (ms)

<img width="1294" alt="imhex" src="https://github.com/user-attachments/assets/8e95b0b7-6928-424e-924a-0ec6bf872b0b">
