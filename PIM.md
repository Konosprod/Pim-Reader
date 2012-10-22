Header:
0x00 - 0x04 = Signature "PIM2"
0x04 - 0x06 = bpc ?
0x06 - 0x08 = 0x01 ?
0x08 - 0x10 = 0x00

Info:
0x10 - 0x14 = filesize without header size
0x14 - 0x18 = Palette size
0x18 - 0x1C = Palette offset without full header size (info size + header size)
0x1C - 0x1E = Info size
0x1E - 0x20 = number color
0x20 - 0x22 = 0x0150
0x22 - 0x24 = 0x0583
0x24 - 0x26 = width
0x26 - 0x28 = height
0x28 - 0x2A = ??
0x2A - 0x2C = ??
0x2C - 0x2E = ??
0x2E - 0x30 = 0x0080 (zoom ?)
0x30 - 0x32 = 0x0260 ??
0x32 - 0x40 = 0x00


If info size + header size > 0x40, there is a BUW chunck
BUW: 
Size = Info size + 0x30

Image info:
Size = filesize - palette size - Info size
1 entry = 1 byte

Palette:
0xRRGGBBAA ??


