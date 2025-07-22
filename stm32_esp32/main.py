import network
import time
import ujson as json
import urequests as requests
from machine import UART, Pin
import gc

gc.collect()

ssid = 'CTT-WIFI'
password = 'U5GWBV6P'

city = 'Ithaca'
country_code = 'USA'

api_key = 'a273d01219fa62932f38972a2c047c90'

#usart
uart = UART(0, baudrate=115200, tx=Pin(0), rx=Pin(1))

# --- Connect to Wi-Fi ---
def connect_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    while not wlan.isconnected():
        time.sleep(0.5)
    print("Wi-Fi connected:", wlan.ifconfig())

# --- Get weather data ---
def get_weather():
    url = 'http://api.openweathermap.org/data/2.5/weather?q=' + city + ',' + country_code + '&APPID=' + api_key
    response = requests.get(url)
    data = response.json()
    response.close()

    name = data['name']
    weather = data['weather'][0]['main']
    temp_c = round(data['main']['temp'] - 273.15, 1)  # Kelvin → Celsius

    return name, weather, temp_c

def get_time():
    url = "http://worldtimeapi.org/api/timezone/America/New_York"
    response = requests.get(url)
    data = response.json()
    response.close()

    dt = data['datetime']
    date_str, time_str = dt.split('T')
    time_str = time_str.split('.')[0]  # remove .123456
    return date_str, time_str

# --- Send over UART ---
def send_weather_via_uart(city, desc, temp, date, tstr):
    line = "{},{},{:.1f}C,{},{}\n".format(city, desc, temp, date, tstr)
    print("Sending:", line.strip())
    uart.write(line)

# --- MAIN ---
connect_wifi()
while True:
    try:
        city, desc, temp = get_weather()
        date, tstr = get_time()
        send_weather_via_uart(city, desc, temp, date, tstr)
    except Exception as e:
        print("Error:", e)
    time.sleep(100)  # update every 5 minutes
