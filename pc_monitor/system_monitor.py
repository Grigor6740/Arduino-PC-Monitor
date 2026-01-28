import psutil
import socket
from secret_variables import *
import json

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def secs2hours(secs):
    mm, ss = divmod(secs, 60)
    hh, mm = divmod(mm, 60)
    return "%d:%02d:%02d" % (hh, mm, ss)

def bytes_to_gb(total_memory, used_memory):
    gb_factor = 1024 ** 3

    total_memory_gb = total_memory / gb_factor
    used_memory_gb = used_memory / gb_factor

    return {"total": total_memory_gb, "used": used_memory_gb}

def system_monitor():
    cpu_usage = psutil.cpu_percent(interval=1)
    cpu_frequency = psutil.cpu_freq()
    # print(f"CPU Usage: {cpu_usage}%; CPU Frequency: {cpu_frequency.current} Mhz")

    memory = psutil.virtual_memory()
    total_memory = memory.total
    used_memory = memory.used

    memory_usage_to_gb = bytes_to_gb(total_memory, used_memory)
    # print(f"Memory Usage: {memory.percent}%; ({memory_usage_to_gb["used"]:.2f} GB / {memory_usage_to_gb["total"]:.2f} GB)")

    disk_usage = psutil.disk_usage('/')
    used_memory_d = disk_usage.used
    total_memory_d = disk_usage.total
    disk_memory_usage_gb = bytes_to_gb(total_memory_d, used_memory_d)
    # print(f"Disk Usage: {disk_usage.percent}%, ({disk_memory_usage_gb["used"]:.2f} GB / {disk_memory_usage_gb["total"]:.2f} GB)")

    battery = psutil.sensors_battery()
    # print(f"Battery percent: {battery.percent}; Status: {'Plugged' if battery.power_plugged else "Not plugged"}; Time left: {secs2hours(battery.secsleft)}")

    system_data = {
        "cpu": {
            "cpu_usage": cpu_usage,
            "cpu_frequency": cpu_frequency.current
        },
        "ram": {
            "percent": memory.percent,
            "used": memory_usage_to_gb["used"],
            "total": memory_usage_to_gb["total"]
        },
        "disk": {
            "percent": disk_usage.percent,
            "used": disk_memory_usage_gb["used"],
            "total": disk_memory_usage_gb["total"]
        },
        "battery": {
            "percent": battery.percent,
            # "status": {"Plugged" if battery.power_plugged else "Not plugged"},
            # "time_left": {secs2hours(battery.secsleft)}
        },
    }

    return system_data

def send_data():
    try:
        print(f"Connecting to {arduino_ip}")

        sock.connect((arduino_ip, port))
        print("Connected!")

        while True:
            system_data = system_monitor()

            json_str = json.dumps(system_data)
            print(json_str)
            sock.sendall(json_str.encode('utf-8'))
            print("Data sent.")
    except:
        print("An exception occurred")
    finally:
        sock.close()

if __name__ == '__main__':
    send_data()