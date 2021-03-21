"""
This code is based on examples kindly created, documented, and shared by Adafruit:

This is for use on (Linux) computers that are using CPython with
Adafruit Blinka to support CircuitPython libraries. CircuitPython does
not support PIL/pillow (python imaging library)!
"""

import board
import sys, getopt
import digitalio
from PIL import Image, ImageDraw, ImageFont
import adafruit_ssd1306

#print ('Number of arguments', len(sys.argv), 'arguments.')
#print ('Arguments list:', str(sys.argv))

#Grab the arguments
argv = sys.argv[1:]

textLine1 = ''
textLine2 = ''
textLine3 = ''
textSize = ''

#work through those arguments
try:
  opts, args = getopt.getopt(argv,"ha:b:c:s:",["firstLine=","secondLine=","thirdLine=","textSize="])
except getopt.GetoptError:
  print ('test.py -i <inputfile> -o <outputfile>')
  sys.exit(2)
for opt, arg in opts:
  if opt == '-h':
     print ('printToOLED.py -a <firstline> -b <secondline> -c <thirdline> -s <textsize>')
     sys.exit()
  elif opt in ("-a", "--firstLine"):
     textLine1 = arg
  elif opt in ("-b", "--secondLine"):
     textLine2 = arg
  elif opt in ("-c", "--thirdLine"):
     textLine3 = arg
  elif opt in ("-s", "--textSize"):
     textSize = int(arg)
#print ('First line is ', textLine1)
#print ('Second line is ', textLine2)
#print ('Third line is ', textLine3)
#print ('Text size is ', textSize)


# Define the Reset Pin
oled_reset = None

WIDTH = 128
HEIGHT = 64  
BORDER = 5

# Use for I2C.
i2c = board.I2C()
oled = adafruit_ssd1306.SSD1306_I2C(WIDTH, HEIGHT, i2c, addr=0x3C, reset=oled_reset)

# Use for SPI
# spi = board.SPI()
# oled_cs = digitalio.DigitalInOut(board.D5)
# oled_dc = digitalio.DigitalInOut(board.D6)
# oled = adafruit_ssd1306.SSD1306_SPI(WIDTH, HEIGHT, spi, oled_dc, oled_reset, oled_cs)

# Clear display.
oled.fill(0)
oled.show()

# Create blank image for drawing.
# Make sure to create image with mode '1' for 1-bit color.
image = Image.new("1", (oled.width, oled.height))

# Get drawing object to draw on image.
draw = ImageDraw.Draw(image)

# Load default font.
#font = ImageFont.load_default()

#load the Truetype font.
font = ImageFont.truetype("/home/pi/SmartChess/RaspberryPiCode/WorkSans-Medium.ttf", textSize)

# Draw Some Text
#textLine1 = "Choose a mode"
#textLine2 = "1 = Play computer"
#textLine3 = "2 = Remote play"

(font_width, font_height) = font.getsize(textLine1)
draw.text(
    (oled.width // 2 - font_width // 2, 0),
    textLine1,
    font=font,
    fill=255,
)


(font_width, font_height) = font.getsize(textLine2)
draw.text(
    (oled.width // 2 - font_width // 2, 20),
    textLine2,
    font=font,
    fill=255,
)

(font_width, font_height) = font.getsize(textLine3)
draw.text(
    (oled.width // 2 - font_width // 2, 40),
    textLine3,
    font=font,
    fill=255,
)

# Display image
oled.image(image)
oled.show()


