Import("env")
import os

default_ssid = "Days"
default_password = "Sir Ivory Newton Sees All!"


ssid = default_ssid
if "WIFI_SSID" in os.environ:
    ssid = os.environ["WIFI_SSID"]

password = default_password
if "WIFI_PASSWORD" in os.environ:
    password = os.environ["WIFI_PASSWORD"]


env.Append(CPPDEFINES=[
    ("WIFI_SSID", "\\\"" + ssid + "\\\""),
    ("WIFI_PASSWORD", "\\\"" + password + "\\\"")
])
