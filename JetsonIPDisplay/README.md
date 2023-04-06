# Setting up IP Display for Jetson Nano

1. `sudo apt-get install python-pip`
2. `sudo pip install Jetson.GPIO`
3. It should say requirement already satisfied. cd into that directory and then cd into `Jetson`
4. Add the GPIO rules into this directory. 
    1. `sudo touch 99-gpio.rules`
    2. `sudo gedit 99-gpio.rules`
        1. If using CLI only, you must use `vim` instead of `gedit`
    3. Copy all of the raw file and paste.
5. Test if everything works so far
    1. `python`
    2. `import RPi.GPIO`
6. `sudo pip install Adafruit-SSD1306`
7. Test if everything works so far
    1. `python`
    2. `import Adafruit_SSD1306`
8. Get a OLED screen.
9. Wire it.
    1. VCC to 5V
    2. GND to GND
    3. SDA to 3
    4. SCL to 5
10. Test it by running the script.
    1. `sudo python ip_startup.py`
    2. The OLED should now have the IP :)
    3. If it doesn't LOL NERD get owned
11. Set it to run on startup
    1. `sudo crontab -e`
    2. Type `@reboot sleep 20 && /usr/bin/python /home/jetson/ip_startup.py >/dev/null 2>&1` at the bottom of the crontab
    3. Escape, `:x`, enter to save and exit vim.
12. Test by restarting Jetson.
    1. `sudo reboot`