# Setting up IP Display for Jetson Nano

Anything in the `box quotes like this` are commands to be run in the CLI from the Jetson's Home directory.

1. `sudo apt-get install python-pip`
2. Test to see if GPIO is already working:
    1. `python`
    2. `import RPi.GPIO`
3. If it does not give an error, skip to step 8. Otherwise, continue.
4. `sudo pip install Jetson.GPIO`
5. If it says "requirement already satisfied". cd into that directory and then cd into `Jetson`. Otherwise, skip to step 7.
6. Add the GPIO rules into this directory. 
    1. `sudo touch 99-gpio.rules`
    2. `sudo gedit 99-gpio.rules`
        1. If using CLI only, you must use `vim` instead of `gedit`
    3. Copy all of the raw file from this repo and paste.
7. Test if everything works so far
    1. `python`
    2. `import RPi.GPIO`
8. `sudo pip install Adafruit-SSD1306`
9. Test if it installed correctly
    1. `python`
    2. `import Adafruit_SSD1306`
10. Get a OLED screen.
11. Wire the OLED screen.
    1. VCC to 5V
    2. GND to GND
    3. SDA to 3
    4. SCL to 5
12. Copy the two python scripts from this repo and paste into the Jetson home directory. Test it by running the script.
    1. `sudo python ip_startup.py`
    2. The OLED should now have the IP :)
13. Set it to run on startup
    1. `sudo crontab -e`
    2. Type `@reboot sleep 20 && /usr/bin/python /home/jetson/ip_startup.py >/dev/null 2>&1` at the bottom of the crontab
    3. Escape, `:x`, enter to save and exit vim.
14. Test by restarting Jetson.
    1. `sudo reboot`
