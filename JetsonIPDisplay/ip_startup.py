import RPi.GPIO as GPIO
import time
import Adafruit_SSD1306
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
from ip_getter import *
import random

disp = Adafruit_SSD1306.SSD1306_128_32(rst=None, i2c_bus=1, gpio=1)
disp.begin()
disp.clear()
disp.display()

width = disp.width
height = disp.height
image = Image.new('1',(width, height))

draw = ImageDraw.Draw(image)
draw.rectangle((0,0,width,height), outline=0, fill=0)

font = ImageFont.load_default()
ip = get_ip()	

#draw.text((10,10), str(random.randint(0,100)), font=font, fill = 255)
draw.text((10,10), "IP: " + ip, font=font, fill = 255)

disp.image(image)
disp.display()
time.sleep(0.5)
